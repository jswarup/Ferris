//----------------------------------------- sispyphus.cpp --------------------------------------------------------------------

#include    "cove/barn/cv_include.h"
#include    "rube/barn/ru_include.h"
#include    "rube/blocks/ru_gates.h"

//_____________________________________________________________________________________________________________________________


int main( int argc, char *argv[])
{
    Ru_HalfAdder::Site   rr( nullptr) ;
    std::cout << "OK\n";

    auto    b = Ru_TupleTools::Make( 2, 4);
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
    
    
    return 0;
}

//_____________________________________________________________________________________________________________________________
