//_______________________________________________ cv_xmlencoder.h _______________________________________________________________
#pragma once

#include    "cove/flux/cv_xdoclistener.h"
#include    "cove/flux/cv_flume.h"

//_____________________________________________________________________________________________________________________________

class Cv_XmlEncoder : public Cv_XDocListener, public Cv_AttribValueReceptor
{
protected:
    std::vector< std::string>   m_XlemStack;
    std::vector< bool>          m_HasChildrenStack;
	Cv_SpritzLog                m_Spritz;
    std::string                 m_AttribValue;
    
    bool        Accept( const std::string &str);

public:
	//_____________________________________________________________________________________________________________________________

	Cv_XmlEncoder( Cv_Spritz *oStrm, const char *v = "1.0", const char *e = "UTF-8", bool s = true);
	~Cv_XmlEncoder( void);

    bool        StartDocument( void);
	bool        EndDocument( void);

    bool        StartElement( Cv_XlemDispensor *xdispensor);
	bool        CData( const Cv_CStr &name);
	bool        Text( const Cv_CStr &name);
	bool        EndElement( void);
};


//_____________________________________________________________________________________________________________________________

