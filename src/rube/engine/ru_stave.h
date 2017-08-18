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
    typedef typename Module::Outlet::PtrTuple   PtrOutput;

    auto  ActionFn( void)
    {   
        return Cv_TupleTools::Make( [this]( auto... rest) { 
            Ru_Stave< Module>       *thisModule = static_cast< Ru_Stave< Module> *>( this); 
            thisModule->m_OpOutput.Apply( thisModule->Action( thisModule->m_Input));
   //         Cv_TupleTools::PtrAssign( thisModule->m_PtrOutput, thisModule->Action( thisModule->m_Input));
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
        auto    subActions = static_cast< SubStaves *>( this)->Unary( []( auto var) { return var.ActionFn(); } ); 
        return Cv_TupleTools::Fuse( modFn, Cv_TupleTools::Melt( subActions)); 
    }
};

//_____________________________________________________________________________________________________________________________

template < typename Module>
struct Ru_Stave :  public Ru_StaveCompound< Module>, public Module
{
    typedef typename Module::Inlet::Tuple       Input;
    typedef typename Module::Outlet::Tuple      Output;
    typedef typename Module::Outlet::PtrTuple   PtrOutput; 

    Input                      m_Input;
    Input                      m_Future;
    //PtrOutput                  m_PtrOutput;
    Cv_OpTuple< Output>        m_OpOutput;
    
public: 
    Ru_Stave( void) 
    {} 

template < int K>
    auto        VarPtr( void)  { return Cv_TupleIndex< Input, Input::Sz -1 -K>( &m_Future).PVar(); } 

template < int K>
    auto        OpPtr( void)  { return Cv_TupleIndex< Cv_OpTuple< Output>, Output::Sz -1 -K>( &m_OpOutput).PVar(); } 
 };
 
 
//_____________________________________________________________________________________________________________________________
