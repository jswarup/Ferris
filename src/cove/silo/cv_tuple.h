//----------------------------------------- cv_tuple.h --------------------------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
class   Cv_Tuple; 

//_____________________________________________________________________________________________________________________________

template <typename T, int K>
struct Cv_TupleIndex
{
    typedef typename T::TupleBase TupleBase;

	T	*m_T;

	
	Cv_TupleIndex( T *p)
		:  m_T( p)
	{}

	auto	PVar( void)  {  return  Cv_TupleIndex< TupleBase, K -1>( static_cast< TupleBase *>( m_T)).PVar();  }
};

template <typename T>
struct Cv_TupleIndex< T, 0>
{
    typedef typename T::TupleBase TupleBase;

	T	*m_T;
	
	Cv_TupleIndex( T *p)
		:  m_T( p)
	{}

	auto	PVar( void) { return m_T->PVar(); }
};

//_____________________________________________________________________________________________________________________________

struct  Cv_TupleTools
{   
template< typename... Types >
    static constexpr auto   Make( Types&&... args );
	
template< typename T, typename... BT >
    static constexpr auto  	Make( T &&t1, Cv_Tuple< BT...> &&t2);

template< typename... Types >
    static constexpr auto 	Dump( std::ostream &ostr, const  Cv_Tuple< Types...> &tuple);
	
template< typename... Types >
    static constexpr void 	PtrAssign( Cv_Tuple< Types*...> &ptrTuple, const Cv_Tuple< Types...> &tuple);
	
};

//_____________________________________________________________________________________________________________________________

template < typename T, typename... Rest>
class   Cv_Tuple : public Cv_Tuple< Rest...>
{
public:
    T        m_Var;
   

    typedef T                     CType;
    typedef Cv_Tuple< T, Rest...> Tuple;
    typedef Cv_Tuple< Rest...>    TupleBase;

    enum {
        Sz = TupleBase::Sz +1,
    };
    
    Cv_Tuple( void)
    {}

    Cv_Tuple( const Cv_Tuple &t)
        : TupleBase( ( const TupleBase &) t), m_Var( t.m_Var)
    {}

    Cv_Tuple( const T &t, Rest... rest)
        : TupleBase( rest...), m_Var( t)
    {}

    Cv_Tuple( Cv_Tuple< T> &&t, TupleBase &&base)
        : TupleBase( std::move( base)), m_Var( std::move( t.m_Var)) 
    {}

template <class X>
    Cv_Tuple( X x)
        : TupleBase( x), m_Var( x)
    {}
    
	auto	PVar( void) { return &m_Var; }
	

template < int K>
    auto        Ptr( void)  { return Cv_TupleIndex< Tuple, Tuple::Sz -1 -K>( this).PVar(); } 

template < int K>
    auto        Var( void) const { return *const_cast<Tuple *>( this)->Ptr< K>(); }

template < int K>
    auto        Var( void)  { return *const_cast<Tuple *>( this)->Ptr< K>(); }

template < typename Lambda>
    auto    Compose( Lambda lambda) const
    {
        return Cv_TupleTools::Make( [=](auto... rest) { return lambda( uint32_t( Tuple::Sz -1), m_Var, rest...);},  TupleBase::Compose( lambda)); 
    };

template < typename Lambda>
    auto    Xform( Lambda lamdba) const
    {
       // return  Cv_TupleTools::Make( [=](auto... rest) { return param( uint32_t( 0), m_Var, rest...);});
        return   Cv_TupleTools::Make( lamdba( m_Var)...); 
    };
template < typename... X>
    auto    Invoke( X... args) const
    {
        return  Cv_TupleTools::Make( m_Var( args...), TupleBase::Invoke( args...)) ;
    }
template < typename... X>
    auto    Invoke( void) const
    {
        return  Cv_TupleTools::Make( m_Var(), TupleBase::Invoke()) ;
    }
};

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Cv_Tuple< T>
{
 public:
   T       m_Var;
    

    typedef T                       CType;
    typedef Cv_Tuple< T>            Tuple;
    typedef void                    TupleBase;
    
    enum {
        Sz = 1,
    };
    
    Cv_Tuple( void)
    {}

    Cv_Tuple( const T &t)
        : m_Var( t)
    {}

    Cv_Tuple( const Cv_Tuple &t)
        : m_Var( t.m_Var) 
    {}

template <class X>
    Cv_Tuple(  X x)
        : m_Var( x) 
    {}

    auto	PVar( void) { return &m_Var; }

template < int K>
    auto    Var( void) const { return m_Var; }

template < int K>
    auto    &Var( void) const { return m_Var; }

template < typename Lambda>
    auto    Compose( Lambda param) const
    {
        return  Cv_TupleTools::Make( [=](auto... rest) { return param( uint32_t( 0), m_Var, rest...);});
    };

template < typename... X>
    auto Invoke( X... args) const
    {
        return  Cv_TupleTools::Make( m_Var( args...)) ;
    }
template < typename... X>
    auto Invoke( void) const
    {
        return  Cv_TupleTools::Make( m_Var()) ;
    }
}; 

//_____________________________________________________________________________________________________________________________

template < int K, typename Tuple>
struct Ru_Index : public Ru_Index< K -1, typename Tuple::TupleBase>
{
};

template <typename Tuple>
struct Ru_Index< 0, Tuple>
{
    typedef typename Tuple::CType  Type;
};

//_____________________________________________________________________________________________________________________________


template< typename... Types >
constexpr auto   Cv_TupleTools::Make( Types&&... args )
{
	return Cv_Tuple< Types...>( std::forward<Types>( args)...);
}

template< typename T, typename... BT >
constexpr auto  Cv_TupleTools::Make( T &&t1, Cv_Tuple< BT...> &&t2)
{
	return Cv_Tuple< T, BT...>( std::forward<T>( t1), std::forward< Cv_Tuple< BT...> >( t2));
}


template< typename... Types >
constexpr auto   Cv_TupleTools::Dump( std::ostream &ostr, const  Cv_Tuple< Types...> &tuple)
{
    return tuple.Compose( [&ostr]( uint32_t k, auto var) 
    { 
        if ( k == ( Cv_Tuple< Types...>::Sz -1)) 
            ostr << "[ ";
        ostr << var << (( k == 0) ? "] " : ", "); 
        return true; 
    });
}

template< typename... Types >
constexpr void 	Cv_TupleTools::PtrAssign( Cv_Tuple< Types*...> &ptrTuple, const Cv_Tuple< Types...> &tuple) 
{
    *ptrTuple.Var() = *tuple.Var(); 
    PtrAssign( ( Cv_Tuple< Types*...>::TupleBase &) ptrTuple, ( const Cv_Tuple< Types...>::TupleBase &) tuple);
}

//_____________________________________________________________________________________________________________________________

template < typename X>
class Cv_Twin : public Cv_Tuple< X, X>
{
public:
    auto    First( void) const { return this->template Var< 0>(); }
    void    SetFirst( const X &x) { this->template Var< 0>() = x; }
    
    auto    Second( void) const { return this->template Var< 1>(); }
    void    SetSecond( const X &x) { this->template Var< 1>() = x; } 
};

//_____________________________________________________________________________________________________________________________
