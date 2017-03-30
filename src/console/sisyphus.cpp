//----------------------------------------- sispyphus.cpp --------------------------------------------------------------------

#include    "cove/barn/cv_include.h"
#include    "rube/barn/ru_include.h"
#include    "rube/blocks/ru_gates.h"
#include    "cove/silo/cv_vecop.h"
#include    "cove/silo/cv_arena.h"

//_____________________________________________________________________________________________________________________________

void TestArena( void)
{
    typedef  Cv_FileArena< uint32_t, false, 4, 5, 6>    Arena;
    
    bool                                    freshFLg = false;
    Arena                                   arena( "test.sge", freshFLg);

    //typedef  Cr_Arena< uint32_t, false, 4, 5, 6>    Arena;
    //Arena                                           arena;

    uint32_t                            szVec = 1 << Arena::SzMask();
    if ( freshFLg)
    {
        auto        pin1 = arena.Pin( 0);
        for ( uint32_t i = 0; i < szVec; ++i, ++pin1)
            *pin1 = i;
    }
    std::vector< uint32_t>      vec( szVec);
    auto                        pin2 = arena.Pin( 0);
    for ( uint32_t i = 0; i < vec.size(); ++i, ++pin2)
        vec[ vec.size() -1 -i] = ( *pin2);
    return;
}

//_____________________________________________________________________________________________________________________________


int main( int argc, char *argv[])
{
    Ru_HalfAdder::Site   rr( nullptr) ;
    std::cout << "OK\n";

    auto    b = Ru_TupleTools::Make( 2, 4, 7);
    auto    lm = []( uint32_t k, auto var, auto... args) 
    { 
        std::cout << var; 
        auto    a = Ru_TupleTools::Make( args...);
        auto    d = Ru_TupleTools::Dump( std::cout, a);
        d.Invoke();
        return true; 
    };   

    auto    c = b.Compose( lm);
    auto    d = c.Invoke( 6, 5);  
    
    std::vector< int>   vec = { 1, 2, 4, 7};

    auto    v1 = Cv_VecOp::Make( vec, []( auto val) { return 3 * val; });
    auto    v2 = Cv_VecOp::Make( v1, []( auto val) { return 7 * val; });
    auto    v3 = Cv_VecOp::Make( v2, []( auto val) { return 11 * val; });

     
    return 0;
}

//_____________________________________________________________________________________________________________________________
