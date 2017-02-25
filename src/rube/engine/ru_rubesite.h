//----------------------------------------- ru_rubesite.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_tuple.h"

//_____________________________________________________________________________________________________________________________

template < typename T >
class   Ru_Track : public Cv_DLink< Ru_Track < T>>
{

public:
    Ru_Track( void) 
    {}

};

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Inlet : public Ru_Inlet< Rest...>
{
    Ru_Track< T>     m_Track;

    typedef typename  Ru_Inlet< Rest...>    Base;
       
public:
    typedef typename  Ru_Tuple< T, Rest...> Tuple;
    
    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Track; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Inlet< T>
{
    Ru_Track< T>                    m_Track;

public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Inlet( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Track; }
}; 

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Outlet : public Ru_Outlet< Rest...>
{
    Ru_Track< T>     m_Track;

    typedef typename  Ru_Outlet< Rest...>  Base;
public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Track( void) { return Base::Track< K -1>(); }

    template <>
    auto        Track< 0>( void) { return &m_Track; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Outlet< T>
{
    Ru_Track< T>     m_Track;

public:
    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Outlet( void)
    {}

    template < int K>
    auto        Track( void) { return &m_Track; }
};

//_____________________________________________________________________________________________________________________________

class Ru_RubeSite : public Cv_Lackey< Ru_RubeSite>
{
    std::string         m_Name;

public:
    Ru_RubeSite( const std::string &name, Ru_RubeSite *master)
        : Cv_Lackey< Ru_RubeSite>( master), m_Name( name)
    {}
};

//_____________________________________________________________________________________________________________________________

template < class Module>
class Ru_Site : public Ru_RubeSite
{
    typedef typename Module::Inlet      Inlet;
    typedef typename Module::Outlet     Outlet;

    Inlet                               m_Inlet;  
    Outlet                              m_Outlet;  

public:
    Ru_Site( const std::string &name, Ru_RubeSite *master)
        : Ru_RubeSite( name, master)
    {}

};

//_____________________________________________________________________________________________________________________________



