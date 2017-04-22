//_______________________________________________ cv_flume.cpp _______________________________________________________________

#include	"cove/barn/cv_include.h"
#include	"cove/flux/cv_flume.h"
#include	"cove/flux/cv_spritz.h"
#include	"cove/barn/cv_aid.h"
#include    <iomanip>

//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream  &ostr, const Cv_FlumeKit::IncrMargin &cstr)
{
    Cv_OStreamLog   *osLog = dynamic_cast< Cv_OStreamLog *>( &ostr);
    if ( osLog)
    {
        osLog->IncrMargin();
        return ostr;
    }
    Cv_SpritzLog   *spLog = dynamic_cast< Cv_SpritzLog *>( &ostr);
    if ( spLog)
    {
        spLog->IncrMargin();
        return ostr;
    }

    return ostr;
}

//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream  &ostr, const Cv_FlumeKit::DecrMargin &cstr)
{
    Cv_OStreamLog   *osLog = dynamic_cast< Cv_OStreamLog *>( &ostr);
    if ( osLog)
    {
        osLog->DecrMargin();
        return ostr;
    }
    Cv_SpritzLog   *spLog = dynamic_cast< Cv_SpritzLog *>( &ostr);
    if ( spLog)
    {
        spLog->DecrMargin();
        return ostr;
    }
    return ostr;
}

//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream  &ostr, const Cv_FlumeKit::NLFlush &cstr)
{
    ostr << '\n';
    ostr.flush();
    return ostr;
}

//_____________________________________________________________________________________________________________________________

Cv_Flume::Stream::Stream( Cv_Flume *flume)
    :   m_Flume( flume), m_Buffer( nullptr) 
{
}

//_____________________________________________________________________________________________________________________________

void    Cv_Flume::Stream::DoInitialize( void)
{
    if ( !m_Buffer)
        m_Buffer = new std::vector< char>( 256);
    setp( Begin(), End());
    return;
}

//_____________________________________________________________________________________________________________________________

Cv_Flume::Stream::~Stream()
{
    if ( m_Buffer)
    {
        if ( pptr() != Begin())
            sync();
        delete m_Buffer;
    }
}

//_____________________________________________________________________________________________________________________________

std::streamsize Cv_Flume::Stream::xsputn( const char* p, std::streamsize n)
{
    if ( !m_Buffer)
        return 0;

    int     overflow = int( n) - int(epptr() - pptr());
    if ( overflow > 0)
    {
        int             offset = int( pptr() - pbase());
        m_Buffer->resize( size_t( m_Buffer->size() + overflow));
        setp( Begin(), End());
        pbump( offset);
    }

    std::copy( p, p + n, pptr());
    pbump( int( n));

    return n;
}

//_____________________________________________________________________________________________________________________________

int     Cv_Flume::Stream::sync()
{
    if ( !m_Buffer)
       return 0; 
    int     n = int( pptr() - pbase());
    pbump( -n);

    m_Flume->DoPlay( Cv_CStr( Begin(), n));
    return n;
}

//_____________________________________________________________________________________________________________________________

int     Cv_Flume::Stream::overflow( int i)
{
    if ( !m_Buffer)
        return 0;
    int         offset = int( pptr() - pbase());
    m_Buffer->push_back( i);
    setp( Begin(), End());
    pbump( offset + 1);

    return i;
}

//_____________________________________________________________________________________________________________________________

Cv_Flume::Cv_Flume( void)
    :   std::basic_ostream<char>( &m_Strm), m_Strm( this)
{}

//_____________________________________________________________________________________________________________________________

Cv_Flume::~Cv_Flume()
{
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_OStreamFlume::DoPlay( const Cv_CStr &str)
{
    if ( !m_OStrm)
        return 0;
    m_OStrm->write( &str[ 0], str.Size());
    m_OStrm->flush();
    return str.Size();
}

//_____________________________________________________________________________________________________________________________

void    Cv_OStreamFlume::SetStream( std::ostream *oStrm) 
{ 
    m_Strm.DoInitialize();
    m_OStrm = oStrm; 
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_SpirtzFlume::DoPlay( const Cv_CStr &str)
{
    if ( !m_Spritz)
        return 0;
    m_Spritz->Write( &str[ 0], str.Size());
    return str.Size();
}

//_____________________________________________________________________________________________________________________________

void    Cv_SpirtzFlume::SetStream( Cv_Spritz *oStrm) 
{ 
    m_Strm.DoInitialize();
    m_Spritz = oStrm; 
}

//_____________________________________________________________________________________________________________________________

void    Cv_SpirtzFlume::FlushAll( void) 
{ 
    m_Spritz->FlushAll(); 
    return;
}

//_____________________________________________________________________________________________________________________________

Cv_MarginMgr::~Cv_MarginMgr( void)
{
    if ( m_MarginUnit)
        delete m_MarginUnit;
    if ( m_Margin)
        delete m_Margin;
}

//_____________________________________________________________________________________________________________________________

void    Cv_MarginMgr::SetMargin( const std::string &mUint) 
{ 
    if ( !m_MarginUnit)
        m_MarginUnit = new std::string();
    *m_MarginUnit = mUint; 
    return;
}

//_____________________________________________________________________________________________________________________________

void  Cv_MarginMgr::DoInitialize( void)
{
    m_Margin = new std::string();
    if ( !m_MarginUnit)
        m_MarginUnit = new std::string( "    ");
    return;
}

//_____________________________________________________________________________________________________________________________

void  Cv_MarginMgr::IncrMargin( void) 
{ 
    *m_Margin += *m_MarginUnit; 
    return;
}

//_____________________________________________________________________________________________________________________________

void    Cv_MarginMgr::DecrMargin( void) 
{ 
    m_Margin->resize( m_Margin->size() -m_MarginUnit->size()); 
}

//_____________________________________________________________________________________________________________________________
