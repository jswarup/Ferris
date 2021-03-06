//_______________________________________________ cv_traversor.h _______________________________________________________________
#pragma once

#include    "cove/barn/cv_aid.h"

//_____________________________________________________________________________________________________________________________

template < class Elem>
class  Cv_TreeTraversor
{
protected:
    //_____________________________________________________________________________________________________________________________

template < class Anchor, typename Method>
    class Support
    {
        Cv_TreeTraversor    *m_Context;
        Anchor              *m_Anchor;
        Method              m_Method;
    public:
        Support( Cv_TreeTraversor *ctxt, Anchor *anchor, Method method)
            :   m_Context( ctxt), m_Anchor( anchor), m_Method( method)
        {}

    template < class Arg>
        bool    CallMethod( const Elem &elem, bool entryFlg, Arg *arg)
        {
            return (m_Anchor->*m_Method)( elem, m_Context, entryFlg, arg);
        }

    template < class Arg>
        bool  DoDepthFirst( Arg *arg)
        {
            while ( m_Context->HasElem())
            {
                if ( !m_Context->IsCurrEntered())    
                {
                    m_Context->MarkCurrEntered();                    
                    bool    res = CallMethod( m_Context->CurrElem(), true, arg);
                    if ( !res)
                        return false;
                }
                if ( m_Context->IsCurrEntered())
                {
                    Elem    elem = m_Context->CurrElem();
                    m_Context->PopCurr();
                    bool res = CallMethod( elem, false, arg);
                    if ( !res)
                        return false;
                }
            }
            return true;            
        }
    };

    //_____________________________________________________________________________________________________________________________

    std::vector< Elem>      m_ElemQue;
    std::vector< bool>      m_StatusQue;

public:
    Cv_TreeTraversor( void)
    {}

    void        PushElem( const Elem &elem) {  m_ElemQue.push_back( elem); m_StatusQue.push_back( false); }
    bool        IsCurrEntered( void)  const { return  m_StatusQue.back(); }
    void        MarkCurrEntered( void) { m_StatusQue.back() = true; }
    const Elem  &CurrElem( void) { return m_ElemQue.back(); }
    void        PopCurr( void) {  m_ElemQue.pop_back(); m_StatusQue.pop_back(); }
    bool        HasElem( void) const { return !!m_ElemQue.size(); }
   
template < class Anchor, typename Method, typename Args>
    bool    DoDepthTraversal( const Elem &elem, Anchor *anchor, Method method, Args *args)
    {
        PushElem( elem);   
        Support<  Anchor, Method>           support( this, anchor, method);
        return support.DoDepthFirst( args); 
    }
};

//_____________________________________________________________________________________________________________________________

template < class Store>
class Cv_GraphTraversor
{
protected:
    //_____________________________________________________________________________________________________________________________

    struct StoreIfc
    {
        uint32_t    Size( void) const { return CV_UINT32_MAX; }
        uint32_t    SzDest( uint32_t stateId) const { return CV_UINT32_MAX; }  
        uint32_t    DestAt( uint32_t stateId, uint32_t i) const { return CV_UINT32_MAX; }    
    };

    //_____________________________________________________________________________________________________________________________

    Store                           *m_Store;
    std::vector< uint32_t>          m_StateQue;
    std::vector< bool>              m_StateStatus;
     
    //_____________________________________________________________________________________________________________________________

template < class Anchor, typename Method>
    class Support
    {
        Cv_GraphTraversor   *m_Context;
        Anchor              *m_Anchor;
        Method              m_Method;

    public:
        Support( Cv_GraphTraversor *ctxt, Anchor *anchor, Method method)
            :   m_Context( ctxt), m_Anchor( anchor), m_Method( method)
        {}

    template < class Arg>
        bool    CallMethod( uint32_t stateId, bool entryFlg, Arg *arg)
        {
            return (m_Anchor->*m_Method)( stateId, m_Context, entryFlg, arg);
        }

        bool    CallMethod( uint32_t stateId, bool entryFlg, void *arg)
        {
            return (m_Anchor->*m_Method)( stateId, m_Context, entryFlg);
        }

    template < class Arg>
        void  DoDepthFirst( uint32_t spawnId, Arg *arg)
        {
            m_Context->Rejig();
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
                    m_Context->Rejig();
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
                    bool        qFlg = m_Context->DoQueUnenteredDests( stateId);
                    
                    if ( qFlg)            
                        continue;   
                    bool    res = CallMethod( stateId, false, arg);
                    m_Context->Rejig();
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
    Cv_GraphTraversor( Store *repos)
        : m_Store( repos)
    {}

    Store      *GetStore( void)  const {  return m_Store; }

    bool        IsEntered( uint32_t stateId)  const { return  m_StateStatus[ 2 * stateId]; }

    void        MarkEntered( uint32_t stateId) { m_StateStatus[ 2 * stateId] = true; }
        
    bool        IsExited( uint32_t stateId)  const {  return m_StateStatus[ 2 * stateId +1]; }

    void        MarkExited( uint32_t stateId) { m_StateStatus[ 2 * stateId +1] = true; }
        
    void        UnmarkExited( uint32_t stateId) {  m_StateStatus[ 2 * stateId +1] = false; }
        
    void        Rejig( void)
    {
        if (!( (2 * m_Store->Size()) < m_StateStatus.size()))
            m_StateStatus.resize( 2 * m_Store->Size(), false);
    }

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
        Rejig();
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

template < class Anchor, typename Method>
    void  DoDepthTraversal( uint32_t stateId, Anchor *anchor, Method method)
    {   
        m_StateStatus.resize( 2 * m_Store->Size(), false);
        Support<  Anchor, Method>( this, anchor, method).DoDepthFirst( stateId, ( void *) nullptr); 
    }
        
          
template < class Anchor, typename Method, typename Args>
    void  DoDepthTraversal( uint32_t stateId, Anchor *anchor, Method method, Args *args)
    {   
        m_StateStatus.resize( 2 * m_Store->Size(), false);
        Support<  Anchor, Method>( this, anchor, method).DoDepthFirst( stateId, args); 
    }
};

//_____________________________________________________________________________________________________________________________
