//_______________________________________________ cv_trotter.h _______________________________________________________________
#pragma once


//_____________________________________________________________________________________________________________________________

template < class Anchor>
class Cv_GraphTrotter
{
protected:
    //_____________________________________________________________________________________________________________________________

    std::vector< uint32_t>          m_StateQue;
    std::vector< bool>              m_StateStatus;
     
    //_____________________________________________________________________________________________________________________________

template < class  Store, typename Method>
    class Support
    {
        Store               *m_Store;
        Cv_GraphTrotter     *m_Context;
        Anchor              *m_Anchor;
        Method              m_Method;
        
        void        Rejig( void)
        {
            if (!( (2 * m_Store->Size()) < m_Context->m_StateStatus.size()))
                m_Context->m_StateStatus.resize( 2 * m_Store->Size(), false);
        }
    public:
        Support( Store *store, Anchor *anchor, Method method)
            :  m_Store( store), m_Context( anchor), m_Anchor( anchor), m_Method( method)
        {}

    template < class Arg>
        bool    CallMethod( uint32_t stateId, bool entryFlg, Arg *arg)
        {
            return (m_Anchor->*m_Method)( stateId,  entryFlg, arg);
        }

        bool    CallMethod( uint32_t stateId, bool entryFlg, void *arg)
        {
            return (m_Anchor->*m_Method)( stateId,  entryFlg);
        }

    template < class Arg>
        void  DoDepthFirst( uint32_t spawnId, Arg *arg)
        {
            Rejig();
            if ( m_Context->IsExited( spawnId))
                return;
                
            m_Context->DoQueState( spawnId);

            uint32_t    qSize = 0;
            while ( !!( qSize = m_Context->SzQue()))
            {
                uint32_t        stateId = m_Context->Peek();
                if ( m_Context->IsExited( stateId))     
                {
                    if ( qSize == m_Context->SzQue())
                        m_Context->Pop();    
                    continue;
                }

                bool            entryFlg = !m_Context->IsEntered( stateId);        // if it has not been entered then it is an entry
                if ( entryFlg)
                {
                    bool    res = CallMethod( stateId, true, arg);
                    Rejig();
                    if ( res)                                           
                    {
                        m_Context->MarkEntered( stateId);
                    }
                    else // mark it processed
                    {
                        if ( qSize == m_Context->SzQue())
                            m_Context->Pop();
                        m_Context->MarkEntered( stateId);
                        m_Context->MarkExited( stateId);
                    }
                }
                else
                {   
                    Rejig();
                    bool        qFlg = m_Context->DoQueUnenteredDests( stateId);
                    
                    if ( qFlg)            
                        continue;   
                    bool    res = CallMethod( stateId, false, arg);
                    Rejig();
                    if ( res)
                    {
                        if ( qSize == m_Context->SzQue())
                            m_Context->Pop();
                        m_Context->MarkExited( stateId);         // if it was an exit => processing done
                    }
                    else
                    {
                        CV_ERROR_ASSERT( m_Context->Peek() != stateId)           // else will go in infinite loop
                    }
                }
                
            }
            return;
        }
    };   

    //_____________________________________________________________________________________________________________________________

public:
    Cv_GraphTrotter(  void)
    {}
 

    bool        IsEntered( uint32_t stateId)  const { return  m_StateStatus[ 2 * stateId]; }

    void        MarkEntered( uint32_t stateId) { m_StateStatus[ 2 * stateId] = true; }
        
    bool        IsExited( uint32_t stateId)  const {  return m_StateStatus[ 2 * stateId +1]; }

    void        MarkExited( uint32_t stateId) { m_StateStatus[ 2 * stateId +1] = true; }
        
    void        UnmarkExited( uint32_t stateId) {  m_StateStatus[ 2 * stateId +1] = false; }
        
    

    uint32_t    SzQue( void)  const {  return uint32_t( m_StateQue.size()); }

    uint32_t    Root( void)  const {  return m_StateQue.front(); }
    
    uint32_t    Peek( void)  const {  return m_StateQue.back(); }
        
    uint32_t    Pop( void)  
    { 
        uint32_t        stateId = m_StateQue.back();
        m_StateQue.pop_back();
        return stateId;
    }
            
    void        DoQueState( uint32_t stateId)  
    {
        m_StateQue.push_back(stateId);
        UnmarkExited( stateId); 
    }

template < class Iterator>
    void        DoQueStates( Iterator b, Iterator e)   
    { 
        Rejig(); 
        for ( Iterator it = b; it != e; ++it) 
        DoQueState( *it); 
    }
        
    bool        DoQueUnenteredDests( uint32_t stateId)                              // test if children been entered : if not push them in..
    {
        
        Cv_Enumerator< uint32_t>    *stateEnumerator = m_Store->StateEnumerator( stateId);
        if ( !stateEnumerator)
            return false;
        bool                        qFlg = false;
        while ( stateEnumerator->MoveNext())
        {
            uint32_t    dest = stateEnumerator->Current();
            if (( stateId != dest) && !IsEntered( dest) && ( qFlg = true))           // child need to have entered before we process further, if not que it for processing
                DoQueState( dest);
        }
        return qFlg;
    }

template < class Store, typename Method>
    void  DoDepthTraversal( uint32_t stateId, Store *store, Method method)
    {   
        m_StateStatus.resize( 2 * store->Size(), false);
        Support<  Store, Method>( store, static_cast< Anchor *>( this), method).DoDepthFirst( stateId, ( void *) nullptr); 
    }
        
          
template < class Store, typename Method, typename Args>
    void  DoDepthTraversal( uint32_t stateId, Store *store, Method method, Args *args)
    {   
        m_StateStatus.resize( 2 * store->Size(), false);
        Support<  Store, Method>( store, static_cast< Anchor *>( this), method).DoDepthFirst( stateId, args); 
    }
};

//_____________________________________________________________________________________________________________________________
