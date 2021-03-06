//----------------------------------------- ru_stave.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"
#include    "cove/silo/cv_crate.h"
#include    "cove/silo/cv_traversor.h"
#include	"cove/barn/cv_aid.h"
//#include    "rube/engine/ru_site.h"

//_____________________________________________________________________________________________________________________________

template < typename Module>
struct Ru_Stave;

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct Ru_StaveAction
{

    auto ActionFn( void)
    {
        return Cv_TupleTools::Make( []( auto... rest) { return true; });
    } 
};

template < typename Module>
struct Ru_StaveAction<  Module, typename Cv_TypeEngage::Exist< decltype((( Module *) nullptr)->Action(  *( typename Module::Inlet::Tuple *) nullptr) )>::Note>
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output; 

    auto  ActionFn( void)
    {   
        return Cv_TupleTools::Make( [this]( auto... rest) { 
            Ru_Stave< Module>       *thisModule = static_cast< Ru_Stave< Module> *>( this);  
            Output                  output = thisModule->Action( thisModule->m_Input);
            Cv_TupleTools::PtrVectorAssign( thisModule->m_Listeners, output);
            return true; });
    }
 };

//_____________________________________________________________________________________________________________________________

template < typename Module, typename = void>
struct Ru_StaveCompound : public Ru_StaveAction< Module> 
{
    auto  ActionFn( void)
    {   
        return  this->Ru_StaveAction< Module>::ActionFn(); 
    }
};

template < typename Module>
struct Ru_StaveCompound<  Module, typename Cv_TypeEngage::Exist< typename  Module::Compound>::Note> :  public Ru_StaveAction< Module>, public Module::Compound::SubStaves  
{  
    typedef typename  Module::Compound::SubStaves SubStaves;
    auto  ActionFn( void)
    {   
        auto    modFn = static_cast< Ru_StaveAction< Module> *>( this)->ActionFn(); 
        auto    subActions = static_cast< SubStaves *>( this)->Unary( []( auto &var) { return var.ActionFn(); } ); 
        return Cv_TupleTools::Fuse( modFn, Cv_TupleTools::Melt( subActions)); 
    }
};

 
//_____________________________________________________________________________________________________________________________


template < typename Module>
struct Ru_Stave :  public Ru_StaveCompound< Module>, public Module
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;  

    Input                                       m_Input;
    Input                                       m_Future; 
    Cv_TypeMapTuple< Cv_PtrVector, Output>      m_Listeners;

public: 
    Ru_Stave( void) 
    {} 

};

//_____________________________________________________________________________________________________________________________

template < typename Stave, int K>
struct Ru_StaveTools
{
    static auto        VarPtr( void)  { return []( Stave *stave) {
                                           return  Cv_TupleIndex< typename Stave::Input, Stave::Input::Sz -1 -K>( &stave->m_Future).PVar(); }; }

    static auto        Listener( void)  { return []( Stave *stave) {
                                            return  Cv_TupleIndex< Cv_TypeMapTuple< Cv_PtrVector, typename Stave::Output>, Stave::Output::Sz -1 -K>( &stave->m_Listeners).PVar(); }; }
};
 
 
//_____________________________________________________________________________________________________________________________
