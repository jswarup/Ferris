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

template < typename T>
class   Ru_Port : public Cv_DLink< Ru_Port< T >>
{
    typedef Ru_Port< T>  Port; 

public: 
    Ru_Port( void) 
    {}

    Port    *Join( Ru_Port *port)
    {
        Cv_DList< Port>  list1( Cv_DLink< Port>::GetHeadLink());
        Cv_DList< Port>  list2( port->GetHeadLink());
        list1.Transfer( &list2); 
        return list1.GetHead();
    } 
};
  
//_____________________________________________________________________________________________________________________________

template < typename Module, typename T, bool InPortFlg >
class   Ru_ModulePort;

template < typename Module, typename T>
class   Ru_ModulePort< Module, T, false> : public Ru_Port< T>
{   
public: 

    Ru_ModulePort( void) 
    {}
};

template < typename Module, typename T>
class   Ru_ModulePort< Module, T, true> : public Ru_Port< T>
{   
public:
    T           *m_PVal;

    Ru_ModulePort( T *pVal) 
        :  m_PVal( pVal)
    {}
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
        : TupleBase( stave), m_Var()// stave->OpPtr< Sz -1>())
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
        : m_Var()//  stave->OpPtr< Sz -1>())
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



