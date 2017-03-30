//_________________________________________________ cv_shared.h ________________________________________________________________
#pragma once

#include	"cove/barn/cv_atomics.h"

//_____________________________________________________________________________________________________________________________
  
template < bool MT>  
class    Cv_Shared   
{   
protected:
    Cv_Atomic< uint16_t, MT>                m_UseCount;		              

public:
    Cv_Shared(  void)
		:  m_UseCount( 0) 
	{}
    
    bool        IsInUse( void) const { return m_UseCount.Load() != 0; } 
    uint16_t    RaiseRef( void) { return m_UseCount.Incr(); }
    uint16_t    LowerRef( void) { CV_ERROR_ASSERT( m_UseCount.Load()) return m_UseCount.Decr(); }
};

//_____________________________________________________________________________________________________________________________
