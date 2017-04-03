//----------------------------------------- ru_rubereactor.h --------------------------------------------------------------------
#pragma once

#include    "cove/silo/cv_tuple.h"

//_____________________________________________________________________________________________________________________________


template < typename T >
class   Ru_Slot 
{
    uint32_t        m_Id;

public:
    Ru_Slot( void) 
        : m_Id( CV_UINT32_MAX)
    {}

};

//_____________________________________________________________________________________________________________________________


template < typename... T>
class   Ru_Ingress : public Cv_Tuple< Ru_Slot< T>... >
{

public:
    typedef Cv_Tuple< Ru_Slot< T>...>  	Base;

    Ru_Ingress( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); } 
};


//_____________________________________________________________________________________________________________________________


template < typename... T>
class   Ru_Outgress : public Cv_Tuple< Ru_Slot< T>... >
{
    
public:
	typedef Cv_Tuple< Ru_Slot< T>...> 	Base;
	
    Ru_Outgress( void)
    {}

    template < int K>
    auto        Port( void) { return Cv_TupleIndex< Base, K>( this).PVar(); } 
};

//_____________________________________________________________________________________________________________________________


class Ru_Block
{
   
public:
    Ru_Block( void);
};

//_____________________________________________________________________________________________________________________________



