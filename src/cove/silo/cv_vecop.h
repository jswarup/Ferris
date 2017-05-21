//______________________________________ cv_vecop.h _________________________________________________________________________
#pragma once

#include    "cove/barn/cv_cexpr.h"

//_____________________________________________________________________________________________________________________________

class Cv_VecOp
{
public:
    template < typename Vecop, typename LambdaFn, typename = void >
    class  Cv_Vecop  
    {
        Vecop     	m_Vec;
        LambdaFn	m_Lambda;

    public:
        typedef LambdaFn Lambda;

        Cv_Vecop( Vecop &&vec, Lambda &&lambda)
            : m_Vec( vec), m_Lambda( lambda)
        {}
		
        uint32_t    Size( void) { return  m_Vec.size(); }

        auto        GetLambda( void) const { return m_Lambda; } 
  
    template < typename... X>
        auto        Reduce( X... args) const
        {
            std::vector< decltype( m_Lambda)>   retVec( m_Vec.size()); 
            for ( uint32_t i = 0; i < retVec.size(); ++i)
            {
                auto    redVec = m_Vec.Reduce( args...);
            }
            return  retVec;
        }
  
    };

    //_____________________________________________________________________________________________________________________________

    template <  typename Vecop, typename LambdaFn>
    class  Cv_Vecop< Vecop, LambdaFn, typename Cv_TypeEngage::Exist< typename Vecop::Lambda>::Note >  
    {
        Vecop     	m_Vec;
        LambdaFn    m_Lambda;

    public:
        typedef LambdaFn    Lambda;

        Cv_Vecop( Vecop &&vec, Lambda &&lambda)
            : m_Vec( std::move( vec)), m_Lambda( [this, lambda]( auto... args) { return lambda( m_Vec.GetLambda( args...)); })
        {}
     
		
        uint32_t    Size( void) { return  m_Vec.Size(); }

        auto        GetLambda( void) const { return m_Lambda; } 

    template < typename... X>
        auto        Reduce( X... args) const
        {
            auto                    redVec = m_Vec.Reduce( args...);
            std::vector< decltype( m_Lambda)>   retVec( m_Vec.Size()); 
            return  retVec;
        }
    };

    //_____________________________________________________________________________________________________________________________

    template <  typename Vecop>
    class  Cv_Vecop< Vecop, void, typename Cv_TypeEngage::Exist< typename Vecop::value_type>::Note>  
    {
        Vecop       m_Vec;

    public: 

        Cv_Vecop( Vecop &&vec)
            : m_Vec( std::move( vec))
        {}
		
		
        int32_t     Size( void) { return uint32_t( m_Vec.size()); }
    
    };

template< typename Vecop >
    static constexpr auto   Make( Vecop &&vec )
    {
	    return Cv_Vecop< Vecop, void, void>( std::forward<Vecop>( vec));
    }

template< typename Vecop, typename LambdaFn>
    static constexpr auto   Make( Vecop &&vec, LambdaFn &&lambda)
    {
	    return Cv_Vecop< Vecop, LambdaFn>( std::forward<Vecop>( vec), std::forward<LambdaFn>( lambda));
    }


};
//_____________________________________________________________________________________________________________________________

