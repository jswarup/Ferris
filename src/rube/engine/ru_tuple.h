//----------------------------------------- ru_tuple.h --------------------------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________

template <typename T, int K>
struct Ru_TupleIndex
{
	T	*m_T;
	
	Ru_TupleIndex( T *p)
		:  m_T( p)
	{}

	auto	PVar( void) { return Ru_TupleIndex< typename T::Base, K -1>( m_T).PVar(); }
};

template <typename T>
struct Ru_TupleIndex< T, 0>
{
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

    Ru_Tuple( const T &t, Rest... rest)
        : m_Var( t), Base( rest...)
    {}

	Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var), Base( t)
    {}

    Ru_Tuple( const T &t, const Base &rest)
        : m_Var( t), Base( rest)
    {}

    Ru_Tuple( Ru_Tuple &&t)
        : m_Var( t.m_Var), Base( t)
    {}

    Ru_Tuple( T &&t, Base &&rest)
        : m_Var( t), Base( rest)
    {}

    auto	PVar( void) { return &m_Var; }
	
template < int K>
    auto        Ptr( void)  { return Ru_TupleIndex< Tuple, K>( this).PVar(); } 

template < int K>
    auto        Var( void) const { return *const_cast<Tuple *>( this)->Ptr< K>(); }


template < typename Lambda>
    auto    Compose( const Lambda &param)
    {
        return Ru_TupleTools::Make( [=]() { return  param( m_Var); }, Base::Compose( param) );
    };

template < typename... X>
    auto Invoke( X... args)
    {
        return  Ru_TupleTools::Make( m_Var( args...), Base::Invoke( args...)) ;
    }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Tuple< T>
{
    T       m_Var;
    
public:
    typedef T                       CType;

    typedef Ru_Tuple< T>            Tuple;

    Ru_Tuple( void)
    {}

    Ru_Tuple( const T &t)
        : m_Var( t)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var) 
    {}

    Ru_Tuple( Ru_Tuple &&t)
        : m_Var( t.m_Var) 
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto    Var( void) const { return m_Var; }

template < typename Lambda>
    auto    Compose( const Lambda &param)
    {
        return Ru_TupleTools::Make( [=]() { return  param( m_Var); } );
    };


template < typename X>
    auto Invoke( X args)
    {
        return  Ru_TupleTools::Make( m_Var( args)) ;
    }
}; 

//_____________________________________________________________________________________________________________________________

struct Ru_TupleTools
{   

template< class... Types >
    static constexpr auto   Make( Types&&... args )
    {
        return Ru_Tuple< Types...>( args...);
    }


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
