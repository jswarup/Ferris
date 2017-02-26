//----------------------------------------- ru_tuple.h --------------------------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________


template < typename T, typename... Rest>
class   Ru_Tuple : public Ru_Tuple< Rest...>
{
    T        m_Var;
   
public:
    typedef T                               CType;
    typedef typename  Ru_Tuple< T, Rest...> Tuple;
    typedef typename  Ru_Tuple< Rest...>    Base;
    
    Ru_Tuple( void)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var), Base( t)
    {}

    Ru_Tuple( const T &t, Rest... rest)
        : m_Var( t), Base( rest)
    {}

    template < int K>
    auto        Var( void) const { return Base::Var< K -1>(); }

    template <>
    auto        Var< 0>( void) const { return m_Var; }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Tuple< T>
{
    T       m_Var;
    
public:
    typedef T                       CType;

    typedef typename  Ru_Tuple< T>  Tuple;

    Ru_Tuple( void)
    {}

    Ru_Tuple( const T &t)
        : m_Var( t)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var) 
    {}

    template < int K>
    auto    Var( void) const { return m_Var; }
}; 

//_____________________________________________________________________________________________________________________________

template < int K, typename Tuple>
struct Ru_Index : public Ru_Index< K -1, typename Tuple::Base>
{
};

template <typename Tuple>
struct Ru_Index< 0, Tuple>
{
    typedef typename Tuple::CType  Type;
};

//_____________________________________________________________________________________________________________________________
