//_______________________________________________ cv_flume.h _______________________________________________________________
#pragma once

#include    "cove/silo/cv_cstr.h"
#include	"cove/barn/cv_aid.h"
#include    <iostream>
#include    <cstdarg>

//_____________________________________________________________________________________________________________________________

class Cv_Spritz;

//_____________________________________________________________________________________________________________________________

class Cv_FlumeKit
{
public:
    class Format
    {
        enum {  BufferSz = 1024 };

        char    m_Buffer[ BufferSz];
        int     m_Size; 

    public:
        Format( void)
            : m_Size( 0)
        {
            m_Buffer[ 0] = '\0';
        }

        Format( const char *fmt, ...)
        {
            va_list     vl;
            va_start( vl, fmt);
            m_Size  = vsnprintf( m_Buffer, BufferSz, fmt, vl);
            CV_ERROR_ASSERT( m_Size)
            va_end( vl);
        }

        operator Cv_CStr( void) { return Cv_CStr( &m_Buffer[ 0], m_Size); }

        operator std::string( void) { return std::string( &m_Buffer[ 0], &m_Buffer[ m_Size]); }

        friend std::ostream     &operator<<( std::ostream  &ostr, const Format &cstr)
        {
            ostr.write( cstr.m_Buffer, cstr.m_Size);
            return ostr;
        }
    };
    struct IncrMargin
    {
        friend std::ostream     &operator<<( std::ostream  &ostr, const IncrMargin &cstr);
    };
    struct DecrMargin
    {
        friend std::ostream     &operator<<( std::ostream  &ostr, const DecrMargin &cstr);
    };
    struct NLFlush
    {
        friend std::ostream     &operator<<( std::ostream  &ostr, const NLFlush &cstr);
    };
};

//_____________________________________________________________________________________________________________________________

class Cv_Flume  : public std::basic_ostream<char>, public Cv_FlumeKit
{
protected:

    enum
    {
        BufSz = 250
    };
    class Stream : public std::streambuf
    {
        Cv_Flume            *m_Flume;
        std::vector< char>  *m_Buffer;

        char                *Begin( void) { return &m_Buffer->at( 0); }
        char                *End( void) { return Begin() +m_Buffer->size(); }

    public:
        Stream( Cv_Flume *flume);
        ~Stream( void);

        void                DoInitialize( void);

        std::streamsize     xsputn( const char *p, std::streamsize n);
        int                 sync( void);
        int                 overflow( int);
    };

    Stream                  m_Strm;

public:
    Cv_Flume( void);

    virtual                 ~Cv_Flume( void);
    
    

    virtual uint32_t        DoPlay( const Cv_CStr &str) = 0;

    void                    Flush( void) { std::basic_ostream<char>::flush(); }
};


//_____________________________________________________________________________________________________________________________

class Cv_OStreamFlume  :  public Cv_Flume
{
protected:
    std::ostream            *m_OStrm;

public:
    typedef  std::ostream  Stream;
    
    Cv_OStreamFlume( void)
        :   m_OStrm( nullptr)
    {}

    bool                IsInitialized( void) const { return !!m_OStrm; }

    virtual void        SetStream( std::ostream *oStrm);
    uint32_t            DoPlay( const Cv_CStr &str);
    void                FlushAll( void) { m_OStrm->flush(); }
};

//_____________________________________________________________________________________________________________________________

class Cv_SpirtzFlume  :  public Cv_Flume
{
protected:
    Cv_Spritz           *m_Spritz;

public:
    typedef  Cv_Spritz  Stream;

    Cv_SpirtzFlume( void)
        :   m_Spritz( nullptr)
    {}

    bool                IsInitialized( void) const { return !!m_Spritz; }

    virtual void        SetStream( Cv_Spritz *oStrm);
    uint32_t            DoPlay( const Cv_CStr &str);
    void                FlushAll( void);
};

//_____________________________________________________________________________________________________________________________

class Cv_MarginMgr
{
protected:
    std::string     *m_Margin;
    std::string     *m_MarginUnit;

public:
    Cv_MarginMgr( void)
        :   m_MarginUnit( nullptr), m_Margin( nullptr)
    {}

    ~Cv_MarginMgr( void);

    void        DoInitialize( void);

    void        SetMargin( const std::string &mUint);

    void        IncrMargin( void);

    void        DecrMargin( void);

};

//_____________________________________________________________________________________________________________________________

template < class Flume>
class Cv_MarginFlume : public Flume, public Cv_MarginMgr
{
    uint32_t        m_Cols;
    uint32_t        m_Lines;
  
public:
    typedef typename Flume::Stream  Stream;

    Cv_MarginFlume( void)
        :   m_Lines( 0), m_Cols( 0)
    {}
    
    ~Cv_MarginFlume( void)
    {
        this->Flush();
    }

    void    SetStream( Stream *oStrm)
    {
        Flume::SetStream( oStrm);
        Cv_MarginMgr::DoInitialize();
    }

    void    IncrMargin( void) 
    { 
        if ( !this->IsInitialized())
            return;

        this->Flush();
        Cv_MarginMgr::IncrMargin();
    }

    void    DecrMargin( void) 
    { 
        if ( !this->IsInitialized())
            return;
        this->Flush(); 
        Cv_MarginMgr::DecrMargin(); 
    }

    uint32_t    DoPlay( const Cv_CStr &str)
    {
        uint32_t    curChars = 0;
        for ( Cv_CStr  cstr = str; cstr.Size(); )
        { 
            Cv_Twin< Cv_CStr>       csplit = Cv_Aid::LSplit( cstr, '\n');
            uint32_t    SzChar = Flume::DoPlay( csplit.First());
            m_Cols += SzChar;
            curChars += SzChar;

            if ( !csplit.Second().Size())
                return curChars;

            curChars += Flume::DoPlay( "\n");
            m_Cols = Flume::DoPlay( *m_Margin);
            curChars += m_Cols;
            ++m_Lines;
            cstr = csplit.Second().Advance( 1);
        }
        return curChars;
    }
};

//_____________________________________________________________________________________________________________________________

typedef Cv_MarginFlume< Cv_OStreamFlume>    Cv_OStreamLog;
typedef Cv_MarginFlume< Cv_SpirtzFlume>     Cv_SpritzLog;

//_____________________________________________________________________________________________________________________________
