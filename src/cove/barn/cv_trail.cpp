//_______________________________________________ cv_trail.cpp _______________________________________________________________

#include	"cove/barn/cv_include.h"
#include	"cove/barn/cv_trail.h"

//_____________________________________________________________________________________________________________________________

Cv_Trail::Module::Module( const Cv_CStr &cstr, bool activeFlg)
    :   m_Name( cstr), m_ActiveFlg( activeFlg)
{
   Cv_Trail     *trail = Cv_Trail::Singleton(); 
   trail->PushModule( this);
}

//_____________________________________________________________________________________________________________________________

Cv_Trail     *Cv_Trail::Singleton( void)
{
    static Cv_Trail     s_Trail;
    return &s_Trail;
}

//_____________________________________________________________________________________________________________________________

void    Cv_Trail::RaiseIndent( const Cv_CStr &tpName, uint32_t count)
{
    ++m_Level;
    if  (m_Level == 1) 
    {
        SELF << "TrailId-" << ++m_TrailId << " {\n";
        IncrMargin();
    }
    SELF << tpName << " #" <<   count << "  {";
    IncrMargin();
    m_Innermost = true;
}

//_____________________________________________________________________________________________________________________________

void    Cv_Trail::LowerIndent( void)
{
    if ( !m_Innermost)
        SELF << '\n';
    DecrMargin();
    SELF << '}';
    if  (m_Level == 1) 
    {
        DecrMargin();
        SELF << '\n' << '}';
    }
    --m_Level;
    m_Innermost = false;
}

//_____________________________________________________________________________________________________________________________
    
bool    Cv_Trail::Gate::NoteEntry( Pass *entry)
{
    m_Count += 1;
    if ( !IsActive())
        return false;
    *Cv_Trail::Singleton() << '\n';
    Cv_Trail::Singleton()->RaiseIndent( entry->TrailGate()->Name(), m_Count);
    Cv_Trail::Singleton()->Flush();
    return true;
}

//_____________________________________________________________________________________________________________________________

void    Cv_Trail::Gate::NoteExit( Pass *entry)
{
    Cv_Trail::Singleton()->LowerIndent();
    Cv_Trail::Singleton()->Flush();
    return;
}

//_____________________________________________________________________________________________________________________________

