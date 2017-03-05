//----------------------------------------- ru_gate.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_rubesite.h"

//_____________________________________________________________________________________________________________________________

struct Ru_AndGate
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool>            Outlet;   
     
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

 template < class Site  >
    static void    Bind( Site *site)
    {

        site->InPort< 0>()->Join( site->Child< 0>()->InPort< 0>());
		site->InPort< 0>()->Join( site->Child< 1>()->InPort< 0>());

		site->InPort< 1>()->Join( site->Child< 0>()->InPort< 1>());
		site->InPort< 1>()->Join( site->Child< 1>()->InPort< 1>());

		site->OutPort< 0>()->Join( site->Child< 1>()->OutPort< 0>());
		site->OutPort< 1>()->Join( site->Child< 0>()->OutPort< 0>());       
    }
    
};

//_____________________________________________________________________________________________________________________________
