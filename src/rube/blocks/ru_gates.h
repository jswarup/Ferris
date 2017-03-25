//----------------------------------------- ru_gate.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_rubesite.h"

//_____________________________________________________________________________________________________________________________

struct Ru_AndGate
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool>            Outlet;   
    typedef Ru_Site< Ru_AndGate>        Site;
     
    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() && input.Var< 1>());
    }
};

//_____________________________________________________________________________________________________________________________

struct Ru_XorGate
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool>            Outlet;   
    typedef Ru_Site< Ru_XorGate>        Site;
    
    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() != input.Var< 1>());
    }
};

//_____________________________________________________________________________________________________________________________

struct Ru_HalfAdder 
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool, bool>      Outlet;   

    typedef Ru_Compound< Ru_AndGate, Ru_XorGate>   Compound;

    struct Site : public Ru_Site< Ru_HalfAdder>
    {
        Site( Ru_RubeSite *master)
            : Ru_Site< Ru_HalfAdder>( master)
        {

            InPort< 0>()->Join( Child< 0>()->InPort< 0>());
		    InPort< 0>()->Join( Child< 1>()->InPort< 0>());

		    InPort< 1>()->Join( Child< 0>()->InPort< 1>());
		    InPort< 1>()->Join( Child< 1>()->InPort< 1>());

		    OutPort< 0>()->Join( Child< 1>()->OutPort< 0>());
		    OutPort< 1>()->Join( Child< 0>()->OutPort< 0>());       
        }
    };

};

//_____________________________________________________________________________________________________________________________
