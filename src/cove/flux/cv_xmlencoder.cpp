//_______________________________________________ cv_xmlencoder.cpp _______________________________________________________________

#include    "cove/barn/cv_include.h"   
#include    "cove/flux/cv_xmlencoder.h"

//_____________________________________________________________________________________________________________________________

std::string cv_xmlEncode(const Cv_CStr &source)
{
    return source;

	std::stringstream     sstrm;
	for ( uint32_t  i = 0; i < source.Size(); ++i)
    switch ( source[ i])
	{
		case '&': sstrm <<  "&amp;"; break;
		case '<': sstrm << "&lt;"; break;
		case '>': sstrm << "&gt;"; break;
		case '"': sstrm << "&quot;"; break;
		case '\'': sstrm << "&apos;"; break;
		case ' ': sstrm << "&#160;"; break;
        /*
		case 'ß': sstrm << "&#223;"; break;
		case 'ä': sstrm << "&#228;"; break;
		case 'ö': sstrm << "&#246;"; break;
		case 'ü': sstrm << "&#252;"; break;
		case 'Ä': sstrm << "&#196;"; break;
		case 'Ö': sstrm << "&#214;"; break;
		case 'Ü': sstrm << "&#220;"; break;
        */
		default :
            sstrm << source[ i];
	}
	return sstrm.str();
}

//_____________________________________________________________________________________________________________________________

std::string cv_xmlDecode( const Cv_CStr &str)
{
    return          str;
	std::string     dest;
    for ( uint32_t i = 0; i < str.Size(); ++i)
        if ( str[ i] != '&')
			dest += str[ i];
		else 
		{
			if ( strncmp( &str[ i], "&lt;", 4) == 0)
			{
				dest += '<';
				i += 3;
			}
			else if ( strncmp( &str[ i], "&gt;", 4) == 0)
			{
				dest += '>';
				i += 3;
			}
			else if ( strncmp( &str[ i], "&quot;", 6) == 0)
			{
				dest += '"';
				i += 5;
			}
			else if ( strncmp( &str[ i], "&apos;", 6) == 0)
			{
				dest += '\'';
				i += 5;
			}
			else if ( strncmp( &str[ i], "&amp;", 5) == 0)
			{
				dest += '&';
				i += 4;
			}
			else if ( strncmp( &str[ i], "&#160;", 6) == 0)
			{
				dest += ' ';
				i += 5;
			}
            /*
			else if ( strncmp( &str[ i], "&#223;", 6) == 0)
			{
				dest += char( 'ä');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#228;", 6) == 0)
			{
				dest += char( 'ß');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#246;", 6) == 0)
			{
				dest += char( 'ö');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#252;", 6) == 0)
			{
				dest += char( 'ü');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#196;", 6) == 0)
			{
				dest += char( 'Ä');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#214;", 6) == 0)
			{
				dest += char( 'Ö');
				i += 5;
			}
			else if ( strncmp( &str[ i], "&#214;", 6) == 0)
			{
				dest += char( 'Ü');
				i += 5;
			}*/
			else
				dest += str[i];
		}
	return dest;
}

//_____________________________________________________________________________________________________________________________

bool        Cv_XmlEncoder::Accept( const std::string &str)
{
    m_AttribValue = str;
    return !m_AttribValue.empty();
}

//_____________________________________________________________________________________________________________________________

Cv_XmlEncoder::Cv_XmlEncoder( Cv_Spritz *oStrm, const char *v , const char *e , bool s )
{
    m_Spritz.SetStream( oStrm);
}

//_____________________________________________________________________________________________________________________________

Cv_XmlEncoder::~Cv_XmlEncoder(void)
{
}

//_____________________________________________________________________________________________________________________________

bool Cv_XmlEncoder::StartDocument( void)
{

    return true;
}

//_____________________________________________________________________________________________________________________________

bool Cv_XmlEncoder::EndDocument( void)
{
    m_Spritz << '\n';
    return true;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_XmlEncoder::StartElement( Cv_XlemDispensor *xdispensor)
{
    m_XlemStack.push_back( xdispensor->Name());
    
    m_Spritz << '\n';
    
    m_Spritz << "<" << cv_xmlEncode( m_XlemStack.back());
    for ( bool  res = xdispensor->MoveNextAttrib( this); res; res = xdispensor->MoveNextAttrib( this))
	    m_Spritz << ' ' << xdispensor->AttribName() << "=\"" <<  cv_xmlEncode( m_AttribValue) << "\"";
    
    m_HasChildrenStack.push_back( xdispensor->HasChildren());
    if ( !m_HasChildrenStack.back())
    {    
        m_Spritz << "/>";
    }
    else
    {
        m_Spritz << '>';
        m_Spritz.IncrMargin();
    }
    return true;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_XmlEncoder::CData( const Cv_CStr &name)
{
    m_Spritz << '\n';
    m_Spritz <<  "<![CDATA[" << name << "]]>";
    return true;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_XmlEncoder::Text( const Cv_CStr &name)
{
    m_Spritz << '\n';
    m_Spritz << name;
    return true;
}
 
//_____________________________________________________________________________________________________________________________
// record the end of element

bool Cv_XmlEncoder::EndElement( void)
{
    if ( m_HasChildrenStack.back())
    {
        m_Spritz.DecrMargin();
        m_Spritz << '\n';
        m_Spritz << "</" << cv_xmlEncode( m_XlemStack.back()) << '>';	
    }
    m_XlemStack.pop_back();
    m_HasChildrenStack.pop_back();
    return true;
}

//_____________________________________________________________________________________________________________________________


