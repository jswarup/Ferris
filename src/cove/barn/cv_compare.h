//_______________________________________________ cv_compare.h _______________________________________________________________
#pragma once

#include    <stdint.h>

//_____________________________________________________________________________________________________________________________

template < typename Deriv>
struct Cv_TCmpBase 
{

    
    int32_t  Compare( int32_t a,  int32_t b)     const
    { 
        return  a != b ? ( a < b ? -1: 1) : 0;
    }

    int32_t  Compare( uint32_t a,  uint32_t b)  const
    { 
        return  a != b ? ( a < b ? -1: 1) : 0;
    }
    
    int32_t  Compare( uint64_t a,  uint64_t b)   const
    { 
        return  a != b ? ( a < b ? -1: 1) : 0;
    }

template < typename X>
    int32_t  Compare( X *a, X *b)           const
    { 
        return ( !a || !b) ?  ( a < b ? -1: 1) : static_cast< const Deriv *>( this)->Compare( *a, *b);
    }

template < typename X>
    int32_t  Compare( const X *a, const X *b)     const
    { 
        return ( !a || !b) ?  ( a < b ? -1: 1) : static_cast< const Deriv *>( this)->Compare( *a, *b);
    }

    int32_t  Compare( const void *a, const void *b)   const
    { 
        return ( a != b) ?  ( a < b ? -1: 1) : 0;
    }
template < typename X>
    int32_t  Compare( const X  &a, const X &b)         const
    {  
        return  static_cast< const  Deriv *>( this)->ElemCompare( a, b); 
    }

template < typename X>
    int32_t  ElemCompare( const X  &a, const X &b)    const
    { 
        return  a != b ? ( a < b ? -1: 1) : 0;
    }

template < typename X>
    int32_t  Compare( const std::vector< X> &a, const std::vector< X> &b)   const
    { 
        if ( a.size() != b.size())
            return a.size() < b.size() ? -1 : 1;
        for ( uint32_t i = 0; i < a.size(); ++i)
        {
            int32_t     res = static_cast< const Deriv *>( this)->Compare( a[ i], b[ i]);
            if ( res != 0)
                return res;
        }
        return 0; 
    }

template < typename X>
    int32_t  Compare( const std::vector< X *> &a, const std::vector< X *> &b)   const
    { 
        if ( a.size() != b.size())
            return a.size() < b.size() ? -1 : 1;
        for ( uint32_t i = 0; i < a.size(); ++i)
        {
            if ( a[ i] == b[ i])
                continue;

            if ( !a[ i] || !b[ i])                //  either NULL
                return a[ i] < b[ i] ? -1 : 1;

            int32_t     res = static_cast< const Deriv *>( this)->Compare( *a[ i], *b[ i]);
            if ( res != 0)
                return res;
        }
        return 0; 
    }
};

//_____________________________________________________________________________________________________________________________

struct Cv_Cmp  : public Cv_TCmpBase< Cv_Cmp > 
{

};

//_____________________________________________________________________________________________________________________________

template < typename Select>
struct Cv_TCmp : public Cv_TCmpBase< Cv_TCmp< Select> > 
{
    Select         m_Slct;

template < typename X>
    int32_t  ElemCompare( const X  &a, const X &b) const  {  return a.Compare( m_Slct, b);}

    Cv_TCmp( const Select &y1) 
        : m_Slct( y1) 
    {}
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_TCmp< void> : public Cv_TCmpBase< Cv_TCmp< void> >
{
template < typename X>
    int32_t  ElemCompare( const X  &a, const X &b) const {  return  a.Compare( b); }

    Cv_TCmp( void) 
    {}
};

//_____________________________________________________________________________________________________________________________

template < typename Select>
struct Cv_TLess : public Cv_TCmp< Select> 
{
    
    Cv_TLess( const Select &y1) 
        : Cv_TCmp< Select>( y1) 
    {}

template < typename X>
    bool    operator()( const X &a, const X &b) const { return Compare( a, b) < 0; }
};

//_____________________________________________________________________________________________________________________________

template <>
struct Cv_TLess< void> : public Cv_TCmp< void> 
{

template < typename X>
    bool    operator()( const X &a, const X &b) const { return Compare( a, b) < 0; }
};

//_____________________________________________________________________________________________________________________________
   