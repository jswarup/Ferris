//----------------------------------------- ru_rubesite.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"

//_____________________________________________________________________________________________________________________________

template < typename T, bool InPortFlg >
class   Ru_Port : public Cv_DLink< Ru_Port < T, InPortFlg>>
{
    typedef Ru_Port < T, InPortFlg>  Port;

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
};
  
//_____________________________________________________________________________________________________________________________


template < typename... T>
class   Ru_Inlet : public Cv_Tuple< Ru_Port< T, true>...>
{ 
public:
    typedef Cv_Tuple< Ru_Port< T, true>...>     Base;
    typedef Cv_Tuple< T...>                     Tuple;
    
    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base,  K>( this).PVar(); }
     
};


//_____________________________________________________________________________________________________________________________

template < typename... T>
class   Ru_Outlet : public Cv_Tuple< Ru_Port< T, false>...>
{ 
    
public:
    typedef Cv_Tuple< Ru_Port< T, false>...> 	Base;
    typedef Cv_Tuple< T...>  			    Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename T>
class   Ru_Connection
{
    typedef Ru_Port < T, true>      InPort;
    typedef Ru_Port < T, false>     OutPort;

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
            m_InPort = in;
        if ( m_OutPort)
            m_OutPort->GetTailLink()->SetNext( out);
        else
            m_OutPort = out;
    }
};

//_____________________________________________________________________________________________________________________________

template < typename... T>
class   Ru_Junction : public Cv_Tuple< Ru_Connection< T>...>
{ 
public:
    typedef Cv_Tuple< Ru_Connection< T>...> 	Base;
    typedef Cv_Tuple< T...>  			        Tuple;

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
template < int K>
    void       *Conn( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TJunctionSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Junction>::Note>
{
    typedef typename Module::Junction   Junction;  
    Junction                            m_Junction; 

template < int K>
    auto        Conn( void) { return Cv_TupleIndex< Junction, K>( &m_Junction).PVar();  }  
};
//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TInletSite 
{
template < int K>
    void       *InPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TInletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Inlet>::Note>
{
    typedef typename Module::Inlet   Inlet;  
    Inlet                            m_Inlet; 

template < int K>
    auto        InPort( void) { return Cv_TupleIndex< Inlet, K>( &m_Inlet).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class Module, typename = void> 
struct Ru_TOutletSite 
{
template < int K>
    void       *OutPort( void) { return nullptr;  } 
};

template <typename Module>
struct Ru_TOutletSite< Module,  typename Cv_TypeEngage::Exist< typename Module::Outlet>::Note>
{
    typedef typename Module::Outlet   Outlet;  
    Outlet                            m_Outlet; 

template < int K>
    auto        OutPort( void) { return Cv_TupleIndex< Outlet, K>( &m_Outlet).PVar();  }  
};

//_____________________________________________________________________________________________________________________________

template < class Module>
struct Ru_TSite : public Ru_RubeSite, public Ru_TInletSite< Module>, public Ru_TOutletSite< Module>, public Ru_TJunctionSite< Module>
{
    
     
    Ru_TSite( Ru_RubeSite *master)
        : Ru_RubeSite( master)
    {}

};

//_____________________________________________________________________________________________________________________________
 
template < class Module, typename = void>
struct Ru_Site : public Ru_TSite< Module>
{
    Ru_Site( Ru_RubeSite *master)
        : Ru_TSite< Module>( master)
    {}

};

//_____________________________________________________________________________________________________________________________

template < typename...  T>
struct Ru_Compound : public Cv_Tuple< typename T::Site...>
{
    typedef Cv_Tuple< typename T::Site...>  CBase;

    Ru_Compound( Ru_RubeSite *master)
        : CBase( master)
    {}

template < int K>
    auto    Child( void)  { return Cv_TupleIndex< CBase, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < class Module>
struct Ru_Site< Module, typename Cv_TypeEngage::Exist< typename Module::Compound>::Note > : public Ru_TSite< Module>, public Module::Compound
{
    typedef typename Module::Compound   Compound; 
    
public:
    Ru_Site( Ru_RubeSite *master)
        : Ru_TSite< Module>( master), Compound( master) 
    {
       // Module::Bind( this);
    }

};

//_____________________________________________________________________________________________________________________________



