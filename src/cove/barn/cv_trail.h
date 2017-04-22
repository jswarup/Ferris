//_______________________________________________ cv_trail.h _____________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include	"cove/flux/cv_flume.h"
#include    "cove/silo/cv_slist.h"

//_____________________________________________________________________________________________________________________________

class  Cv_Trail : public Cv_SpritzLog
{
public:
    //_____________________________________________________________________________________________________________________________

    class  Log;
    class  Pass;
    class  Module;
    class  Gate;

    //_____________________________________________________________________________________________________________________________

    class Module : public Cv_Link< Module>
    {
        Cv_CStr         m_Name;
        bool            m_ActiveFlg;

    public:
        Module( const Cv_CStr &cstr, bool activeFlg);
        
        Cv_CStr     Name( void) const { return  m_Name; }
        bool        IsActive( void) { return m_ActiveFlg; }

    };

    template < class X>
    struct  Base : public Cv_FlumeKit
    {
        static Module     *TrailModule( void)
        {
            static Module  s_Module( X::ModuleName(), X::IsActive());
            return &s_Module;
        }
    };

    //_____________________________________________________________________________________________________________________________

    class Gate : public Cv_Link< Gate>
    {
        Cv_CStr         m_Name;
        Module          *m_Module;
        uint32_t        m_Count;

    public:
        Gate( Module *m, const Cv_CStr &cstr)
            :  m_Name( cstr), m_Module( m)
        {}
  
        const Cv_CStr   &Name( void) const { return m_Name; }
        uint32_t        Count( void) const { return m_Count; }

        bool            IsActive( void) const { return m_Module->IsActive(); }

        bool            IsOpen( Log *logger) const { return true; }
    
        bool            NoteEntry( Pass *entry);
        void            NoteExit( Pass *entry); 
    };

    //_____________________________________________________________________________________________________________________________


    class Pass : public Cv_Link< Pass>
    {
        Gate    *m_TrailGate;
        bool    m_Active;

    public:
        Pass( Gate *a) 
        :   m_TrailGate( a)
        {
            m_Active = m_TrailGate->NoteEntry( this);
        }

        ~Pass( void)
        {
            if ( m_Active)
                m_TrailGate->NoteExit( this);
        }

        bool    IsActive( void) const { return m_Active; }
        Gate    *TrailGate( void) const { return m_TrailGate; }
    };

protected:
    //_____________________________________________________________________________________________________________________________

    uint32_t                    m_Level;
    Cv_LinkStack< Module >      m_Modules;
    bool                        m_Innermost;
    uint32_t                    m_TrailId;


    Cv_Trail( void)
        : m_Level( 0), m_Innermost( false), m_TrailId( uint32_t( -1))
    {}

public:

    //_____________________________________________________________________________________________________________________________
    
    static Cv_Trail     *Singleton( void);

    void                PushModule( Module *m) { m_Modules.Push( m); }
    void                RaiseIndent( const Cv_CStr &tpName, uint32_t count);
    void                LowerIndent( void);
};

//_____________________________________________________________________________________________________________________________

class Cv_CoveBase : public Cv_Trail::Base< Cv_CoveBase>
{
public:
    static const char   *ModuleName( void) { return "Cove"; }
    static bool         IsActive( void) { return true; }
};

//_____________________________________________________________________________________________________________________________

#define CV_FNPASS( Y)                                                                                                           \
    static Cv_Trail::Gate   _gate( TrailModule(), CV_FUNCNAME());                                                                \
    Cv_Trail::Pass          _pass( &_gate);                                                                                     \
    if ( _pass.IsActive())                                                                                                      \
        *Cv_Trail::Singleton  Y;                                                                                                \
                                                                                                                                \
//_____________________________________________________________________________________________________________________________

#define CV_FNLOG( Y)                                                                                                            \
    if ( _pass.IsActive())                                                                                                      \
        *Cv_Trail::Singleton Y;                                                                                                 \

//_____________________________________________________________________________________________________________________________

