//----------------------------------------- ru_site.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "rube/engine/ru_stave.h"

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

template < typename Module, typename T>
class   Ru_ModulePort< Module, T, true> : public Ru_Port< T, true>
{   
public:
    T       *m_PVal;
    Ru_ModulePort( T *pVal) 
        : m_PVal( pVal)
    {}

};

template < typename Module, bool InPortFlg, int Ind, typename T, typename... Rest>
class   Ru_ModulePortTuple : public Ru_ModulePortTuple< Module, InPortFlg, Ind +1, Rest...> 
{
public:
    Ru_Port< T, InPortFlg>      m_Var;

    typedef Ru_ModulePortTuple< Module, InPortFlg, Ind +1, Rest...>    TupleBase;

    auto	PVar( void) { return &m_Var; }
};

template < typename Module, bool InPortFlg, int Ind, typename T>
class   Ru_ModulePortTuple< Module, InPortFlg,  Ind, T>  
{
public:
    Ru_Port< T, InPortFlg>      m_Var;
    
    auto	PVar( void) { return &m_Var; }
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, typename... Rest> 
class   Ru_Inlet : public Ru_Inlet< Module, Rest...>
{   
public:
    typedef Ru_Inlet< Module, Rest...> 	            TupleBase;
    typedef Cv_Tuple< T, Rest...>  			        Tuple;
    typedef Cv_Tuple< T*, Rest*...>  			    PtrTuple;
    enum {
        Sz = TupleBase::Sz +1,
    };
    Ru_ModulePort< Module, T, true>                 m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : TupleBase( stave), m_Var( stave->VarPtr< Sz -1>())
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

template < typename Module, typename T> 
class   Ru_Inlet< Module, T>   
{ 
public:
    typedef Cv_Tuple< T>  			                Tuple;
    typedef Cv_Tuple< T*>  			                PtrTuple;
    enum {
        Sz = 1,
    };
    Ru_ModulePort< Module, T, true>                m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : m_Var( stave->VarPtr< Sz -1>())
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, typename... Rest> 
class   Ru_Outlet : public Ru_Outlet< Module, Rest...>
{   
public:
    typedef Ru_Outlet< Module, Rest...> 	        TupleBase;
    typedef Cv_Tuple< T, Rest...>  			        Tuple;
    typedef Cv_Tuple< T*, Rest*...>  			    PtrTuple;
    enum {
        Sz = TupleBase::Sz +1,
    };
    Ru_ModulePort< Module, T, false>                 m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
        : TupleBase( stave)
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

template < typename Module, typename T> 
class   Ru_Outlet< Module, T>   
{ 
public:
    typedef Cv_Tuple< T>  			                Tuple;
    typedef Cv_Tuple< T*>  			                PtrTuple;
    enum {
        Sz = 1,
    };
    Ru_ModulePort< Module, T, false>                m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return PVar(); }
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


template < typename Module, typename T, typename... Rest> 
class   Ru_Junction : public Ru_Junction< Module, Rest...>
{ 
    
public:
    typedef Ru_Junction< Module, Rest...> 	        TupleBase;
    typedef Cv_Tuple< T, Rest...>  			        Tuple;
    Ru_Connection< T>                               m_Var;

    Ru_Junction( Ru_Stave< Module> *stave)
        : TupleBase( stave)
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Connection( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

template < typename Module, typename T> 
class   Ru_Junction< Module, T>   
{ 

public:
    typedef Cv_Tuple< T>  			                Tuple;

    Ru_Connection< T>                               m_Var;
    Ru_Junction( Ru_Stave< Module> *stave)
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Connection( void) { return PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TJunctionSite 
{
    Ru_TJunctionSite( Ru_Stave< Module> *stave)
    {}

template < int K>
    void       *Conn( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TJunctionSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Junction>::Note> : public Module::Junction
{
    typedef typename Module::Junction   Junction;   

    Ru_TJunctionSite( Ru_Stave< Module> *stave)
        : Junction( stave)
    {}
template < int K>
    auto        Conn( void) { return Cv_TupleIndex< Junction, K>( this).PVar();  }  
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
struct Ru_TSite : public Ru_TInletSite< TModule>, public Ru_TOutletSite< TModule>, public Ru_TJunctionSite< TModule>
{
    typedef TModule     Module;
    Ru_Stave< Module>   *m_Stave;

    Ru_TSite( Ru_Stave< Module> *stave)
        : Ru_TInletSite< Module>( stave), Ru_TOutletSite< Module>( stave), Ru_TJunctionSite< Module>( stave), m_Stave( stave)
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



