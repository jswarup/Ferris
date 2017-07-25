//----------------------------------------- ru_stave.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "cove/silo/cv_crate.h"
#include    "cove/silo/cv_traversor.h"
#include	"cove/barn/cv_aid.h"
//#include    "rube/engine/ru_site.h"

//_____________________________________________________________________________________________________________________________

struct Ru_StaveCrate; 
template < typename Module>
struct Ru_StaveModule;

//_____________________________________________________________________________________________________________________________

struct Ru_Stave : public Cv_CrateEntry
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
        return Cv_TupleTools::Make( []( auto... rest) { return true; });
    } 
};

template < typename Module>
struct Ru_StaveModuleAction<  Module, typename Cv_TypeEngage::Exist< decltype((( Module *) nullptr)->Action(  *( typename Module::Inlet::Tuple *) nullptr) )>::Note>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;
    typedef typename Module::Outlet::PtrTuple   PtrOutput;

    auto  ActionFn( void)
    {   
        return Cv_TupleTools::Make( [this]( auto... rest) { 
            Ru_StaveModule< Module>     *thisModule = static_cast< Ru_StaveModule< Module> *>( this); 
            Cv_TupleTools::PtrAssign( thisModule->m_PtrOutput, thisModule->Action( thisModule->m_Input));
            return true; });
    }
 };

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct Ru_StaveModuleCompound : public Ru_StaveModuleAction< Module> 
{
    auto  ActionFn( void)
    {   
        return  this->Ru_StaveModuleAction< Module>::ActionFn(); 
    }
};

template < typename Module>
struct Ru_StaveModuleCompound<  Module, typename Cv_TypeEngage::Exist< typename  Module::Compound>::Note> :  public Ru_StaveModuleAction< Module>, public Module::Compound::SubStaves  
{  
    typedef typename  Module::Compound::SubStaves SubStaves;
    auto  ActionFn( void)
    {   
        auto    modFn = static_cast< Ru_StaveModuleAction< Module> *>( this)->ActionFn(); 
        auto    subActions = static_cast< SubStaves *>( this)->Unary( []( auto var) { return var.ActionFn(); } ); 
        return Cv_TupleTools::Fuse( modFn, Cv_TupleTools::Melt( subActions)); 
    }
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
struct Ru_StaveModule : public Ru_Stave, public Ru_StaveModuleCompound< Module>, public Module
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
 
struct Ru_StaveCrate
{
   
public:
    Ru_StaveCrate( void)
    {}
    
    bool    ElemTravesor( Ru_Stave *elem, Cv_TreeTraversor< Ru_Stave*> *context, bool entryFlg, void *)
    {
            return true;
    } 
    
    
template < typename Site>
    Ru_StaveModule< typename Site::Module>      Proliferate( Site *rr)
    {
        Ru_StaveModule< typename Site::Module>      stave; 
        Cv_TreeTraversor< Ru_Stave *>                 staveTraversor;      
        staveTraversor.DoDepthTraversal( &stave, this,  &Ru_StaveCrate::ElemTravesor, ( void *) nullptr); 
        return stave;
    }    
};
 
//_____________________________________________________________________________________________________________________________
