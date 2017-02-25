//----------------------------------------- ru_gate.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_rubesite.h"

//-------------------------------------------------------------------------

struct Ru_AndGate
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool>            Outlet;   



    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        bool b = input.Var< 0>() && input.Var< 1>();
        return Outlet::Tuple( b);
    }
};

