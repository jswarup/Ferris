//______________________________________ cv_minion.h _________________________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template < class Owner>
class Cv_Minion
{
protected:
	Owner		*m_Owner;

public:
	Cv_Minion( Owner *o = nullptr)
		: m_Owner( o) 
    {}

	Owner		*GetOwner( void) const { return m_Owner; }
	void		SetOwner(  Owner *o)  {  m_Owner = o;  }

};

//_____________________________________________________________________________________________________________________________
