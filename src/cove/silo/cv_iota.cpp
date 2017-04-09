//_______________________________________________ cv_iota.cpp _______________________________________________________________

#include	"cove/barn/cv_include.h"
#include	"cove/silo/cv_iota.h"

//_____________________________________________________________________________________________________________________________

Cv_IotaDef::BitWidth     Cv_IotaDef::BitWidthForValue( uint64_t value)
{
    uint32_t    bw = 0;
    for (; value; ++bw)
        value >>= 1; 
    
    if ( bw <= 1)                
        return BitWidth_1;    
    if ( bw <= 2)                
        return BitWidth_2;    
    if ( bw <= 4) 
        return BitWidth_4;
    if ( bw <= 8)
        return BitWidth_8;
    if ( bw <= 16)
        return BitWidth_16;
    if ( bw <= 32)
        return BitWidth_32;
    return BitWidth_64;
} 

//_____________________________________________________________________________________________________________________________
    
uint32_t    Cv_IotaDef::BitSize( BitWidth wd)
{
    static uint32_t     s_BitSize[] = { 0, 1, 2, 4, 8, 16, 32, 64 };
    return s_BitSize[ wd];
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_IotaDef::MaxBitVecSize( uint32_t imgSz)
{
    if ( imgSz == 1)
    {
        //AX_ERROR_ASSERT( false)
        return 1;
    }
    uint32_t    bw = MaxBitWidth( imgSz -1);
    if (( 64 % bw) == 0)                    // 1, 2, 4, 8, 16, 32, 64
        return bw;    
    if ( bw < 4) 
        return 4;
    if ( bw < 8)
        return 8;
    if ( bw < 16)
        return 16;
    if ( bw < 32)
        return 32;
    return 64;
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_IotaDef::MemSize( uint32_t sz, BitWidth wdIndex) 
{
    switch ( wdIndex)
    {
        case BitWidth_1  : return Cv_IotaTrailImpl< BitWidth_1 >::MemSize( sz);
        case BitWidth_2  : return Cv_IotaTrailImpl< BitWidth_2 >::MemSize( sz);
        case BitWidth_4  : return Cv_IotaTrailImpl< BitWidth_4 >::MemSize( sz);
        case BitWidth_8  : return Cv_IotaTrailImpl< BitWidth_8 >::MemSize( sz);
        case BitWidth_16 : return Cv_IotaTrailImpl< BitWidth_16>::MemSize( sz);
        case BitWidth_32 : return Cv_IotaTrailImpl< BitWidth_32>::MemSize( sz);
        case BitWidth_64 : return Cv_IotaTrailImpl< BitWidth_64>::MemSize( sz);
        default:  break;
    }
    return 0;
}

//_____________________________________________________________________________________________________________________________

void    Cv_IotaAggr::DumpAggrData( std::ostream &ostr, std::vector< uint8_t> *pVec, const std::string &vName)
{
    ostr << "uint8_t     " << vName << "[ " << pVec->size() << "] = { ";
    for ( uint32_t i = 0; i <pVec->size(); ++i)
        ostr << pVec->at( i) << (( ( i+1) != pVec->size()) ?  ", " :  "};\n");
    return;
}

//_____________________________________________________________________________________________________________________________
