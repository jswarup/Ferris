//----------------------------------------- ru_site.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "rube/engine/ru_stave.h"

//_____________________________________________________________________________________________________________________________

template < typename T>
struct   Ru_Port : public Cv_DLink< Ru_Port< T>>
{
    typedef Ru_Port< T>  Port; 
    union Data  
    {
        Cv_PtrVector< T>    *m_Listener;
        T                   *m_PVal;
    };

    bool                    m_InFlg;
    Data                    m_Data;
    
    Ru_Port( Cv_PtrVector< T>  *l) : m_InFlg( false)  {  m_Data.m_Listener = l; }

    Ru_Port( T *pVal) : m_InFlg( true) {  m_Data.m_PVal = pVal; }

    void    Join( Ru_Port *port)
    {
        Cv_DList< Port>  list1( Cv_DLink< Port>::GetHeadLink());
        Cv_DList< Port>  list2( port->GetHeadLink());

        for ( Ru_Port  *port1 = list1.GetHead(); port1; port1 = port1->GetNext())
            for ( Ru_Port  *port2 = list2.GetHead(); port2; port2 = port2->GetNext())
                if ( port1->m_InFlg != port2->m_InFlg)
                    port1->m_InFlg ? port2->OutConnect( port1) :  port1->OutConnect( port2);
                   
        while ( port = list2.Pop())
            list1.Append( port);        
        return;
    } 

    void    OutConnect( Ru_Port *port) { m_Data.m_Listener->push_back( port->m_Data.m_PVal); }
};
  
//_____________________________________________________________________________________________________________________________
 
template < typename Module, uint32_t Index, bool InFlg>
struct Ru_StavePortAdaptor;

template < typename Module, uint32_t Index>
struct Ru_StavePortAdaptor< Module, Index, true>
{
    Ru_Stave< Module>   *m_Stave;

    Ru_StavePortAdaptor( Ru_Stave< Module> *stave)
        : m_Stave( stave)
    {};

    auto    Stub( void) { return m_Stave->VarPtr< Index>(); }
};

template < typename Module, uint32_t Index>
struct Ru_StavePortAdaptor< Module, Index, false>
{
    Ru_Stave< Module>   *m_Stave;

    Ru_StavePortAdaptor( Ru_Stave< Module> *stave)
        : m_Stave( stave)
    {};

    auto    Stub( void) { return m_Stave->Listener< Index>(); }
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

    Ru_Port< T>         m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : TupleBase( stave), m_Var( Ru_StavePortAdaptor< Module, Sz -1, true>( stave).Stub())
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

template < typename Module, typename T> 
struct   Ru_Inlet< Module, T>   
{  
    typedef Cv_Tuple< T>  			       Tuple; 
    enum {
        Sz = 1,
    };

    Ru_Port< T>                             m_Var;

    Ru_Inlet( Ru_Stave< Module> *stave)
        : m_Var( Ru_StavePortAdaptor< Module, Sz -1, true>( stave).Stub())
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
    Ru_Port<  T>                            m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
        : TupleBase( stave), m_Var( Ru_StavePortAdaptor< Module, Sz -1, false>( stave).Stub()) 
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

template < typename Module, typename T> 
struct   Ru_Outlet< Module, T>   
{  
    typedef Cv_Tuple< T>                    Tuple; 
    enum {
        Sz = 1,
    };
    Ru_Port<  T>                            m_Var;

    Ru_Outlet( Ru_Stave< Module> *stave)
        : m_Var( Ru_StavePortAdaptor< Module, Sz -1, false>( stave).Stub()) 
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



