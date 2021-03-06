//___________________________________________ cv_aid.h _________________________________________________________________________
#pragma once

#include		"cove/barn/cv_cexpr.h"
#include		"cove/silo/cv_cstr.h"
#include		"cove/silo/cv_tuple.h"

//_____________________________________________________________________________________________________________________________

struct Cv_Aid
{
    //_____________________________________________________________________________________________________________________________

    enum 
    {
        BufferSz = 512,
    };

    //_____________________________________________________________________________________________________________________________

template < class Iterator, class X>
    static void  Set( Iterator b, Iterator e, const X &x) 
    { 
        for ( ; b != e; ++b) 
        	*b = x;
    }

    //_____________________________________________________________________________________________________________________________

    template < class Iterator>
    static void  Init( Iterator b, Iterator e) 
    { 
        typedef typename Iterator::value_type ValueType;

        ValueType     x = Cv_CExpr::InitVal< ValueType>();
        for ( ; b != e; ++b) 
        	*b = x;
    }
    
    static bool                 FileExists( const char *fileName);
    
    static uint32_t             BinCoeff( uint32_t n, uint32_t k);

    static Cv_CStr              TrimFuncName( const Cv_CStr &name);

    static std::string          TypenameCleanup( const Cv_CStr &typeIdName);

    static std::string          CPPTypeStringCleanup( const Cv_CStr &typeName) ;

        
    static Cv_Twin< Cv_CStr>    LSplit( const Cv_CStr &str, const Cv_CStr &ws);

    static Cv_Twin< Cv_CStr>    LSplit( const Cv_CStr &str, char c);
    
    static Cv_Twin< Cv_CStr>    RSplit( const Cv_CStr &str, char c);

    //_____________________________________________________________________________________________________________________________
    
template < class T>
    static auto ToString( const T &obj)
    {
        std::stringstream   sstrm;
        sstrm << obj; 
        return sstrm.str();
    }
    
    static char                 HexToChar( int chr);

    static std::string          PrintBytes( const uint8_t *bytes, uint64_t len, uint32_t numBytesPerLine = CV_UINT32_MAX);

    static std::string          Format( const char *fmt, ...);
    
    //_____________________________________________________________________________________________________________________________
};

//_____________________________________________________________________________________________________________________________

template< class X> 
struct Cv_PtrVector : public std::vector< X *> 
{};

//_____________________________________________________________________________________________________________________________

template < typename Piece>
struct  Cv_Enumerator
{
    virtual bool            MoveNext( void) = 0;
    virtual Piece           Current( void) const = 0;
    
    virtual ~Cv_Enumerator( void) {}         
};

//_____________________________________________________________________________________________________________________________

struct Cv_Dummy
{

};

//_____________________________________________________________________________________________________________________________

template < class Container, class Base = Cv_Dummy>
struct  Cv_StdEnumeration : public Base
{
    typedef typename Container::iterator    Iterator;
    typedef typename Container::value_type  Piece;

    Container       m_Container;
    Iterator        m_It;
    
    Cv_StdEnumeration( const Container &container)
        : m_Container( container)
    {
        m_It = m_Container.end();
    }

    Cv_StdEnumeration( Container &&container)
        : m_Container( container)
    {
        m_It = m_Container.end();
    }

    bool    MoveNext( void) 
    { 
        if ( m_It == m_Container.end())
            m_It = m_Container.begin();
        else
            ++m_It;
        return m_It != m_Container.end();
    }

    Piece   Current( void) const
    {
        return *m_It;
    }

};


//_____________________________________________________________________________________________________________________________

template < class Container >
struct  Cv_StdEnumerator : public Cv_StdEnumeration< Container, Cv_Enumerator< typename Container::value_type>  >
{
    typedef  Cv_StdEnumeration< Container, Cv_Enumerator< typename Container::value_type>  >        Base;
    Cv_StdEnumerator( Container &&container)
        : Base( container)
    {}
};


//_____________________________________________________________________________________________________________________________
