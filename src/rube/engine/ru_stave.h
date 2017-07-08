//----------------------------------------- ru_stave.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "cove/silo/cv_crate.h"
#include    "cove/silo/cv_trotter.h"

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
};

//_____________________________________________________________________________________________________________________________

class  Ru_StaveCrateCnstr :  public Cv_GraphTrotter< Ru_StaveCrateCnstr>
{ 

public:
    Ru_StaveCrateCnstr( void)
    {}
    
template < typename Site>
    Ru_StaveModule< typename Site::Module>  *Proliferate( Site *rr, Ru_StaveCrate *crate)
    {
        Ru_StaveModule< typename Site::Module>    *stave = crate->Create< Ru_StaveModule< typename Site::Module> >();
        return stave;
    }    
    
};

//_____________________________________________________________________________________________________________________________
