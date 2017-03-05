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
        Cv_DList< Ru_Port< T>>  list1( GetHeadLink());
        Cv_DList< Ru_Port< T>>  list2( port->GetHeadLink());
        list1.Transfer( &list2); 
        return ;
    }
};

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Inlet : public Ru_Inlet< Rest...>
{
    Ru_Port< T>     m_Port;

    typedef typename  Ru_Inlet< Rest...>    Base;
       
public:
    typedef typename  Ru_Tuple< T, Rest...> Tuple;
    
    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Port; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Inlet< T>
{
    Ru_Port< T>                    m_Port;

public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Port; }
}; 

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Outlet : public Ru_Outlet< Rest...>
{
    Ru_Port< T>     m_Port;

    typedef typename  Ru_Outlet< Rest...>  Base;
public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Port; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Outlet< T>
{
    Ru_Port< T>     m_Port;

public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Port; }
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
    auto        InPort( void) { return m_Inlet.Port< K>(); }

    template < int K>
    auto        OutPort( void) { return m_Outlet.Port< K>(); }

};

//_____________________________________________________________________________________________________________________________
 
template < class Module, typename = void>
struct Ru_Site : public Ru_TSite< Module>
{
    Ru_Site( Ru_RubeSite *master)
        : Ru_TSite( master)
    {}

};

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
struct Ru_Compound : public Ru_Compound< Rest...>
{
    typedef Ru_Compound< Rest...>  Base;

    Ru_Site< T>         m_Site;

    Ru_Compound( Ru_RubeSite *master)
        : Base( master), m_Site( master)
    {}

template < int K>
    auto    Child( void)  { return Base::Child< K -1>(); }

template <>
    auto    Child< 0>( void)  { return &m_Site; }

};

//_____________________________________________________________________________________________________________________________

template <typename T>
struct Ru_Compound< T> 
{
    Ru_Site< T>         m_Site;

    Ru_Compound( Ru_RubeSite *master)
        : m_Site( master)
    {}

    
template < int K>
    auto    Child( void)  { return &m_Site; }
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
        Module::Bind( this);
    }

};
//_____________________________________________________________________________________________________________________________



