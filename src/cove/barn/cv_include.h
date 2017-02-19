//----------------------------------------- cv_include.h --------------------------------------------------------------------
#pragma once

//-----------------------------------------------------------------------------------------------------------------------------

#include    <assert.h>
#include	<string.h>
#include	<stdarg.h>
#include    <list>
#include    <stack>
#include    <string>
#include    <vector>
#include    <bitset>
#include	<set>
#include    <map>
#include    <algorithm>
#include    <iostream>
#include    <fstream>
#include    <sstream>
#include    <typeinfo>
#include	<memory>
#include    <numeric>
#include	<valarray>
#include    <stdexcept>
#include    <limits>

#include    <atomic>
#include    <thread>

#ifdef _MSC_VER
#define strcasecmp      stricmp
#define strncasecmp     strnicmp
#endif

#define CV_UINT8_MAX        uint8_t( -1)
#define CV_UINT16_MAX       uint16_t( -1)
#define CV_UINT32_MAX       uint32_t( -1)
#define CV_UINT64_MAX       uint64_t( -1)
#define CV_UINT32_SUBMAX    ( uint32_t( -1) -1)
        
#define CV_INT8_MIN         int8_t( 0x80)    
#define CV_INT8_MAX         int8_t( 0x7F)
        
#define CV_INT16_MIN        int16_t( 0x8000)
#define CV_INT16_MAX        int16_t( 0x7FFF) 
        
#define CV_INT32_MIN        int32_t( 0x80000000)
#define CV_INT32_MAX        int32_t( 0x7FFFFFFF)
        
#define CV_INT64_MIN        int64_t( 0x8000000000000000)
#define CV_INT64_MAX        int64_t( 0x7FFFFFFFFFFFFFFF)

#define	CV_LOWMASK( x)	    (( uint64_t (0x1) << ( x)) -1)

//-----------------------------------------------------------------------------------------------------------------------------
