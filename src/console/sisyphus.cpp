//----------------------------------------- sispyphus.cpp --------------------------------------------------------------------

#include    "cove/barn/cv_include.h"
#include    "rube/barn/ru_include.h"
#include    "rube/blocks/ru_gates.h"
#include    "cove/silo/cv_vecop.h"
#include    "cove/silo/cv_arena.h"
#include    "cove/silo/cv_vector.h"
#include    "cove/silo/cv_stadium.h"
#include    "cove/net/cv_graphnode.h"
#include	"cove/flux/cv_spritz.h"

//_____________________________________________________________________________________________________________________________

void TestArena( void)
{
    typedef  Cv_FileArena< uint32_t, false, 4, 5, 6>    Arena;
    uint32_t                            szVec = 1 << Arena::SzMask();
    
    {
        Cv_FileSpritz   fp( "test.sge", Cv_FileSpritz::WriteTrim);                   // if the file exists and user intends to use it
        {
            Arena        arena( &fp, 0); 
            auto         pin1 = arena.Spot( 0);
            for ( uint32_t i = 0; i < szVec; ++i, ++pin1)
                *pin1 = i;
           
        }
    }
    {
        Cv_FileSpritz   fp( "test.sge", Cv_FileSpritz::ReadOnly); 
        {
            Arena        arena( &fp, 0);
            std::vector< uint32_t>      vec( szVec);
            auto                        pin2 = arena.Spot( 0);
            for ( uint32_t i = 0; i < vec.size(); ++i, ++pin2)
                vec[ vec.size() -1 -i] = ( *pin2);
        } 
    }
    return;
}

//_____________________________________________________________________________________________________________________________

void    TestAdder( void)
{
    Ru_HalfAdder::Site   rr( nullptr) ;
}

//_____________________________________________________________________________________________________________________________

void    TestVecOp( void)
{
    std::cout << "OK\n";

    auto    lm = []( uint32_t k, auto var, auto... args) 
    { 
        std::cout << var; 
        auto    a = Cv_TupleTools::Make( args...);
        auto    d = Cv_TupleTools::Dump( std::cout, a);
        d.Invoke();
        return true; 
    };   

    auto    b = Cv_TupleTools::Make( 2, 4, 7);
    auto    c = b.Compose( lm);
    auto    d = c.Invoke( 6, 5);  

    std::vector< int>   vec = { 1, 2, 4, 7};
    Cv_Vector< int>     cvec( vec);
 
    auto                test = Cv_VectorTools::Construct( lm);

 /*   cvec.Compose( lm);


    auto    v1 = Cv_VecOp::Make( vec, []( auto val) { return 3 * val; });
    auto    v2 = Cv_VecOp::Make( v1, []( auto val) { return 7 * val; });
    auto    v3 = Cv_VecOp::Make( v2, []( auto val) { return 11 * val; });
 */
    return;
} 

//_____________________________________________________________________________________________________________________________

void    TestExtent( void)
{   
    Cv_Extent< uint32_t>        extent;
    std::vector< uint32_t>      sizes = { 6, 4, 8, 12, 6, 4, 8, 24, 18, 6 };
   // auto      ptrs = v1.Reduce();
    return;
}

//_____________________________________________________________________________________________________________________________

int main( int argc, char *argv[])
{
    TestArena();
    TestAdder();
    TestExtent();
    TestVecOp();
    return 0;
}

//_____________________________________________________________________________________________________________________________
