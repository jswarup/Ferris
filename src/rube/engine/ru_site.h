//----------------------------------------- ru_rubesite.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"

//_____________________________________________________________________________________________________________________________

template < class Module>
struct Ru_TSite;

template < typename T>
class   Ru_Connection;

//_____________________________________________________________________________________________________________________________

template < typename T, bool InPortFlg >
class   Ru_Port : public Cv_DLink< Ru_Port< T, InPortFlg>>
{
    typedef Ru_Port< T, InPortFlg>  Port;
    Ru_Connection< T>               *m_Conn;
public:
    const static bool In = InPortFlg; 

    Ru_Port( void) 
    {}

    Port    *Join( Ru_Port *port)
    {
        Cv_DList< Port>  list1( Cv_DLink< Port>::GetHeadLink());
        Cv_DList< Port>  list2( port->GetHeadLink());
        list1.Transfer( &list2); 
        return list1.GetHead();
    }
    
    void SetConnection( Ru_Connection< T> *conn) {   m_Conn = conn; }
};
  
//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, bool InPortFlg >
class   Ru_ModulePort : public Ru_Port< T, InPortFlg>
{   
public:
    Ru_ModulePort( void) 
    {}

};
  
//_____________________________________________________________________________________________________________________________


template < typename Module, typename... T>
class   Ru_Inlet : public Cv_Tuple< Ru_ModulePort< Module, T, true>...>
{ 
public:
    typedef Cv_Tuple< Ru_ModulePort< Module, T, true>...>       Base;
    typedef Cv_Tuple< T...>                                     Tuple;
    
    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base,  K>( this).PVar(); }
     
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename... T>
class   Ru_Outlet : public Cv_Tuple< Ru_ModulePort< Module, T, false>...>
{ 
    
public:
    typedef Cv_Tuple< Ru_ModulePort< Module, T, false>...> 	Base;
    typedef Cv_Tuple< T...>  			                    Tuple;
    typedef Cv_Tuple< T*...>  			                    PtrTuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename T>
class   Ru_Connection
{
    typedef Ru_Port< T, true>      InPort;
    typedef Ru_Port< T, false>     OutPort;

    InPort          *m_InPort;
    OutPort         *m_OutPort;

public:
    Ru_Connection( void)
        : m_InPort( nullptr), m_OutPort( nullptr)
    {}
    
    void    Join( OutPort *out, InPort *in)
    {
        if ( m_InPort)
            m_InPort->GetTailLink()->SetNext( in);
        else
            m_InPort = in->GetHeadLink();
        for ( InPort *in = m_InPort; in; in = in->GetNext())
            in->SetConnection( this);

        if ( m_OutPort)
            m_OutPort->GetTailLink()->SetNext( out);
        else
            m_OutPort = out->GetHeadLink();
        for ( OutPort *out = m_OutPort; out; out = out->GetNext())
            out->SetConnection( this);
    }
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename... T>
class   Ru_Junction : public Cv_Tuple< Ru_Connection< T>...>
{ 

public:
    typedef Cv_Tuple< Ru_Connection< T>...> 	        Base;
    typedef Cv_Tuple< T...>  			                Tuple;

    Ru_Junction( void)
    {}

    template < int K>
    auto        Connection( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

struct Ru_RubeSite : public Cv_Minion< Ru_RubeSite>
{
public:
    Ru_RubeSite( Ru_RubeSite *master )
        : Cv_Minion< Ru_RubeSite>( master) 
    {}
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TJunctionSite 
{
    Ru_TJunctionSite( Ru_TSite< Module> *site)
    {}

template < int K>
    void       *Conn( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TJunctionSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Junction>::Note>
{
    typedef typename Module::Junction   Junction;  
    Junction                            m_Junction; 

    Ru_TJunctionSite( Ru_TSite< Module> *site)
    {}
template < int K>
    auto        Conn( void) { return Cv_TupleIndex< Junction, K>( &m_Junction).PVar();  }  
};
//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TInletSite 
{
    Ru_TInletSite( Ru_TSite< Module> *site)
    {}

template < int K>
    void       *InPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TInletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Inlet>::Note> : public Module::Inlet
{
    typedef typename Module::Inlet   Inlet;   
    
    Ru_TInletSite( Ru_TSite< Module> *site)
    {}

template < int K>
    auto        InPort( void) { return Cv_TupleIndex< Inlet, K>( this).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TOutletSite 
{
    Ru_TOutletSite( Ru_TSite< Module> *site)
    {}

template < int K>
    void       *OutPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TOutletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Outlet>::Note> : public Module::Outlet
{
    typedef typename Module::Outlet   Outlet; 

    Ru_TOutletSite( Ru_TSite< Module> *site)
    {}
template < int K>
    auto        OutPort( void) { return Cv_TupleIndex< Outlet, K>( this).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class TModule>
struct Ru_TSite : public Ru_RubeSite, public Ru_TInletSite< TModule>, public Ru_TOutletSite< TModule>, public Ru_TJunctionSite< TModule>
{
    typedef TModule Module;

    Ru_TSite( Ru_RubeSite *master)
        : Ru_RubeSite( master), Ru_TInletSite< Module>( this), Ru_TOutletSite< Module>( this), Ru_TJunctionSite< Module>( this)
    {}
};

//_____________________________________________________________________________________________________________________________
 
template < class Module, typename = void>
struct Ru_CSite : public Ru_TSite< Module>
{
    Ru_CSite( Ru_RubeSite *master)
        : Ru_TSite< Module>( master)
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct  Ru_Site : public Ru_CSite< Module>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;

    Ru_Site( Ru_RubeSite *master)
        :   Ru_CSite< Module>( master)
    {}

 
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_DSite : public Ru_Site< Module>
{
    Ru_DSite( Ru_RubeSite *master)
        : Ru_Site< Module>( master)
    {}
};

template <typename Module>
struct Ru_DSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Site>::Note> : public Module::Site
{    
    Ru_DSite( Ru_RubeSite *master)
        :  Module::Site( master)
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename...  T>
struct Ru_Compound : public Cv_Tuple< Ru_DSite< T>...>
{
    typedef Cv_Tuple< Ru_DSite< T>...>  CBase;
    typedef Cv_Tuple< T...>             Tuple;

    Ru_Compound( Ru_RubeSite *master)
        : CBase( master)
    {}

template <int K>
    auto    Child( void)  { return Cv_TupleIndex< CBase, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < class ModuleT>
struct Ru_CSite< ModuleT, typename Cv_TypeEngage::Exist< typename ModuleT::Compound>::Note > : public Ru_TSite< ModuleT>, public ModuleT::Compound
{
    typedef typename ModuleT::Compound  Compound; 
    typedef ModuleT                     Module; 
    
public:
    Ru_CSite( Ru_RubeSite *master)
        : Ru_TSite< ModuleT>( master), Compound( master) 
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
struct  Ru_Site<Module, typename Cv_TypeEngage::Exist< typename Module::Action>::Note > : public Ru_CSite< Module>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;

    Ru_Site( Ru_RubeSite *master)
        :   Ru_CSite< Module>( master)
    {}


};

//_____________________________________________________________________________________________________________________________



