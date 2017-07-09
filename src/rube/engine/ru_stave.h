//----------------------------------------- ru_stave.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "cove/silo/cv_crate.h"
#include    "cove/silo/cv_traversor.h"
#include	"cove/barn/cv_aid.h"
#include    "rube/engine/ru_site.h"

//_____________________________________________________________________________________________________________________________

class Ru_StaveCrate;

//_____________________________________________________________________________________________________________________________

class Ru_Stave : public Cv_CrateEntry
{
public: 
    Ru_Stave( uint32_t id)
        :   Cv_CrateEntry( id)
    {} 

    void    WriteFuture( uint32_t ind, void *value);
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
class Ru_StaveModule : public Ru_Stave
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;

    Input                                       m_Input;
    Input                                       m_Future;
    
public: 
    Ru_StaveModule( uint32_t id)
        :   Ru_Stave( id)
    {}
 };
 
//_____________________________________________________________________________________________________________________________

class Ru_StaveCrate : public Cv_Crate< Ru_Stave>
{
   
public:
    Ru_StaveCrate( void)
    {}

    Cv_Enumerator< uint32_t>    *StateEnumerator( uint32_t stateId)
    {
        return NULL;
    }
};

//_____________________________________________________________________________________________________________________________

class  Ru_StaveCrateCnstr : public Cv_GraphTraversor< Ru_StaveCrate>
{
    std::vector< Ru_RubeSite *>     m_Sites;

    bool    Visit( uint32_t stateId, Cv_GraphTraversor< Ru_StaveCrate> *traveror, bool entryFlg)
    {
            return true;
    }   

public:
    Ru_StaveCrateCnstr( Ru_StaveCrate *crate)
        : Cv_GraphTraversor< Ru_StaveCrate>( crate)
    {
        m_Sites.push_back( 0);
    }
    
template < typename Site>
    Ru_StaveModule< typename Site::Module>  *Proliferate( Site *rr)
    {
        Ru_StaveModule< typename Site::Module>    *stave = m_Store->Create< Ru_StaveModule< typename Site::Module> >( m_Store->Size());
        Ru_RubeSite         *rs = rr;
        m_Sites.push_back( rs);
        DoDepthTraversal( stave->GetId(), this, &Ru_StaveCrateCnstr::Visit);
        return stave;
    }    
    
};

//_____________________________________________________________________________________________________________________________
