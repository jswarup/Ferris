//----------------------------------------- ru_tuple.h --------------------------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
class   Ru_Tuple; 

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

struct  Ru_TupleTools
{   
template< typename... Types >
    static constexpr auto   Make( Types&&... args );
	
template< typename T, typename... BT >
    static constexpr auto  Make( T &&t1, Ru_Tuple< BT...> &&t2);
	
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

    enum {
        Sz = Base::Sz +1,
    };
    
    Ru_Tuple( void)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : Base( ( const Base &) t), m_Var( t.m_Var)
    {}

    Ru_Tuple( const T &t, Rest... rest)
        : Base( rest...), m_Var( t)
    {}

    Ru_Tuple( const Ru_Tuple< T> &t, const Base &base)
        : Base( base), m_Var( t.Var< 0>()) 
    {}

template <class X>
    Ru_Tuple( X x)
        : Base( x), m_Var( x)
    {}
    
	auto	PVar( void) { return &m_Var; }
	

template < int K>
    auto        Ptr( void)  { return Ru_TupleIndex< Tuple, Tuple::Sz -1 -K>( this).PVar(); } 

template < int K>
    auto        Var( void) const { return *const_cast<Tuple *>( this)->Ptr< K>(); }

template < typename Lambda>
    auto    Compose( Lambda param) const
    {
        return Ru_TupleTools::Make( [=](auto... rest) { return param( uint32_t( Tuple::Sz -1), m_Var, rest...);},  Base::Compose( param)); 
    };

    template < typename... X>
    auto Invoke( X... args) const
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
    typedef void                    Base;
    
    enum {
        Sz = 1,
    };
    
    Ru_Tuple( void)
    {}

    Ru_Tuple( const T &t)
        : m_Var( t)
    {}

    Ru_Tuple( const Ru_Tuple &t)
        : m_Var( t.m_Var) 
    {}

template <class X>
    Ru_Tuple(  X x)
        : m_Var( x) 
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto    Var( void) const { return m_Var; }

template < typename Lambda>
    auto    Compose( Lambda param) const
    {
        return  Ru_TupleTools::Make( [=](auto... rest) { return param( uint32_t( 0), m_Var, rest...);});
    };

template < typename... X>
    auto Invoke( X... args) const
    {
        return  Ru_TupleTools::Make( m_Var( args...)) ;
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


template< typename... Types >
constexpr auto   Ru_TupleTools::Make( Types&&... args )
{
	return Ru_Tuple< Types...>( std::forward<Types>( args)...);
}

template< typename T, typename... BT >
constexpr auto  Ru_TupleTools::Make( T &&t1, Ru_Tuple< BT...> &&t2)
{
	return Ru_Tuple< T, BT...>( std::forward<T>( t1), std::forward< Ru_Tuple< BT...> >( t2));
}


//_____________________________________________________________________________________________________________________________
