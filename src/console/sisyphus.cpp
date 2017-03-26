//----------------------------------------- sispyphus.cpp --------------------------------------------------------------------

#include    "cove/barn/cv_include.h"
#include    "rube/barn/ru_include.h"
#include    "rube/blocks/ru_gates.h"

//_____________________________________________________________________________________________________________________________


int main( int argc, char *argv[])
{
    auto    b = Ru_TupleTools::Make( 2);
    auto    lm = []( uint32_t k, auto var, auto... args) 
    { 
        std::cout << var; 
        /*
        auto    a = Ru_TupleTools::Make( args...);
        
        auto    d = Ru_TupleTools::Dump( std::cout, a);
        d.Invoke();
        */return true; 
    };   

    auto    c = b.Compose( lm);
    auto    d = c.Invoke( 6, 5);  
    
    Ru_HalfAdder::Site   rr( nullptr) ;
    std::cout << "OK";
    return 0;
}

//_____________________________________________________________________________________________________________________________
