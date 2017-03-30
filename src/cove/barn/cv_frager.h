//_______________________________________________ cv_frager.h _______________________________________________________________
#pragma once


//_____________________________________________________________________________________________________________________________

template < typename R, typename T = uint32_t, typename... Rest>
constexpr R cv_BitMask( T t, Rest... rest) { return cv_BitMask< R>( t) | cv_BitMask< R>( rest...); }

template < typename R, typename T = uint32_t>
constexpr R cv_BitMask( T t) { return R( R( 0x1) << t); }

//_____________________________________________________________________________________________________________________________

template <uint32_t Pos, uint32_t Len>
class Cv_FragerImpl
{
protected:
	uint64_t 	m_Value;

	static const uint64_t Mask = (( uint64_t( 1) << Len) -1);
	static const uint64_t Blank = ~(Mask  << Pos);
		
public:
	Cv_FragerImpl( uint64_t pTarget)
		: m_Value( pTarget)
	{}
	
	operator uint64_t( void) const { return m_Value; }
	operator uint32_t( void) const { return uint32_t( m_Value); }       // @ ur risk
    operator uint16_t( void) const { return uint16_t( m_Value); }       // @ ur risk
    operator uint8_t( void) const { return uint8_t( m_Value); }         // @ ur risk

	Cv_FragerImpl &Set( uint64_t val)
	{
		m_Value &= Blank;
		m_Value |= ( uint64_t( val) & Mask) << Pos;
		return *this;
	}
    
    Cv_FragerImpl &BitOr( uint64_t val)
	{
		m_Value |= ( uint64_t( val) & Mask) << Pos;
		return *this;
	}

	uint32_t Get( void) const
	{
		return uint32_t( ( m_Value >> Pos) & Mask);
	}
    
    uint64_t Get64( void) const
	{
		return ( ( m_Value >> Pos) & Mask);
	}
};

//_____________________________________________________________________________________________________________________________

template <uint32_t Pos, uint32_t Len>
class Cv_Frager : public Cv_FragerImpl< Pos, Len>
{
public:
	Cv_Frager( uint64_t pTarget)
		: Cv_FragerImpl< Pos, Len>( pTarget)
	{}
};

//_____________________________________________________________________________________________________________________________
// specialization to handle single bit fragments as bool

template <uint32_t Pos>
class Cv_Frager< Pos, 1> : public Cv_FragerImpl< Pos, 1>
{
public:
	Cv_Frager( uint64_t pTarget)
		: Cv_FragerImpl< Pos, 1>( pTarget)
	{}
	
	bool Get( void) const
	{
		return bool( ( Cv_FragerImpl< Pos, 1>::m_Value >> Pos) & Cv_FragerImpl< Pos, 1>::Mask);
	}
};

//_____________________________________________________________________________________________________________________________

