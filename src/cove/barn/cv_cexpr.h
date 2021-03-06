//___________________________________________ cv_cexpr.h ___________________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

class Cv_CExpr
{
public:
template < class X>
    static bool                 TestAspect( X x, X y) { return (( x & y) == y); }

template < class X>
    static constexpr X          DoubleTrim( X sz)  { return (( sz + sizeof( double) -1) / sizeof( double)) * sizeof( double) -1; }

template < class X>    
    static constexpr X          Pow2( X bits) { return bits == 0 ? 1 : 2 * Cv_CExpr::Pow2( bits -1); }

template < class X>
    static constexpr auto       InitVal( void) { return X(); }

template < int>
    static constexpr int        InitVal( void) { return 0; }

template < uint32_t>
    static constexpr uint32_t   InitVal( void) { return 0; }

    static constexpr uint64_t	LowMask( uint64_t x) { return (( uint64_t (0x1) << x) -1); }

template < class X>    
    static constexpr const X    &Min( const X &x1, const X &x2) { return x1 < x2 ? x1 : x2; }

};

//_____________________________________________________________________________________________________________________________

struct Cv_TypeEngage
{
template < typename T> 
    struct  Exist
    { 
        typedef void        Note; 
    };

   //_____________________________________________________________________________________________________________________________

template < typename T1, typename T2> 
    struct Same
    { 
    };


template < typename T1> 
    struct Same< T1, T1>
    { 
        typedef void      Note; 
    };

    //_____________________________________________________________________________________________________________________________

template < typename T1, typename T2> 
    struct NotSame
    { 
        typedef void      Note; 
    };


template < typename T1> 
    struct NotSame< T1, T1>
    { 
    };

    //_____________________________________________________________________________________________________________________________

template < typename T, typename = void> 
    struct  NotVoid
    { 
        typedef void        Note; 
    };

template < typename T> 
    struct  NotVoid< T, typename Same<  T, void>::Note> 
    {
    };
    
    //_____________________________________________________________________________________________________________________________

template < typename T, typename = void> 
    struct Stream
    { 
        typedef std::false_type  Note;
    };
       
template < typename T>
    struct Stream< T, typename Exist< decltype( operator<<( *( std::ostream *) nullptr, *( const T *) nullptr)) >::Type> 
    { 
        typedef std::true_type  Note;
    };
};

//_____________________________________________________________________________________________________________________________
