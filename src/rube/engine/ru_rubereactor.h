//----------------------------------------- ru_rubereactor.h --------------------------------------------------------------------
#pragma once


//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Slot 
{
    uint32_t        m_Id;

public:
    Ru_Slot( void) 
        : m_Id( CV_UINT32_MAX)
    {}

};

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Ingress : public Ru_Ingress< Rest...>
{
    Ru_Slot< T>     m_Port;

    typedef typename  Ru_Ingress< Rest...>  Base;
public:
    Ru_Ingress( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Port; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Ingress< T>
{
    Ru_Slot< T>     m_Port;

public:
    Ru_Ingress( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Port; }
}; 

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Outgress : public Ru_Outgress< Rest...>
{
    Ru_Slot< T>     m_Port;

    typedef typename  Ru_Ingress< Rest...>  Base;
public:
    Ru_Outgress( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Port; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Outgress< T>
{
    Ru_Slot< T>     m_Port;

public:
    Ru_Outgress( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Port; }
};

//_____________________________________________________________________________________________________________________________


class Ru_RubeRunner
{
    Ru_Ingress< int, double>   m_Ingress;  
public:
    Ru_RubeRunner( void);
};

//_____________________________________________________________________________________________________________________________



