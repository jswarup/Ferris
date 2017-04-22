//_______________________________________________ cv_recordlistener.h _______________________________________________________________
#pragma once

#include    <typeinfo>
#include    "cove/silo/cv_slist.h"
#include    "cove/silo/cv_cstr.h"
#include    "cove/barn/cv_aid.h"

//_____________________________________________________________________________________________________________________________

struct Cv_AttribValueReceptor
{
    virtual bool    Accept( const std::string &str) = 0;
};

//_____________________________________________________________________________________________________________________________

class Cv_XlemDispensor
{
public:
    virtual const Cv_CStr           &Name( void) = 0;
    virtual bool                    MoveNextAttrib( Cv_AttribValueReceptor *) = 0;
    virtual const Cv_CStr           &AttribName( void) = 0;
    virtual bool                    HasChildren( void) = 0;
};

//_____________________________________________________________________________________________________________________________

class Cv_XDocListener
{
public:
	virtual bool    StartElement( Cv_XlemDispensor *attribs) = 0;
	virtual bool    CData( const Cv_CStr &name) = 0;
	virtual bool    Text( const Cv_CStr &name) = 0;
	virtual bool    EndElement( void)  = 0;
};

//_____________________________________________________________________________________________________________________________


