//_______________________________________________ cv_extent.h _______________________________________________________________
#pragma once  

//_____________________________________________________________________________________________________________________________

template < class X>
class Cv_Range
{
protected:
	X		m_Begin;	// inclusive
	X		m_End;		// exclusive

public:
	Cv_Range( const X &begin, const X &end)
		: m_Begin( begin), m_End( end) 
	{}


	Cv_Range( const Cv_Range &ex)
		: m_Begin( ex.m_Begin), m_End( ex.m_End) 
	{}

	
	const X		&Begin( void) const { return m_Begin; }
	const X		&End( void) const { return m_End; }

    X           Length( void) const { return m_End - m_Begin; }

	bool 		IsInside( const X &x) const { return ( m_Begin <= x) && ( x < m_End); }     // is value within the range

    int32_t     Compare( const Cv_Range &a) const
                {  return ( m_Begin == a.m_Begin) ? (( m_End == a.m_End) ? 0 : (( m_End < a.m_End) ? -1 : 1)) : (( m_Begin < a.m_Begin) ? -1 : 1); }
	
    friend std::ostream &operator<<( std::ostream &os, const Cv_Range< X> &range)           // dump the Range
	{
		os << "[ " << range.Begin() << " : " << range.End() << "]";
		return os;
	}
};

//_____________________________________________________________________________________________________________________________
// Extent store the subsets of the one-dimensional integer line.

template < class X>
class Cv_Extent  
{
    std::vector< X>         m_Knots;
    std::vector< bool>      m_AllocBits;
    
public: 

	Cv_Extent( void)
    {
        m_Knots.push_back( X( 0));                                                  // [ 0, ) is free
        m_AllocBits.push_back( false);
    }   
    
    X   Allocate( uint32_t sz)
    {
        for ( uint32_t i = 0, k = 1; i < m_AllocBits.size(); ++i, ++k)              // i => interval, k = i +1 
        {
            if ( m_AllocBits[ i])                                                   // [ K[i], ] is is not free, continue
                continue;   

            if (( k == m_AllocBits.size()) || ( sz < ( m_Knots[ k] -m_Knots[ i])))  // if we are at the last interval, it is by default free and  infinite.
            {                                                                       // or the gap for the free is big enough. 
                CV_ERROR_ASSERT( i == 0 || m_AllocBits[ i -1])                      // assert either it is first interval or pervious one was not free.    
                X       x = m_Knots[ i];
                m_Knots[ i] += sz;
                return x;
            }
            if ( ( m_Knots[ k] -m_Knots[ i]) < sz)
                continue;

            CV_ERROR_ASSERT( m_AllocBits[ k])                                      // the next interval is occupied
            X       x = m_Knots[ i];
            m_Knots.erase(  m_Knots.begin() +i);
            m_AllocBits.erase(  m_AllocBits.begin() +i);
            return x;
        }
        return X( -1);
    }

    X   Deallocate( X x, uint32_t sz)
    {
        typename std::vector< X>::iterator   it = std::lower_bound( m_Knots.begin(), m_Knots.end(), x);
        CV_ERROR_ASSERT( it != m_Knots.end());
        if ( *it == x)
        {
            auto    i =  it -m_Knots.begin();
            CV_ERROR_ASSERT(  m_AllocBits[ i])  
                
        }
    }
};

//_____________________________________________________________________________________________________________________________
 

