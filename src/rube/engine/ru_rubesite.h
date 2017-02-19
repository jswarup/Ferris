//----------------------------------------- ru_rubesite.h --------------------------------------------------------------------
#pragma once

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

    typedef typename  Ru_Inlet< Rest...>  Base;
public:
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
    Ru_Track< T>     m_Track;

public:
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

    typedef typename  Ru_Inlet< Rest...>  Base;
public:
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
    Ru_RubeSite( Ru_RubeSite *master, const std::string &name)
        : Cv_Lackey< Ru_RubeSite>( master), m_Name( name)
    {}
};

//_____________________________________________________________________________________________________________________________

template < class Inlet, class Outlet>
class Ru_Site : public Ru_RubeSite
{
    Inlet               m_Inlet;  
    Outlet              m_Outlet;  

public:
    Ru_Site( Ru_RubeSite *master, const std::string &name)
        : Ru_RubeSite( master, name)
    {}

};

//_____________________________________________________________________________________________________________________________



