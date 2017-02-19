//----------------------------------------- ru_gate.h --------------------------------------------------------------------
#pragma once

#include    "rube/engine/ru_rubesite.h"


//-------------------------------------------------------------------------
// A simple and-gate

class Ru_AndGate : public Ru_Site< Ru_Inlet< bool, bool>, Ru_Outlet< bool >>
{ 
    typedef Ru_Site< Ru_Inlet< bool, bool>, Ru_Outlet< bool >>  Base;
public:
	//---------------------------------------------------------------------

	Ru_AndGate( Ru_RubeSite *master, const std::string &str)
		: Base( master, str)
	{
		
	}

	
};
