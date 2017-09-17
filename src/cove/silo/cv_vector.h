//______________________________________ cv_vector.h _________________________________________________________________________
#pragma once
 
//_____________________________________________________________________________________________________________________________

template < typename X>
class Cv_Vector ;

struct Cv_VectorTools
{

template < typename X>
    static Cv_Vector< X>  Construct( const X &x)
        {   return Cv_Vector< X>( 1, x);  }
};

template < typename X>
class Cv_Vector : public std::vector< X>, public Cv_VectorTools
{
    typedef std::vector< X>     Base;

public:
    Cv_Vector( const std::vector< X> &vec)
        : Base( vec)
    {}
    
    Cv_Vector( std::vector< X> &&vec)
        : Base( std::move( vec))
    {}

    Cv_Vector( uint32_t sz, const X &x)
        : Base( sz, x)
    {}
 
template < typename Lambda>
    auto    Compose( Lambda param) const
    {
        typedef decltype( param) RetLambda; 
        auto        lwrap = [ param,  this]( auto... rest) { return param( 0, this->at( 0), rest...);};  
        auto        retVals = Construct( lwrap); 
        for ( uint32_t i = 1; i < this->size(); ++i)
            retVals.push_back( [ param, i, this]( auto... rest) { return param( i, this->at( i), rest...);}); 
        return retVals;
    };
     
};

//_____________________________________________________________________________________________________________________________

