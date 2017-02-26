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
        return Outlet::Tuple( input.Var< 0>() && input.Var< 1>());
    }
};


struct Ru_XorGate
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool>            Outlet;   



    Outlet::Tuple     Action( const Inlet::Tuple  &input)
    {
        return Outlet::Tuple( input.Var< 0>() != input.Var< 1>());
    }
};



struct Ru_HalfAdder 
{
    typedef Ru_Inlet< bool, bool>       Inlet;
    typedef Ru_Outlet< bool, bool>      Outlet;   

    typedef Ru_Compound< Ru_AndGate, Ru_XorGate>   Compound;

 template < class SitePtr >
    static void    Bind( SitePtr site)
    {

        site->InPort< 0>()->Join( site->Child< 0>()->InPort< 0>());
		site->InPort< 0>()->Join( site->Child< 1>()->InPort< 0>());

		site->InPort< 1>()->Join( site->Child< 0>()->InPort< 1>());
		site->InPort< 1>()->Join( site->Child< 1>()->InPort< 1>());

		site->OutPort< 0>()->Join( site->Child< 1>()->OutPort< 0>());
		site->OutPort< 1>()->Join( site->Child< 0>()->OutPort< 0>());       
    }
    
};


/*

class FlHalfAdder : public FlModule  
{
public:
	FlInPort<FlBool>		a;
	FlInPort<FlBool>		b;
	
	FlOutPort<FlBool>		sum;
	FlOutPort<FlBool>		carry;

	FlAndGate				andGate;
	FlXorGate				xorGate;

public:
	//---------------------------------------------------------------------

	FlHalfAdder( const std::string &str = std::string(), FlModule *p = NULL) 
		: FlModule( str, p),
		  andGate(  "and", this),
		  xorGate( "xor", this),
		  a( "in1", this),
		  b( "in2", this),
		  sum( "sum", this),
		  carry( "carry", this)
	{
		a.join( andGate.fetchPort( "in1"));
		a.join( xorGate.fetchPort( "in1"));

		b.join( andGate.fetchPort( "in2"));
		b.join( xorGate.fetchPort( "in2"));

		sum.join( xorGate.fetchPort( "out"));
		carry.join( andGate.fetchPort( "out"));
	}

	//---------------------------------------------------------------------
};
*/