//----------------------------------------- ru_rubesite.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_tuple.h"

//_____________________________________________________________________________________________________________________________

template < typename T >
class   Ru_Port : public Cv_DLink< Ru_Port < T>>
{

public:
    Ru_Port( void) 
    {}

   void     Join( Ru_Port *port)
    {
        Cv_DList< Ru_Port< T>>  list1( Cv_DLink< Ru_Port < T>>::GetHeadLink());
        Cv_DList< Ru_Port< T>>  list2( port->GetHeadLink());
        list1.Transfer( &list2); 
        return ;
    }
};
  
//_____________________________________________________________________________________________________________________________


template < typename... T>
class   Ru_Inlet : public Ru_Tuple< Ru_Port< T>...>
{ 
public:
    typedef Ru_Tuple< Ru_Port< T>...>   Base;
    typedef Ru_Tuple< T...>             Tuple;
    
    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return Ru_TupleIndex< Base,  K>( this).PVar(); }
     
};


//_____________________________________________________________________________________________________________________________

template < typename... T>
class   Ru_Outlet : public Ru_Tuple< Ru_Port< T>...>
{ 
    
public:
    typedef Ru_Tuple< Ru_Port< T>...> 	Base;
    typedef Ru_Tuple< T...>  			Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return Ru_TupleIndex< Base, K>( this).PVar(); }
};

//_____________________________________________________________________________________________________________________________

struct Ru_RubeSite : public Cv_Lackey< Ru_RubeSite>
{
 
public:
    Ru_RubeSite( Ru_RubeSite *master )
        : Cv_Lackey< Ru_RubeSite>( master) 
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
    auto        InPort( void) { return Ru_TupleIndex< Inlet, K>( &m_Inlet).PVar();  }

    template < int K>
    auto        OutPort( void) { return Ru_TupleIndex< Outlet, K>( &m_Outlet).PVar();  } 

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
struct Ru_Compound : public Ru_Tuple< Ru_Site< T>...>
{
    typedef Ru_Tuple< Ru_Site< T>...>  Base;

    Ru_Compound( Ru_RubeSite *master)
        : Base( master)
    {}

template < int K>
    auto    Child( void)  { return Ru_TupleIndex< Base, K>( this).PVar(); }
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



