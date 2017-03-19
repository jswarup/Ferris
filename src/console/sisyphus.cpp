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
    auto    c = b.Compose( [=]( auto p) 
        { 
            return [=]( auto... args) 
            { 
                std::cout << p; 
                return true; 
            }; 
        });
        
     auto    d = c.Invoke( 6, 4); 
    

  //  x.Invoke( 1, 2);
 //   Ru_Site< Ru_HalfAdder>      rr( nullptr) ;
    std::cout << "OK";
    return 0;
}

//_____________________________________________________________________________________________________________________________
