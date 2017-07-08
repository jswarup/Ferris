//____________________________________________ cv_arenatools.h ________________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template < class Arena, class LeafStall>
class Cv_Spot
{
protected:
     typedef typename LeafStall::ValueType   LeafType;

    Arena                   *m_Arena;
    uint32_t                m_Index;
    LeafStall               *m_MemChunk;
    
public:
    typedef LeafType                    value_type;
    typedef int                         difference_type;
    typedef LeafType                    *pointer;
    typedef LeafType                    &reference;
    typedef std::input_iterator_tag     iterator_category;

    Cv_Spot( void)
        : m_Arena( nullptr), m_Index( 0), m_MemChunk( nullptr)
    {}
    
    Cv_Spot( Arena *arena, uint32_t index)
        : Cv_Spot( m_Arena->Spot( index))
    {}
    
    Cv_Spot( Arena *arena, uint32_t index, LeafStall *memChunk)
        : m_Arena( arena), m_Index( index), m_MemChunk( memChunk)
    {
        m_MemChunk->RaiseRef();
    }
    
    Cv_Spot( const Cv_Spot &a)
        : m_Arena( a.m_Arena), m_Index( a.m_Index), m_MemChunk( a.m_MemChunk)
    {
        if ( m_MemChunk)
            m_MemChunk->RaiseRef();
    }

    Cv_Spot(Cv_Spot &&a)
        : m_Arena( a.m_Arena), m_Index( a.m_Index), m_MemChunk( a.m_MemChunk)
    {
        m_MemChunk = a.m_MemChunk;
        a.m_MemChunk = nullptr; 
        a.m_Index = 0;
    }

    ~Cv_Spot( void)
    {
        if ( m_MemChunk && !m_MemChunk->LowerRef())
        {
            m_Arena->BackLog()->push_back( [ memChunk = m_MemChunk](Arena *arena) { return  memChunk->Evict( arena); }); 
            m_Arena->BackLog()->Process( m_Arena);
        }      
    }

    Cv_Spot  &operator=( const Cv_Spot &a)
    {
        if ( this == &a)
            return *this;
        this->Cv_Spot::~Cv_Spot();
        return *( ::new (this) Cv_Spot( a));
    }
    
    operator LeafType       *( void)  { m_MemChunk->SetClean( false); return m_MemChunk->template PtrAt< LeafType>( m_Index & LeafStall::Mask); }

    operator const LeafType *( void) const { return m_MemChunk->template PtrAt< LeafType>( m_Index & LeafStall::Mask); }
    
    Cv_Spot  &operator++( void)
    {
        if ( ++m_Index & LeafStall::Mask)
            return *this;
        *this = m_Arena->Spot( m_Index);
        return *this;
    }

    Cv_Spot  &operator+=( int k)
    {
        *this = m_Arena->Spot( m_Index + k);
        return *this;
    }

    friend Cv_Spot  operator+( const Cv_Spot  &p, int k)
    {
        return Cv_Spot( p.m_Arena->Spot( p.m_Index + k));
    }

    friend int  operator-( const Cv_Spot  &p, const Cv_Spot  &q)
    {
        return p.m_Index -q.m_Index;
    }
    friend bool  operator==( const Cv_Spot  &p, const Cv_Spot  &q)
    {
        return p.m_Index ==q.m_Index;
    }
    LeafStall    *Snitch( void)
    {
        LeafStall    *memChunk = m_MemChunk;
        m_MemChunk = nullptr; 
        m_Index = 0;
        return memChunk;
    }

    friend void swap(Cv_Spot& lhs, Cv_Spot& rhs);
};

//_____________________________________________________________________________________________________________________________


template < class Arg>
class Cv_CallBacklog : public std::vector< std::function< bool ( Arg arg)>>
{
	bool        m_ProcessingFlg;

public:
    typedef std::vector< std::function< bool ( Arg arg)>> 	Base;
	
	Cv_CallBacklog( void)
        : m_ProcessingFlg( false)
    {}

    void    Process( Arg arg)
    {
        if ( m_ProcessingFlg)
            return;
        m_ProcessingFlg = true;
        std::vector< std::function< bool ( Arg arg)>>   duds;
        while ( Base::size())
        {
            auto    curr = Base::back();
            Base::pop_back();
            bool    res = curr( arg);
            if ( !res)
               duds.push_back( curr); 
        }
        Base::swap( duds);
        m_ProcessingFlg = false;
        return;
    }
};

//_____________________________________________________________________________________________________________________________

template < class Label>
class Cv_ObjectIndexor : public std::vector< uint32_t>
{
    struct  IndexUpdate
    {
        uint32_t    iVal;

        IndexUpdate( Cv_ObjectIndexor *indexor, uint32_t x)
            : iVal( uint32_t( indexor->size()))
        {
            indexor->push_back( x);
        }
         
        operator uint32_t( void) { return  iVal; }
    };

public:

template < uint32_t Key>
    uint32_t Index( void)
    {
        static IndexUpdate    s_Index( this, Key);
        return s_Index;
    }

    
};

//_____________________________________________________________________________________________________________________________
