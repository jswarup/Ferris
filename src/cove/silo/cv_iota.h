//_______________________________________________ cv_iota.h _______________________________________________________________
#pragma once

#include    <stdint.h>
#include    <vector>
#include	"cove/barn/cv_compare.h"

//_____________________________________________________________________________________________________________________________

struct  Cv_IotaDef
{
    enum  BitWidth
    { 
        BitWidth_Unknown = 0,
        BitWidth_1       = 1,
        BitWidth_2       = 2,
        BitWidth_4       = 3,
        BitWidth_8       = 4,
        BitWidth_16      = 5,
        BitWidth_32      = 6,
        BitWidth_64      = 7,
    };

    static uint32_t     MaxBitWidth( uint64_t x)
    {
        uint32_t    r = 0;
        for ( ; (x >> r) != 0; r++)
            ;
        return r;
    }

    static constexpr BitWidth   BitWidthFromIndex( uint32_t wd)
    {
        return ( wd == 1 ? BitWidth_1 : ( wd == 2 ? BitWidth_2 : ( wd == 4 ? BitWidth_4 : ( wd == 8 ? BitWidth_8 : ( wd == 16 ? BitWidth_16 : ( wd == 32 ? BitWidth_32 : ( wd == 64 ? BitWidth_64 : BitWidth_Unknown)))))));
    }

    static uint32_t             BitSize( BitWidth wdIndex);

    static BitWidth             BitWidthForValue( uint64_t value);   

    static uint32_t             MaxBitVecSize( uint32_t imgSz);

    static uint32_t             MemSize( uint32_t sz, BitWidth wdIndex); 
    
    
};

//_____________________________________________________________________________________________________________________________

template < Cv_IotaDef::BitWidth BW>
struct  Cv_IotaTrailImpl
{};


//_____________________________________________________________________________________________________________________________

template <>
struct  Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_1>
{
template < uint32_t I>
    static uint8_t    Get( uint8_t v) { return ( v >> I) & 0x1; }

template < uint32_t I>
    static uint8_t    Set( uint8_t t, uint64_t v) { return ( t & ~( 0x1 << ( I))) | (( v & 0x1) << (  I)); }
    
    uint8_t     GetAt( uint32_t i) const 
    {
        uint8_t     v = reinterpret_cast< const uint8_t*>( this)[ i / 8];
        switch ( i % 8)
        {
            case 0 : return  Get< 0>( v);
            case 1 : return  Get< 1>( v);
            case 2 : return  Get< 2>( v);
            case 3 : return  Get< 3>( v);
            case 4 : return  Get< 4>( v);
            case 5 : return  Get< 5>( v);
            case 6 : return  Get< 6>( v);
            case 7 : return  Get< 7>( v);
        }
        return  0;
    }
            
