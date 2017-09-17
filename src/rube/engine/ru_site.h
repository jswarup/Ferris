//----------------------------------------- ru_site.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "rube/engine/ru_stave.h"

//_____________________________________________________________________________________________________________________________

template < typename T>
struct   Ru_Port : public Cv_DLink< Ru_Port< T>>
{
    typedef Ru_Port< T>  Port; 
    bool                    m_InFlg;
    
    Ru_Port( void) 
        : m_InFlg( false)  
    { } 

    void    Join( Ru_Port *port)
    {
        Cv_DList< Port>  list1( Cv_DLink< Port>::GetHeadLink());
        Cv_DList< Port>  list2( port->GetHeadLink());
        for ( Ru_Port  *port1 = list1.GetHead(); port1; port1 = port1->GetNext())
            for ( Ru_Port  *port2 = list2.GetHead(); port2; port2 = port2->GetNext())
                if ( port1->m_InFlg != port2->m_InFlg)
                    port1->m_InFlg ? port2->OutConnect( port1) :  port1->OutConnect( port2);
                 
        while (( port = list2.Pop()))
            list1.Append( port);        
        return;
    } 

    virtual void    OutConnect( Ru_Port *port) = 0;
};
  
//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, bool InFlg>
struct Ru_ModulePort;


template < typename Module, typename T>
struct Ru_ModulePort< Module, T, true> : public Ru_Port< T>
{ 
    union Data
    {
        Cv_PtrVector< T>    *m_Listener;
        T                   *m_PVal;
    };
    Data                    m_Data;


    Ru_Stave< Module>       *m_Stave;

    Ru_ModulePort(Ru_Stave< Module> *stave, T *pVal)
        : Ru_Port< T>( ), m_Stave(stave)
    {
        m_Data.m_PVal = pVal;
        m_InFlg = true;
    }
    void    WireInputs(void)
    {
        Cv_DList<  Ru_Port< T>>  list(Cv_DLink<  Ru_Port< T> >::GetHeadLink());
        for (Ru_Port< T> *port = list.GetHead(); port; port = port->GetNext())
            if ((port != this) && !port->m_InFlg)
                Ru_Port< T>::m_Data.m_Listener->push_back(port->m_Data.m_PVal);
        return;
    }

    void    OutConnect(Ru_Port *port) {}
};

template < typename Module, typename T>
struct Ru_ModulePort< Module, T, false> : public Ru_Port< T>
{
    union Data
    {
        Cv_PtrVector< T>    *m_Listener;
        T                   *m_PVal;
    };
    Data                    m_Data;


    Ru_Stave< Module>       *m_Stave;

    Ru_ModulePort( Ru_Stave< Module> *stave, Cv_PtrVector< T>  *l)
        : Ru_Port< T>(), m_Stave( stave)
    {
        m_Data.m_Listener = l;

        m_InFlg = false;
    }

    void    WireInputs( void)
    {
        Cv_DList<  Ru_Port< T>>  list( Cv_DLink<  Ru_Port< T> >::GetHeadLink());
        for ( Ru_Port< T> *port = list.GetHead(); port; port = port->GetNext())
            if ( ( port != this)  && !port->m_InFlg)
                Ru_Port< T>::m_Data.m_Listener->push_back( port->m_Data.m_PVal);
        return;
    }

    void    OutConnect( Ru_Port< T> *p)
    {
        Ru_ModulePort< Module, T, true> *inPort = static_cast< Ru_ModulePort< Module, T, true> *>( p);

        m_Data.m_Listener->push_back( inPort->m_Data.m_PVal);
    }
};

//_____________________________________________________________________________________________________________________________
 
template < typename Module, uint32_t Index, bool InFlg>
struct Ru_StavePortAdaptor;

template < typename Module, uint32_t Index>
struct Ru_StavePortAdaptor< Module, Index, true>
{
    Ru_Stave< Module>       *m_Stave;

    Ru_StavePortAdaptor( Ru_Stave< Module> *stave)
        : m_Stave( stave)
    {};

    auto    Stub( void) { auto  varPtr = Ru_StaveTools< Ru_Stave< Module>, Index>::VarPtr(); return varPtr(m_Stave); }
};

template < typename Module, uint32_t Index>
struct Ru_StavePortAdaptor< Module, Index, false>
{
    Ru_Stave< Module>       *m_Stave;

    Ru_StavePortAdaptor( Ru_Stave< Module> *stave)
        : m_Stave( stave)
    {};

    auto        Stub( void) { auto listener = Ru_StaveTools< Ru_Stave< Module>, Index>::Listener(); return listener( m_Stave); ; }
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, typename... Rest> 
struct   Ru_Inlet : public Ru_Inlet< Module, Rest...>
{   
    typedef Ru_Inlet< Module, Rest...> 	    TupleBase;
    typedef Cv_Tuple< T, Rest...>  		    Tuple;
    typedef Cv_Tuple< T*, Rest*...>  	    PtrTuple;
    enum {
        Sz = TupleBase::Sz +1,
    };

    Ru_ModulePort< Module, T, true>         m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : TupleBase( stave), m_Var( stave, Ru_StavePortAdaptor< Module, Sz -1, true>( stave).Stub())
    {}

