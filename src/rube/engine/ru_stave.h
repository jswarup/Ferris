//----------------------------------------- ru_stave.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "cove/silo/cv_crate.h"
#include    "cove/silo/cv_traversor.h"
#include	"cove/barn/cv_aid.h"
//#include    "rube/engine/ru_site.h"

//_____________________________________________________________________________________________________________________________

class Ru_StaveCrate; 
//_____________________________________________________________________________________________________________________________

class Ru_Stave : public Cv_CrateEntry
{
public: 
    Ru_Stave( uint32_t id)
        :   Cv_CrateEntry( id)
    {} 

};

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct Ru_StaveModuleAction
{

    auto ActionFn( void)
    {
        return  NULL;
    } 
};

template < typename Module>
struct Ru_StaveModuleAction<  Module, typename Cv_TypeEngage::Exist< decltype((( Module *) nullptr)->Action(  *( typename Module::Input *) nullptr) )>::Note>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;
    typedef typename Module::Outlet::PtrTuple   PtrOutput;

    auto  ActionFn( void)
    {   
        return [this]( void) { 
            Output  output = this->Module::Action( m_Input); 
            Cv_TupleTools::PtrAssign( m_PtrOutput, output);
            return; }
    }
 };

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct Ru_StaveModuleCompound : public Ru_StaveModuleAction< Module> 
{
    auto  ActionFn( void)
    {   
        return [this]( void) { 
            auto    modFn = this->Ru_StaveModuleAction< Module>::ActionFn();
            return; };
    }
};

template < typename Module>
struct Ru_StaveModuleCompound<  Module, typename Cv_TypeEngage::Exist< typename  Module::Compound>::Note> : public Ru_StaveModuleAction< Module>, public Module::Compound::SubStaves  
{ 
    auto  ActionFn( void)
    {   
        return [this]( void) { 
            auto    modFn = this->Ru_StaveModuleAction< Module>::ActionFn(); 
            return; };
    }
 
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
class Ru_StaveModule : public Ru_Stave, public Ru_StaveModuleCompound< Module>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;
    typedef typename Module::Outlet::PtrTuple   PtrOutput;

    Input                                       m_Input;
    Input                                       m_Future;
    PtrOutput                                   m_PtrOutput;
    
public: 
    Ru_StaveModule( uint32_t id = 0)
        :   Ru_Stave( id)
    {} 
 };
 
//_____________________________________________________________________________________________________________________________
/*
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
    
template < typename Site>
    Ru_StaveModule< typename Site::Module>  *Construct( Site *rr)
    {
        Ru_StaveModule< typename Site::Module>    *stave = m_Store->Create< Ru_StaveModule< typename Site::Module> >( m_Store->Size()); 
        m_Sites.push_back( rr);
        return stave;
    }

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
        Ru_StaveModule< typename Site::Module>    *stave = Construct( rr);  
//        DoDepthTraversal( stave->GetId(), this, &Ru_StaveCrateCnstr::Visit);
        return stave;
    }    
    
};
*/

//_____________________________________________________________________________________________________________________________
