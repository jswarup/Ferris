//_______________________________________________ cv_spritz.cpp _______________________________________________________________

#include	"cove/barn/cv_include.h"
#include	"cove/flux/cv_spritz.h"

#include <sys/types.h>
#include <sys/stat.h>

#ifndef CV_WINDOWS
#include <unistd.h>
#endif

//_____________________________________________________________________________________________________________________________

void    Cv_Spritz::FillNull( uint32_t sz) 
{
    uint8_t     *buf = ( uint8_t *) alloca( sz); 
    memset( buf, 0, sz);
    Write(  buf, sz);
    return;
}
    
//_____________________________________________________________________________________________________________________________

Cv_FileSpritz::Cv_FileSpritz( const Cv_CStr &fname, Facet facet)
    : m_Fp( NULL), m_Facet( facet), m_Offset( 0)
{
    if ( m_Facet == ReadOnly)
        m_Fp = fopen( fname.CStr(), "rb");
    else if ( m_Facet & WriteTrim)
        m_Fp = fopen( fname.CStr(), "w+b");

    struct stat     st;
    fstat( fileno( m_Fp), &st);
    m_Size = st.st_size;
}

//_____________________________________________________________________________________________________________________________

Cv_FileSpritz::~Cv_FileSpritz( void)
{
    if ( m_Fp)
        fclose( m_Fp);
}

//_____________________________________________________________________________________________________________________________
    
void        Cv_FileSpritz::SetOffset( uint32_t k)
{
    if ( k < m_Size)
    {
        m_Offset = k;
        fseek( m_Fp, k, SEEK_SET);
    }
    else
    {
        m_Offset = m_Size;
        fseek( m_Fp, 0L, SEEK_END);
    }
    if (( m_Facet == ReadOnly) || ( k <= m_Size) || ( k == CV_UINT32_MAX))
        return;
    uint8_t     t[ 4096];
    t[ 0] = 1;
    uint32_t    sz = 4096;
    for ( uint32_t rem = k -m_Size, wrSz = 0; rem; rem -=wrSz)
    {
        wrSz = rem > sz ? sz : rem;
        if ( t [ 0])
            memset( t, 0, wrSz);  
        fwrite( t, wrSz, 1, m_Fp);
    }
    m_Offset = m_Size = k;
    return;
}

//_____________________________________________________________________________________________________________________________
    
uint32_t    Cv_FileSpritz::EnsureSize( uint32_t sz)
{
    uint32_t    curOff = m_Offset;
    uint32_t    retOff = m_Size;
    SetOffset( m_Size + sz);
    SetOffset( curOff);
    return retOff;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_FileSpritz::Read( void *buf, uint32_t sz) 
{  
    m_Offset += sz; 
    return  fread( buf, sz, 1, m_Fp) == 1;  
}

//_____________________________________________________________________________________________________________________________

bool    Cv_FileSpritz::Write( const void *buf, uint32_t sz) 
{  
    if ( !( m_Facet & WriteRead))
        return false;
    m_Offset += sz;
    return  fwrite( buf, sz, 1, m_Fp) == 1;  
}

//_____________________________________________________________________________________________________________________________

void  Cv_FileSpritz::FlushAll( void)
{   
    fflush( m_Fp);
    return;
}

//_____________________________________________________________________________________________________________________________

Cv_ValidationSpritz::~Cv_ValidationSpritz( void)
{
}

//_____________________________________________________________________________________________________________________________

bool    Cv_ValidationSpritz::Read( void *buf, uint32_t sz)
{
    uint32_t                offset = m_WorkSpritz->Offset();
    m_TestSpritz->SetOffset( offset);
    bool                    res = m_WorkSpritz->Read( buf, sz);
    std::vector< uint8_t>   bench( sz);
    bool                    res2 = m_TestSpritz->Read( &bench[ 0], sz);
    int                     res3 = memcmp( buf, &bench[ 0], sz);
    CV_ERROR_ASSERT( ( res == res2) && ( res3 == 0))
    return res;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_ValidationSpritz::Write( const void *buf, uint32_t sz)
{
    uint32_t                offset = m_WorkSpritz->Offset();
    m_TestSpritz->SetOffset( offset);
    std::vector< uint8_t>   bench( sz);
    bool                    res2 = m_TestSpritz->Read( &bench[ 0], sz);
    int                     res3 = memcmp( buf, &bench[ 0], sz);
    bool                    res = m_WorkSpritz->Write( buf, sz);
    CV_ERROR_ASSERT( ( res == res2) && ( res3 == 0))
    return res;
}

//_____________________________________________________________________________________________________________________________
