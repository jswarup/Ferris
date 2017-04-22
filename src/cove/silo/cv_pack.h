//_______________________________________________ cv_pack.h _______________________________________________________________
#pragma once

#include    "cove/barn/cv_frager.h"

//_____________________________________________________________________________________________________________________________

template <class X>
class Cv_Pack 
{
protected:
    X		    *m_Begin;
    uint32_t    m_Sz;
    
    bool        IsOwned( void) const { return Cv_Frager< 31, 1>( m_Sz).Get(); }
    void        SetSize( uint32_t sz) { m_Sz = Cv_Frager< 0, 31>( m_Sz).Set( sz); }

public:
    Cv_Pack( void) 
        : m_Begin( nullptr), m_Sz( 0) 
    {}
    
    Cv_Pack( const Cv_Pack &arr) 
        : m_Begin( arr.m_Begin), m_Sz( arr.m_Sz) 
    {}
    
    
    Cv_Pack( X *pBegin, X *pEnd)	
        : m_Begin( pBegin), m_Sz( 0)
    {
        SetSize( uint32_t( pEnd -pBegin));
    }
    
    Cv_Pack( X *pBegin, uint32_t sz)	
        : m_Begin( pBegin), m_Sz( 0)	
    {
        SetSize( sz);
    }
    
    ~Cv_Pack( void)	
    {
        if ( IsOwned())
            delete [] m_Begin;  
    }
    
    //_____________________________________________________________________________________________________________________________
    
    uint32_t        Size( void) const { return m_Sz & ~( 1 << 31); }
    void            SetOwned( void) { m_Sz = Cv_Frager< 31, 1>( m_Sz).Set( true); }
    
    X               &operator[]( int i)	  {	return m_Begin[ i]; 	}
    const X			&operator[]( int i)	const {	return m_Begin[ i]; 	}
    operator const	X*( void)	const	{	return m_Begin; }
    
    X               *Begin( void) const { return m_Begin;  }
    X               *End( void) const { return m_Begin +Size(); } 
    
    Cv_Pack< X>     First( uint32_t k) const { return Cv_Pack< X>( m_Begin, m_Begin +k); }
    Cv_Pack< X>     Second( uint32_t k) const { return Cv_Pack< X>( m_Begin +k, m_Begin +Size()); }
    
    
    //_____________________________________________________________________________________________________________________________
    
    void    Reverse( void) 
    {
        for ( X *pBegin = Begin(), pEnd = Begin() +Size() -1; pBegin < pEnd; ++pBegin, --pEnd)
        {
            X   x = *pBegin;
            *pBegin = *pEnd;
            *pEnd = x;
        }
        return;
    }
    
    //_____________________________________________________________________________________________________________________________

};

//_____________________________________________________________________________________________________________________________