    auto	    PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< TupleBase, K>( this).PVar(); }
};

template < typename Module, typename T> 
struct   Ru_Inlet< Module, T>   
{  
    typedef Cv_Tuple< T>  	                Tuple; 
    enum {
        Sz = 1,
    };

    Ru_ModulePort< Module, T, true>        m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : m_Var( stave, Ru_StavePortAdaptor< Module, Sz -1, true>( stave).Stub())
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, typename... Rest> 
struct   Ru_Outlet : public Ru_Outlet< Module, Rest...>
{  
    typedef Ru_Outlet< Module, Rest...> 	TupleBase;
    typedef Cv_Tuple< T, Rest...>  			Tuple; 
    enum {
        Sz = TupleBase::Sz +1,
    };
    Ru_ModulePort< Module, T, false>        m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
        : TupleBase( stave), m_Var( stave, NULL)
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< TupleBase, K>( this).PVar(); }
};

template < typename Module, typename T> 
struct   Ru_Outlet< Module, T>   
{  
    typedef Cv_Tuple< T>                    Tuple; 
    enum {
        Sz = 1,
    };
    Ru_ModulePort< Module, T, false>        m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
        : m_Var( stave, Ru_StavePortAdaptor< Module, Sz -1, false>( stave).Stub())
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TInletSite 
{
    Ru_TInletSite( Ru_Stave< Module> *stave)
    {}

template < int K>
    void       *InPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TInletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Inlet>::Note> : public Module::Inlet
{
    typedef typename Module::Inlet   Inlet;   
    
    Ru_TInletSite( Ru_Stave< Module> *stave)
        : Inlet( stave) {}

template < int K>
    auto        InPort( void) { return Cv_TupleIndex< Inlet, K>( this).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TOutletSite 
{
    Ru_TOutletSite( Ru_Stave< Module> *stave)
    {}

template < int K>
    void       *OutPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TOutletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Outlet>::Note> : public Module::Outlet
{
    typedef typename Module::Outlet   Outlet; 

    Ru_TOutletSite( Ru_Stave< Module> *stave)
        : Outlet( stave)
    {}

template < int K>
    auto        OutPort( void) { return Cv_TupleIndex< Outlet, K>( this).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class TModule>
struct Ru_TSite : public Ru_TInletSite< TModule>, public Ru_TOutletSite< TModule> 
{
    typedef TModule     Module;
    Ru_Stave< Module>   *m_Stave;

    Ru_TSite( Ru_Stave< Module> *stave)
        : Ru_TInletSite< Module>( stave), Ru_TOutletSite< Module>( stave), m_Stave( stave)
    {}
};

//_____________________________________________________________________________________________________________________________
 
template < class Module, typename = void>
struct Ru_CSite : public Ru_TSite< Module>
{
    Ru_CSite( Ru_Stave< Module> *stave)
        : Ru_TSite< Module>( stave)
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct  Ru_Site : public Ru_CSite< Module>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;

    Ru_Site( Ru_Stave< Module> *stave)
        :   Ru_CSite< Module>( stave)
    {}
 
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_DSite : public Ru_Site< Module>
{
    Ru_DSite( Ru_Stave< Module> *stave)
        : Ru_Site< Module>( stave)
    {}
};

template <typename Module>
struct Ru_DSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Site>::Note> : public Module::Site
{    
    Ru_DSite( Ru_Stave< Module> *stave)
        :  Module::Site( stave)
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
struct Ru_Compound : public Ru_Compound< Rest...>
{
    typedef Ru_Compound< Rest...>                       TupleBase;
    typedef Cv_Tuple< Ru_Stave< T>, Ru_Stave< Rest>...> SubStaves;

    Ru_DSite< T>        m_Var;

    Ru_Compound(  Cv_Tuple< Ru_Stave< T>, Ru_Stave< Rest>...> *t)
        : TupleBase( t), m_Var( t->PVar())
    {}
 
	auto	PVar( void) { return &m_Var; }

template <int K>
    auto    Child( void)  { return Cv_TupleIndex< Ru_Compound, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename T>
struct Ru_Compound< T> 
{
    typedef Cv_Tuple< Ru_Stave< T> >          SubStaves;

    Ru_DSite< T>        m_Var;

    Ru_Compound( SubStaves *t)
        : m_Var( t->PVar())
    {}
 
    auto	PVar( void) { return &m_Var; }

template <int K>
    auto    Child( void)  { return &m_Var; }

};
//_____________________________________________________________________________________________________________________________

template < class ModuleT>
struct Ru_CSite< ModuleT, typename Cv_TypeEngage::Exist< typename ModuleT::Compound>::Note > : public Ru_TSite< ModuleT>, public ModuleT::Compound
{
    typedef typename ModuleT::Compound  Compound; 
    typedef ModuleT                     Module; 
    
public:
    Ru_CSite( Ru_Stave< Module> *stave)
        : Ru_TSite< ModuleT>( stave), Compound( stave) 
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
struct  Ru_Site<Module, typename Cv_TypeEngage::Exist< typename Module::Action>::Note > : public Ru_CSite< Module>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;

    Ru_Site( Ru_Stave< Module> *stave)
        :   Ru_CSite< Module>( stave)
    {}


};

//_____________________________________________________________________________________________________________________________



