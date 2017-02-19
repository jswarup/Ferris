//-------------------------------------- cv_lackey.h ----------------------------------------
#pragma once


//-------------------------------------------------------------------------------------------------

template < class Master>
class Cv_Lackey
{
protected:
	Master		*m_Master;

public:
	Cv_Lackey( Master *o = nullptr)
		: m_Master( o) 
    {}

	Master		*GetMaster( void) const { return m_Master; }
	void		SeMaster(  Master *o)  {  m_Master = o;  }

};

//-------------------------------------------------------------------------------------------------
