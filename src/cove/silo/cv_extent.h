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
	//---------------------------------------------------------------------------------------------

	Cv_Range( const X &begin, const X &end)
		: m_Begin( begin), m_End( end) 
	{}

	//_____________________________________________________________________________________________________________________________


	Cv_Range( const Cv_Range &ex)
		: m_Begin( ex.m_Begin), m_End( ex.m_End) 
	{}

	//_____________________________________________________________________________________________________________________________

	
	const X		&Begin( void) const { return m_Begin; }
	const X		&End( void) const { return m_End; }

    X           Length( void) const { return m_End - m_Begin; }

	//_____________________________________________________________________________________________________________________________


	bool 		IsInside( const X &x) const { return ( m_Begin <= x) && ( x < m_End); }

	//_____________________________________________________________________________________________________________________________


	bool operator==( const Cv_Range< X> &ex) const { return ( m_Begin == ex.m_Begin) && ( m_End == ex.m_End); }

	//_____________________________________________________________________________________________________________________________

	
	friend std::ostream &operator<<( std::ostream &os, const Cv_Range< X> &range)
	{
		os << "[ " << range.Begin() << " : " << range.End() << "]";
		return os;
	}

    //_____________________________________________________________________________________________________________________________

	
	struct Cmp
    {
	    bool operator()( const Cv_Range< X> &ex1, const Cv_Range< X> &ex2) const 
        { 
            return ex1.Begin() < ex2.Begin(); 
        }
    };
    
    //_____________________________________________________________________________________________________________________________

};

//_____________________________________________________________________________________________________________________________


template < class X>
class Cv_Extent : public std::vector< Cv_Range< X> >
{
public:
	typedef typename std::vector< Cv_Range< X> >          			Base;
	typedef typename std::vector< Cv_Range< X> >::iterator          Iterator;
	typedef typename std::vector< Cv_Range< X> >::const_iterator	CIterator;

    struct CmpExtent
    {
	    bool operator()( const Cv_Range< X> &ex1, const Cv_Range< X> &ex2) const 
        { 
            return ( ex1.Begin() < ex2.Begin()) || !(ex1.Begin() > ex2.Begin()) ||  ( ex1.End() < ex2.End()); 
        }
    };
    
    //_____________________________________________________________________________________________________________________________


	Iterator		Find( X x, bool *pFound, Iterator f)
	{
		*pFound = false;
		
		Cv_Range< X>	range( x, x +1);
		Iterator		p = std::lower_bound( f, Base::end(), range, CmpExtent());

		if ( p == Base::end()) 						// not found
		{
			if (( p != Base::begin()) && ( *pFound = ( x < ( p -1)->End())))	// test predecessor
				return ( p -1);
			return p;							// *pFound = false;
		}
		if ( (*pFound = ( p->Begin() == x)))		// x matches begin
			return p;							// *pFound = true;
		
		// case where begin > x 
		if ( p == Base::begin())						// we do not have anything before
			return p;
		if ( !!( *pFound = ( x  < ( p -1)->End())))	// begin of Prev < x : check if it contains x
			return p -1;						// *pFound = true;
		return p;
	}

	Iterator		Find( X x, bool *pFound) { return Find( x, pFound, Base::begin()); }

	//_____________________________________________________________________________________________________________________________

	
	Iterator	Insert( const Cv_Range< X> &range)
	{
		bool			fFound = false;
		Iterator		fIt = Find( range.Begin(), &fFound, Base::begin());
		
		if ( fIt == Base::end())						// insert if not found..
			return Base::insert( fIt, range);			// fFound is false anyway 

		if ( !fFound && fIt != Base::begin() &&		// move fIt to predecessor if range sticks to its End.
			( fFound = (( fIt -1)->End() == range.Begin())))
			--fIt;

		bool			lFound = false;
		Iterator		lIt = Find( range.End() -1, &lFound, fIt);

		if ( !fFound && !lFound && ( fIt == lIt))	// range is an isolated entry
			return Base::insert( fIt, range);

		// update the first and remove the rest
		*fIt = Cv_Range< X>( fFound ? fIt->Begin() : range.Begin(), lFound ? lIt->End() : range.End());
		if ( lFound)
			++lIt;
		return Base::erase( ++fIt, lIt);			
	}

	//_____________________________________________________________________________________________________________________________

	
	friend std::ostream &operator<<( std::ostream &os, const Cv_Extent< X> &ex)
	{
		for ( CIterator it = ex.begin(); it != ex.end(); ++it)
			os << *it << ",";
		os << std::endl;
		return os;
	}
};

//_____________________________________________________________________________________________________________________________
 

