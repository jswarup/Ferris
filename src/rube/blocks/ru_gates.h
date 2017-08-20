//----------------------------------------- ru_gate.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_site.h"

//_____________________________________________________________________________________________________________________________

struct Ru_AndGate
{
    typedef Ru_Inlet< Ru_AndGate, bool, bool>       Inlet;
    typedef Ru_Outlet< Ru_AndGate, bool>            Outlet;  
     
    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() && input.Var< 1>());
    }
};

//_____________________________________________________________________________________________________________________________

struct Ru_XorGate
{
    typedef Ru_Inlet< Ru_XorGate, bool, bool>       Inlet;
    typedef Ru_Outlet< Ru_XorGate, bool>            Outlet;  
    
    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() != input.Var< 1>());
    }
};

//_____________________________________________________________________________________________________________________________

struct Ru_OrGate
{
    typedef Ru_Inlet< Ru_OrGate, bool, bool>        Inlet;
    typedef Ru_Outlet< Ru_OrGate, bool>             Outlet;  
    
    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() || input.Var< 1>());
    }
};

//_____________________________________________________________________________________________________________________________

struct Ru_HalfAdder  
{
    typedef Ru_Inlet< Ru_HalfAdder, bool, bool>     Inlet;      // a, b
    typedef Ru_Outlet< Ru_HalfAdder, bool, bool>    Outlet;     // sum, carry
    
    typedef Ru_Compound< Ru_AndGate, Ru_XorGate>    Compound;

    struct Site : public Ru_Site< Ru_HalfAdder>
    {
        Site( Ru_Stave< Ru_HalfAdder> *master)
            : Ru_Site< Ru_HalfAdder>( master)
        { 
            Ru_Site< Ru_AndGate>    *xorGate = Child< 0>();
            Ru_Site< Ru_XorGate>    *andGate = Child< 1>();
            
            InPort< 0>()->Join( andGate->InPort< 0>());
		    InPort< 0>()->Join( xorGate->InPort< 0>());

		    InPort< 1>()->Join( andGate->InPort< 1>());
		    InPort< 1>()->Join( xorGate->InPort< 1>());

		    OutPort< 0>()->Join( xorGate->OutPort< 0>());   // sum
		    OutPort< 1>()->Join( andGate->OutPort< 0>());   // carry   
        }
    };

};

//_____________________________________________________________________________________________________________________________

struct Ru_FullAdder  
{
public:
    struct Site;

    typedef Ru_Inlet< Ru_FullAdder, bool, bool, bool>               Inlet;      // carryIn, a, b
    typedef Ru_Outlet< Ru_FullAdder, bool, bool>                    Outlet;     // sum, carry
    typedef Ru_Junction< Ru_FullAdder, bool, bool, bool>            Junction;   // sum2_A

	typedef Ru_Compound< Ru_HalfAdder, Ru_HalfAdder, Ru_OrGate>     Compound;

	struct Site : public Ru_Site< Ru_FullAdder>
    {
        Site(  Ru_Stave< Ru_FullAdder> *master)             
            : Ru_Site< Ru_FullAdder>( master)
        {
            Ru_Site< Ru_HalfAdder>  *halfAdder1 = Child< 0>();
            Ru_Site< Ru_HalfAdder>  *halfAdder2 = Child< 1>();
            Ru_Site< Ru_OrGate>     *orGate =   Child< 2>(); 

            InPort< 0>()->Join( halfAdder1->InPort< 0>());      // carryIn
            InPort< 1>()->Join( halfAdder2->InPort< 0>());      // a
            InPort< 2>()->Join( halfAdder2->InPort< 1>());      // b
    
            OutPort< 0>()->Join( halfAdder1->OutPort< 0>());
            OutPort< 1>()->Join( orGate->OutPort< 0>());
            
            Conn< 0>()->Join( halfAdder2->OutPort< 0>(), halfAdder1->InPort< 1>());     // sum2 from a & b  => input to the final halfAdder 
            Conn< 1>()->Join( halfAdder1->OutPort< 1>(), orGate->InPort< 0>());         // carry1 => orIn1
            Conn< 2>()->Join( halfAdder2->OutPort< 1>(), orGate->InPort< 1>());         // carry2 => orIn1
             
        }
    };

};

//_____________________________________________________________________________________________________________________________