    void        SetAt( uint32_t i, uint64_t v)
    {
        uint8_t     *t = &reinterpret_cast< uint8_t*>( this)[ i / 8];
        switch ( i % 8)
        {
            case 0 : *t = Set< 0>( *t, v); return;
            case 1 : *t = Set< 1>( *t, v); return;
            case 2 : *t = Set< 2>( *t, v); return;
            case 3 : *t = Set< 3>( *t, v); return;
            case 4 : *t = Set< 4>( *t, v); return;
            case 5 : *t = Set< 5>( *t, v); return;
            case 6 : *t = Set< 6>( *t, v); return;
            case 7 : *t = Set< 7>( *t, v); return;
        }
         
        return; 
    }
    static uint32_t    MemSize( uint32_t sz) { return ( sz + 7)/8; }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_2>
{
template < uint32_t I>
    static uint8_t  Get( uint8_t v) { return ( v >> ( 2 * I)) & 0x3; }

template < uint32_t I>
    static uint8_t  Set( uint8_t t, uint64_t v) { return ( t & ~( 0x3 << ( 2 * I))) | (( v & 0x3) << ( 2 * I)); }
    
    uint8_t         GetAt( uint32_t i) const 
    {
        uint8_t     v = reinterpret_cast< const uint8_t*>(this)[ i / 4];
        switch ( i % 4)
        {
            case 0 : return  Get< 0>( v);
            case 1 : return  Get< 1>( v);
            case 2 : return  Get< 2>( v);
            case 3 : return  Get< 3>( v); 
        }
        return 0;
    }
            
    void            SetAt( uint32_t i, uint64_t v)
    {
        uint8_t     *t = &reinterpret_cast< uint8_t*>( this)[ i / 4];
        switch ( i % 4)
        {
            case 0 : *t = Set< 0>( *t, v); return;
            case 1 : *t = Set< 1>( *t, v); return;
            case 2 : *t = Set< 2>( *t, v); return;
            case 3 : *t = Set< 3>( *t, v); return;
        }
        return; 
    }
    static uint32_t MemSize( uint32_t sz) { return ( sz + 3)/4; }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_4>
{
    uint8_t         GetAt( uint32_t i) const { return ( i % 2 ? ( reinterpret_cast< const uint8_t*>(this)[ i / 2] >> 4) : reinterpret_cast< const uint8_t*>(this)[ i / 2]) & 0xF; }
    void            SetAt( uint32_t i, uint64_t v)  { reinterpret_cast< uint8_t*>(this)[ i / 2 ] = uint8_t(( i % 2) ? (( v << 4) | (reinterpret_cast< uint8_t*>(this)[ i / 2 ] & 0x0F)) :  ((reinterpret_cast< uint8_t*>(this)[ i / 2 ] & 0xF0) | v));  }
    static uint32_t MemSize( uint32_t sz) { return ( sz + 1)/2; }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_8>
{
    uint8_t         GetAt( uint32_t i) const  { return reinterpret_cast< const uint8_t*>(this)[ i]; }
    void            SetAt( uint32_t i, uint64_t v) { reinterpret_cast< uint8_t*>(this)[ i] = uint8_t( v); }
    static uint32_t MemSize( uint32_t sz) { return sz * sizeof( uint8_t); }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_16>
{
    uint16_t        GetAt( uint32_t i) const  { return reinterpret_cast< const uint16_t*>(this)[ i]; }
    void            SetAt( uint32_t i, uint64_t v) { reinterpret_cast< uint16_t*>(this)[ i] = uint16_t( v); }
    static uint32_t MemSize( uint32_t sz) { return sz * sizeof( uint16_t); }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_32>
{
    uint32_t        GetAt( uint32_t i) const  { return reinterpret_cast< const uint32_t*>(this)[ i]; }
    void            SetAt( uint32_t i, uint64_t v) { reinterpret_cast< uint32_t*>(this)[ i] = uint32_t( v); }
    static uint32_t MemSize( uint32_t sz) { return sz * sizeof( uint32_t); }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_64>
{
    uint64_t        GetAt( uint32_t i) const  { return reinterpret_cast< const uint16_t*>(this)[ i]; }
    void            SetAt( uint32_t i, uint64_t v) { reinterpret_cast< uint64_t*>(this)[ i] = uint64_t( v); }
    static uint32_t MemSize( uint32_t sz) { return sz * sizeof( uint64_t); }
};
    
//_____________________________________________________________________________________________________________________________

template <  Cv_IotaDef::BitWidth BW>
struct Cv_IotaTrail  : public Cv_IotaTrailImpl< BW>
{
    void            SetZero( uint32_t sz) { memset( this, 0, Cv_IotaTrailImpl< BW>::MemSize( sz));  }  

template< class Iter>
    void    DoCopy( const Iter &b, const Iter &e)
    {
        uint32_t    i = 0;
        for ( Iter it = b; it != e; ++it, ++i)
            SetAt( i, *it); 
    } 
};

//_____________________________________________________________________________________________________________________________

template < uint32_t Sz, uint32_t Wd>
struct Fe_IArray 
{};

template < uint32_t Sz>
struct Fe_IArray< Sz, 1> : public Cv_IotaTrail< Cv_IotaDef::BitWidth_1>
{
    uint8_t     m_Array[ ( Sz + 7)/8];
};

template < uint32_t Sz>
struct Fe_IArray< Sz, 2> : public Cv_IotaTrail< Cv_IotaDef::BitWidth_2>
{
    uint8_t     m_Array[ ( Sz + 3)/4];
};

template < uint32_t Sz>
struct Fe_IArray< Sz, 4>  : public Cv_IotaTrail< Cv_IotaDef::BitWidth_4>
{
    uint8_t     m_Array[ ( Sz + 1)/2];
};

template < uint32_t Sz>
struct Fe_IArray< Sz, 8> : public Cv_IotaTrail< Cv_IotaDef::BitWidth_8>
{
     uint8_t    m_Array[ Sz];
};

template < uint32_t Sz>
struct Fe_IArray< Sz, 16>  : public Cv_IotaTrail< Cv_IotaDef::BitWidth_16>
{
    uint16_t    m_Array[ Sz];
};

template < uint32_t Sz>
struct Fe_IArray< Sz, 32> : public Cv_IotaTrail< Cv_IotaDef::BitWidth_32>
{
    uint32_t    m_Array[ Sz];
};

template < uint32_t Sz>  
struct Fe_IArray< Sz, 64>  :  public Cv_IotaTrail< Cv_IotaDef::BitWidth_64>
{
    uint64_t    m_Array[ Sz];
};

//_____________________________________________________________________________________________________________________________

class Cv_IotaArrayDef : public Cv_IotaDef
{
public:
    uint64_t            GetAt( BitWidth bw, uint8_t  *iarr, uint32_t i) const
    {
        switch ( bw)
        {
            case 1 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_1>  *>( iarr)->GetAt( i);
            case 2 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_2>  *>( iarr)->GetAt( i);
            case 3 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_4>  *>( iarr)->GetAt( i);
            case 4 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_8>  *>( iarr)->GetAt( i);
            case 5 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_16> *>( iarr)->GetAt( i);
            case 6 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_32> *>( iarr)->GetAt( i);
            case 7 :  return reinterpret_cast< Cv_IotaTrailImpl< BitWidth_64> *>( iarr)->GetAt( i);
            default:  break;
        }
        return 0;
    }
    
    void                SetAt( BitWidth bw, uint8_t  *iarr, uint32_t i, uint64_t v) const
    {
        switch ( bw)
        {
            case 1 :  reinterpret_cast< Cv_IotaTrailImpl<  Cv_IotaDef::BitWidth_1> *>( iarr)->SetAt( i, v); return;
            case 2 :  reinterpret_cast< Cv_IotaTrailImpl<  Cv_IotaDef::BitWidth_2> *>( iarr)->SetAt( i, v); return;
            case 3 :  reinterpret_cast< Cv_IotaTrailImpl<  Cv_IotaDef::BitWidth_4> *>( iarr)->SetAt( i, v); return;
            case 4 :  reinterpret_cast< Cv_IotaTrailImpl<  Cv_IotaDef::BitWidth_8> *>( iarr)->SetAt( i, v); return;
            case 5 :  reinterpret_cast< Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_16> *>( iarr)->SetAt( i, v); return;
            case 6 :  reinterpret_cast< Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_32> *>( iarr)->SetAt( i, v); return;
            case 7 :  reinterpret_cast< Cv_IotaTrailImpl< Cv_IotaDef::BitWidth_64> *>( iarr)->SetAt( i, v); return;
            default:  break;
        }
        return;
    }

};//_____________________________________________________________________________________________________________________________

class Cv_IotaArray : public Cv_IotaArrayDef
{
protected:
    BitWidth        m_WdIndex; 

public:
    Cv_IotaArray( BitWidth wdIndex = BitWidth_Unknown)
        : m_WdIndex( wdIndex) 
    {}
     
    
    BitWidth            WdIndex( void) const { return m_WdIndex; }

    uint64_t            GetAt( uint8_t  *iarr, uint32_t i) const {  return Cv_IotaArrayDef::GetAt( m_WdIndex, iarr, i); }
    void                SetAt( uint8_t  *iarr, uint32_t i, uint64_t v) const {  Cv_IotaArrayDef::SetAt( m_WdIndex, iarr, i, v); }

};

//_____________________________________________________________________________________________________________________________

class Cv_IotaAggr : public Cv_IotaArray 
{
protected:
    uint32_t    m_MemSz; 
    uint32_t    m_Loc;
    uint8_t     *m_Vec;

public:
    Cv_IotaAggr( std::vector< uint8_t> *pVec, uint32_t aggrSz, uint32_t sz, uint32_t wd) 
        : Cv_IotaArray( BitWidthFromIndex( wd)), m_MemSz(  Cv_IotaArray::MemSize(  sz, BitWidthFromIndex( wd))) 
    {
        m_Loc = uint32_t(  pVec->size());
        pVec->resize(  aggrSz * m_MemSz + m_Loc, 0);
        m_Vec = &pVec->at( m_Loc);
    }

    uint32_t        MemSz( void) const { return m_MemSz; }
    uint32_t        Loc( void) const { return m_Loc; }
    void            SetRowElem( uint32_t row, uint32_t i, uint64_t v) { SetAt(&m_Vec[ row  * m_MemSz], i, v); }
    static void     DumpAggrData( std::ostream &ostr, std::vector< uint8_t> *pVec, const std::string &vName);
    
};

//_____________________________________________________________________________________________________________________________
