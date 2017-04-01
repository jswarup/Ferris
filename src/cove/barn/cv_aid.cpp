//___________________________________________ cv_aid.cpp _____________________________________________________________ 

#include	"cove/barn/cv_include.h"
#include 	"cove/barn/cv_aid.h"

//_____________________________________________________________________________________________________________________________
 
bool Cv_Aid::FileExists(const char *fileName)
{
	if ( !fileName)
		return false;

	FILE    *fp = fopen( fileName, "r");
	if (!fp)
		return false;
	fclose( fp);
	return true;
}

//_____________________________________________________________________________________________________________________________

uint32_t Cv_Aid::BinCoeff(uint32_t n, uint32_t k)
{
    uint32_t    c = 1;
    if ( k > ( n - k))
        k = n - k;

    for ( uint32_t i = 1; i <= k; i++, n--)
    {
        if ( c / i > CV_UINT32_MAX / n) 
            return 0;                       // 0 on overflow 
        c = c / i * n + c%i * n / i;            //split c*n/i into (c/i*i + c%i)*n/i 
    }
    return c;
}


//_____________________________________________________________________________________________________________________________


