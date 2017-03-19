//----------------------------------------- sispyphus.cpp --------------------------------------------------------------------

#include    "cove/barn/cv_include.h"
#include    "rube/barn/ru_include.h"
#include    "rube/blocks/ru_gates.h"

//_____________________________________________________________________________________________________________________________


int main( int argc, char *argv[])
{
    auto    vl = [] (auto... param)
    {
        return Ru_TupleTools::Make( param...);
    };
    /*
    auto    a = vl( 2, 2.5, "arfe", 'a');


    auto    lm = [] (auto p)
    {
        return [=]( auto... args) { std::cout << p; return true; };
    };
    auto    q = lm( 1);
    auto    t = q( 1,2,3);

    auto    b = Ru_TupleTools::Make( 2, 5);
    //auto    c = b.Compose( lm);
    auto x = a.Compose( lm);
    */
    auto    b = Ru_TupleTools::Make( 2, 5, 7);

    auto    lm = []( uint32_t k, auto var, auto... args) 
    { 
        std::cout << var; 
        auto    a = Ru_TupleTools::Make( args...);
        auto    d = Ru_TupleTools::Dump( std::cout, a);
        d.Invoke();
        return true; 
    };  

    auto    c = b.Compose( lm);
    auto    d = c.Invoke( 6, 4); 
    

  //  x.Invoke( 1, 2);
 //   Ru_Site< Ru_HalfAdder>      rr( nullptr) ;
    std::cout << "OK";
    return 0;
}

//_____________________________________________________________________________________________________________________________
