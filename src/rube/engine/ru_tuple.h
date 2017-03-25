//----------------------------------------- ru_tuple.h --------------------------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________

template <typename T, int K>
struct Ru_TupleIndex
{
    typedef typename T::Base TBase;

	T	*m_T;

	
	Ru_TupleIndex( T *p)
		:  m_T( p)
	{}

	auto	PVar( void)  {  return  Ru_TupleIndex< TBase, K -1>( static_cast< TBase *>( m_T)).PVar();  }
};

template <typename T>
struct Ru_TupleIndex< T, 0>
{
    typedef typename T::Base TBase;

	T	*m_T;
	
	Ru_TupleIndex( T *p)
		:  m_T( p)
	{}

	auto	PVar( void) { return m_T->PVar(); }
};

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
class   Ru_Tuple : public Ru_Tuple< Rest...>
{
    T        m_Var;
   
public:
    typedef T                     CType;
    typedef Ru_Tuple< T, Rest...> Tuple;
    typedef Ru_Tuple< Rest...>    Base;

    Ru_Tuple( void)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var), Base( t)
    {}

    Ru_Tuple( const T &t, Rest... rest)
        : m_Var( t), Base( rest...)
    {}

	auto	PVar( void) { return &m_Var; }
	

template <class X>
    Ru_Tuple( const X &x)
        : Base( x), m_Var( x)
    {}
    
template < int K>
    auto        Ptr( void)  { return Ru_TupleIndex< Tuple, K>( this).PVar(); } 

template < int K>
    auto        Var( void) const { return *const_cast<Tuple *>( this)->Ptr< K>(); }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Tuple< T>
{
    T       m_Var;
    
public:
    typedef T                       CType;
    typedef Ru_Tuple< T>            Tuple;
    typedef void                    Base;
    
    Ru_Tuple( void)
    {}

    Ru_Tuple( const T &t)
        : m_Var( t)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var) 
    {}

template <class X>
    Ru_Tuple( const X &x)
        : m_Var( x) 
    {}

    auto	PVar( void) { return &m_Var; }

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
