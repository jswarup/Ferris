//----------------------------------------- ru_rubereactor.h --------------------------------------------------------------------
#pragma once


//-----------------------------------------------------------------------------------------------------------------------------

template < typename T >
class   Ru_Port 
{
    uint32_t        m_Id;

public:
    Ru_Port( void) 
        : m_Id( CV_UINT32_MAX)
    {}

};

//-----------------------------------------------------------------------------------------------------------------------------


template < typename T, typename... Rest>
class   Ru_Ingress : public Ru_Ingress< Rest...>
{
    Ru_Port< T>     m_Port;

    typedef typename  Ru_Ingress< Rest...>  Base;
public:
    Ru_Ingress( void)
    {}

    template < int K>
    auto        Port( void) { return Base::Port< K -1>(); }

    template <>
    auto        Port< 0>( void) { return &m_Port; }
};

template < typename T >
class   Ru_Ingress< T>
{
    Ru_Port< T>     m_Port;

public:
    Ru_Ingress( void)
    {}

    template < int K>
    auto        Port( void) { return &m_Port; }
}; 

//-----------------------------------------------------------------------------------------------------------------------------

class Ru_RubeRunner
{
    Ru_Ingress< int, double>   m_Ingress;  
public:
    Ru_RubeRunner( void);
};

//-----------------------------------------------------------------------------------------------------------------------------


