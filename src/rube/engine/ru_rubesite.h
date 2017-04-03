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

   void     Join( Ru_Port *port)
    {
        Cv_DList< Port>  list1( Cv_DLink< Port>::GetHeadLink());
        Cv_DList< Port>  list2( port->GetHeadLink());
        list1.Transfer( &list2); 
        return ;
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
    auto        Port( void) { return Cv_TupleIndex< Base,  Base::Sz -1 -K>( this).PVar(); }
     
};


//_____________________________________________________________________________________________________________________________

template < typename... T>
class   Ru_Outlet : public Cv_Tuple< Ru_Port< T, false>...>
{ 
    
public:
    typedef Cv_Tuple< Ru_Port< T, false>...> 	Base;
    typedef Cv_Tuple< T...>  			Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, Base::Sz -1 -K>( this).PVar(); }
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

template < class Module>
struct Ru_TSite : public Ru_RubeSite
{
    typedef typename Module::Inlet      Inlet;
    typedef typename Module::Outlet     Outlet;

    Inlet                               m_Inlet;  
    Outlet                              m_Outlet;   
     
    Ru_TSite( Ru_RubeSite *master)
        : Ru_RubeSite( master)
    {}

    template < int K>
    auto        InPort( void) { return Cv_TupleIndex< Inlet, Inlet::Sz -1 -K>( &m_Inlet).PVar();  }

    template < int K>
    auto        OutPort( void) { return Cv_TupleIndex< Outlet, Outlet::Sz -1 -K>( &m_Outlet).PVar();  } 
     
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
struct Ru_Compound : public Cv_Tuple< Ru_Site< T>...>
{
    typedef Cv_Tuple< Ru_Site< T>...>  Base;

    Ru_Compound( Ru_RubeSite *master)
        : Base( master)
    {}

template < int K>
    auto    Child( void)  { return Cv_TupleIndex< Base, Base::Sz -1 -K>( this).PVar(); }
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



