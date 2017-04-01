//---------------------------------- cv_arena.h --------------------------------------------------
#pragma once

//_____________________________________________________________________________________________________________________________

#include	"cove/barn/cv_lackey.h" 
#include	"cove/barn/cv_stdsupport.h"
#include	"cove/barn/cv_shared.h"
#include	"cove/barn/cv_cexpr.h"
#include	"cove/barn/cv_aid.h"
#include	"cove/barn/cv_frager.h"
#include	"cove/silo/cv_slist.h"

//_____________________________________________________________________________________________________________________________

template < class Arena, class LeafStall>
class Cv_Pin
{
protected:
     typedef typename LeafStall::ValueType   LeafType;

    Arena                   *m_Arena;
    LeafStall               *m_MemChunk;
    uint32_t                m_Index;

public:
    typedef LeafType                    value_type;
    typedef int                         difference_type;
    typedef LeafType                    *pointer;
    typedef LeafType                    &reference;
    typedef std::input_iterator_tag     iterator_category;

    Cv_Pin( void)
        : m_Arena( nullptr), m_Index( 0), m_MemChunk( nullptr)
    {}
    
    Cv_Pin( Arena *arena, uint32_t index)
        : Cv_Pin( m_Arena->Pin( m_Index))
    {}
    
    Cv_Pin( Arena *arena, uint32_t index, LeafStall *memChunk)
        : m_Arena( arena), m_Index( index), m_MemChunk( memChunk)
    {
        m_MemChunk->RaiseRef();
    }
    
    Cv_Pin( const Cv_Pin &a)
        : m_Arena( a.m_Arena), m_Index( a.m_Index), m_MemChunk( a.m_MemChunk)
    {
        if ( m_MemChunk)
            m_MemChunk->RaiseRef();
    }

    Cv_Pin( Cv_Pin &&a)
        : m_Arena( a.m_Arena), m_Index( a.m_Index), m_MemChunk( a.m_MemChunk)
    {
        m_MemChunk = a.m_MemChunk;
        a.m_MemChunk = nullptr; 
        a.m_Index = 0;
    }

    ~Cv_Pin( void)
    {
        if ( m_MemChunk && !m_MemChunk->LowerRef())
        {
            m_Arena->BackLog()->push_back( [ memChunk = m_MemChunk](Arena *arena) { return  memChunk->Evict( arena); }); 
            m_Arena->BackLog()->Process( m_Arena);
        }      
    }

    Cv_Pin  &operator=( const Cv_Pin &a)
    {
        if ( this == &a)
            return *this;
        this->Cv_Pin::~Cv_Pin();
        return *( ::new (this) Cv_Pin( a));
    }
    
    operator LeafType       *( void)  { m_MemChunk->SetClean( false); return m_MemChunk->template PtrAt< LeafType>( m_Index & LeafStall::Mask); }

    operator const LeafType *( void) const { return m_MemChunk->template PtrAt< LeafType>( m_Index & LeafStall::Mask); }
    
    Cv_Pin  &operator++( void)
    {
        if ( ++m_Index & LeafStall::Mask)
            return *this;
        *this = m_Arena->Pin( m_Index);
        return *this;
    }

    Cv_Pin  &operator+=( int k)
    {
        *this = m_Arena->Pin( m_Index + k);
        return *this;
    }

    friend Cv_Pin  operator+( const Cv_Pin  &p, int k)
    {
        return Cv_Pin( p.m_Arena->Pin( p.m_Index + k));
    }

    friend int  operator-( const Cv_Pin  &p, const Cv_Pin  &q)
    {
        return p.m_Index -q.m_Index;
    }
    friend bool  operator==( const Cv_Pin  &p, const Cv_Pin  &q)
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

    friend void swap(Cv_Pin& lhs, Cv_Pin& rhs);
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
// Declaration of MemStall..It holds pointer to a allocated page, and 16-bit info 
  
template < class Arena>  
class    Cv_MemStall  : public Cv_Shared< Arena::MT>, public Cv_PtrIteratorImpl< Cv_MemStall< Arena>>, public Arena::Janitor
{   
protected:
    Cv_Atomic< uint16_t, Arena::MT>        m_Info;                  // 15-bit for offset in Parent and 1-bit for clean status
    Cv_Atomic< void *, Arena::MT>          m_PagePtr;               // pointer to the allocated page
    
public:
	typedef Cv_Shared< Arena::MT>			BaseShared;
	
    Cv_MemStall( void *pgPtr)
		:  m_Info( 0), m_PagePtr( pgPtr)
	{}
    
    uint16_t    ParentOff( void) const { return Cv_Frager< 1, 15>( m_Info.Load()).Get(); }
    void        SetParentOff( uint16_t off) { m_Info.Store( Cv_Frager< 1, 15>( m_Info.Load()).Set( off)); }
    
    bool        IsClean( void) const { return Cv_Frager< 0, 1>( m_Info.Load()).Get(); }
    void        SetClean( bool t) { m_Info.Store( Cv_Frager< 0, 1>( m_Info.Load()).Set( t)); }

template < class X>
    const X     &AccAt( uint32_t i) const { return static_cast< X*>( m_PagePtr.Load())[ i]; }

template < class X>
    X           *PtrAt( uint32_t i) { return &static_cast< X*>( m_PagePtr.Load())[ i]; }

template < class X>
    void        SetAt( uint32_t i, const X &x) { static_cast< X*>( m_PagePtr.load())[ i] = x; }

    bool        IsProcessed( void) const {  return IsClean() && !BaseShared::IsInUse(); }

    bool        IsShutdown( void) { return false; }
};

//_____________________________________________________________________________________________________________________________


template < class Arena, class ParentStall, typename TValueType , uint32_t SzBits>
class Cv_HeapStall :  public Cv_Lackey< ParentStall>, public Cv_MemStall< Arena>
{
public:
	typedef Cv_MemStall< Arena>		BaseStall;

    typedef ParentStall		Parent;
    typedef TValueType    	ValueType;
    typedef Cv_HeapStall    LeafStall;
    enum 
    { 
        SzArray         = Cv_CExpr::Pow2( SzBits),   
        PageSz          = sizeof( ValueType) * SzArray,
        SzMask          = SzBits,
        Mask            = Cv_CExpr::LowMask( SzMask),
    };
    
	Cv_HeapStall( Arena *arena, Parent *parent, uint16_t pParentlink, const ValueType &iVal = Cv_CExpr::InitVal< ValueType>())
		: Cv_Lackey< ParentStall>( parent), Cv_MemStall< Arena>( arena->template AllocPage< PageSz>())	
    {
        Cv_Aid::Set( this->template At< ValueType>( 0), this->template At< ValueType>( SzArray), iVal);
        BaseStall::SetParentOff( pParentlink);
        if ( this->m_Master)
            this->m_Master->RaiseRef();
    }

    bool    Evict( Arena *arena)    
    {
        if ( this->IsInUse())
            return false;
        
        if ( !this->IsClean() && !arena->ScrubStall( this, PageSz))
            return false;

        if ( !this->m_Master->DetachChild( arena, this))
            return false;

        if ( !this->m_Master->LowerRef())
            arena->BackLog()->push_back( [ parent = this->m_Master](Arena *arena) { return  parent->Evict( arena); }); 
        this->m_Master = nullptr;

        arena->template FreePage< PageSz>( this->m_PagePtr.SpinGrab( nullptr));
        //this->Cv_HeapStall::~Cv_HeapStall();
        arena->template Discard< Cv_HeapStall >( this);
        return true;
    }

    auto    Pin( Arena *arena, uint32_t l)  
    { 
        return  Cv_Pin< Arena, Cv_HeapStall>( arena, l, this); 
    }
};

//_____________________________________________________________________________________________________________________________


template < class Arena, class Parent, uint8_t SzBits, class SubChunk>
class Cv_BranchStall :  public Cv_HeapStall< Arena, Parent, SubChunk *, SzBits>
{
public:
    typedef Cv_HeapStall< Arena, Parent, SubChunk *, SzBits> 	BaseStall;
	typedef typename SubChunk::Parent       					Derived;
    typedef typename SubChunk::LeafStall     					LeafStall; 
	
	enum 
    { 
        SubChunkSz = sizeof( SubChunk), 
        SzMask = SubChunk::SzMask + SzBits,
        SubMask = Cv_CExpr::LowMask( SubChunk::SzMask), 
        Mask = Cv_CExpr::LowMask( SzMask), 
    };

    Cv_BranchStall( Arena *arena, Parent *parent, uint16_t pParentlink)
		: BaseStall( arena, parent, pParentlink, nullptr)
	{}

    auto    Pin( Arena *arena, uint32_t l)
    {
	    uint16_t        childOff = uint16_t( (l & Mask) >> SubChunk::SzMask);               // we would not be index in 64k pointers ever
        CV_DEBUG_ASSERT( childOff < SzArray)
	    SubChunk        **childLink = this->template PtrAt< SubChunk *>( childOff);	                    // top SzBits bits only
        bool            heapFlg = arena->IsOnHeap( *childLink);

	    if ( heapFlg)	                                                                    // The page in memory    	                                    
	        return (*childLink)->Pin( arena, l);
	  
        SubChunk                    *subChunk =  new ( arena->template Allocate< SubChunkSz>()) SubChunk( arena, static_cast< Derived*>( this), childOff);

        if ( *childLink)
        {
            subChunk->PreserveRef( uint64_t( *childLink)); 
            arena->ReloadStall( subChunk, SubChunk::PageSz);
        }	    
        *childLink = subChunk;		                                                        // change the childLink value
        auto    ptr = subChunk->Pin( arena, l);
        return ptr;
    }
    
    bool    DetachChild( Arena *arena, Cv_MemStall< Arena> *spot)
    {
        uint16_t        childOff = spot->ParentOff();
        SubChunk        **childLink = this->template PtrAt< SubChunk *>( childOff);
        CV_ERROR_ASSERT( *childLink == spot)    
        *childLink = reinterpret_cast< SubChunk *>( spot->GrabRef()) ;
       
        return true;
    }
};

//_____________________________________________________________________________________________________________________________


template< class Arena, class Parent, typename LeafType, uint8_t... Rest>
class Cv_ArenaStall;

template < class Arena, class Parent, typename LeafType, uint8_t SzBits, uint8_t... Rest>
class Cv_ArenaStall< Arena, Parent, LeafType, SzBits, Rest...>  : 
	public Cv_BranchStall< Arena, Parent, SzBits, Cv_ArenaStall< Arena, Cv_ArenaStall< Arena, Parent, LeafType, SzBits, Rest...>, LeafType, Rest...> >
{	
public:
	typedef Cv_BranchStall< Arena, Parent, SzBits, Cv_ArenaStall< Arena, Cv_ArenaStall< Arena, Parent, LeafType, SzBits, Rest...>, LeafType, Rest...> >		BaseStall;
	
    Cv_ArenaStall( Arena *arena, Parent *parent, uint16_t pParentlink)
        : BaseStall( arena, parent, pParentlink)
    {}
};

//_____________________________________________________________________________________________________________________________


template < class Arena, class Parent, typename LeafType, uint8_t SzBits>
class Cv_ArenaStall< Arena, Parent, LeafType, SzBits> : public  Cv_HeapStall< Arena, Parent, LeafType, SzBits>
{	
public:
    typedef Cv_HeapStall< Arena, Parent, LeafType, SzBits> 	BaseStall;
	
    Cv_ArenaStall( Arena *arena, Parent *parent, uint16_t pParentlink)
        : BaseStall( arena, parent, pParentlink)
    {}
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


template< class Arena, class LeafType, bool MTh, uint8_t... Rest>
class Cv_BaseArena : public Cv_Shared< MTh>
{
public:
    class RootStall : public Cv_ArenaStall< Arena, Arena, LeafType, Rest...>  
    {
    public:
		typedef Cv_ArenaStall< Arena, Arena, LeafType, Rest...> 	BaseStall;
		
        RootStall( Arena *arena)
            : BaseStall( arena, arena, 0)
        {}
    };

    enum 
    {
        MT = MTh,
    };
    
    constexpr static uint32_t               SzMask( void) { return  RootStall::SzMask; }
    

protected:
    RootStall                                           *m_TopStall;
    Cv_CallBacklog< Arena *>                            m_FnBacklog;
    Cv_ObjectIndexor< Arena>                            m_PageSzIndexor;

public:
    Cv_BaseArena( void)
    {
        m_TopStall = new (Allocate< sizeof( RootStall)>()) RootStall( static_cast< Arena*>( this));
    }

    bool    Evict( Arena *arena) { return true; }
    bool    DetachChild( Arena *arena, Cv_MemStall< Arena> *spot) { return true; }

    bool    IsOnHeap( void *s) { return !!s; }

    auto    Pin( uint32_t k)  { return m_TopStall->Pin( static_cast< Arena*>( this), k); }

    Cv_CallBacklog< Arena *>         *BackLog( void) { return &m_FnBacklog; }

template < uint32_t AllocSz>
    void     *Allocate( void)
    {
        void    *obj = new char[ AllocSz];
        return obj;
    }

template < typename X>
    void     Discard( X *obj)
    {
        delete [] obj;
        return;
    }

template < uint32_t PageSz>
    void    *AllocPage( void)
    {
        uint32_t    m_PgIndex = m_PageSzIndexor.template Index< PageSz>();
	    void    *pg = new char[ PageSz];
        return pg;
    }

template < uint32_t PageSz>
    void    FreePage( void *pg)
    {
        uint32_t    m_PgIndex = m_PageSzIndexor.template Index< PageSz>();

	    delete [] pg;
    }
    class Janitor
    {
    public:
        
        void        PreserveRef( uint64_t off)     {  ; }
        uint64_t    GrabRef( void) const { return 0x0; }
    };

    void    ReloadStall( Cv_MemStall< Arena> *spot, uint32_t pageSz)
    {
    };

    bool    ScrubStall( Cv_MemStall< Arena> *spot, uint32_t sz)
    {
        return true;
    }
};

//_____________________________________________________________________________________________________________________________


template< class LeafType, bool MT, uint8_t... Rest>
class Cv_Arena : public Cv_BaseArena< Cv_Arena< LeafType, MT, Rest...>, LeafType, MT, Rest...>
{
};

//_____________________________________________________________________________________________________________________________


template< class LeafType, bool MT, uint8_t... Rest>
class Cv_FileArena : public Cv_BaseArena< Cv_FileArena< LeafType, MT, Rest...>, LeafType, MT, Rest...>
{
    FILE    *m_Fp;
  
public:
    typedef Cv_BaseArena< Cv_FileArena< LeafType, MT, Rest...>, LeafType, MT, Rest...> 		BaseArena;
	
    Cv_FileArena( const char *nm, bool freshFlg)
    {
        m_Fp = fopen( nm, !freshFlg && Cv_Aid::FileExists( nm) ? "r+b" :  "w+b");                   // if the file exists and user intends to use it
        BaseArena::m_TopStall->m_FileOffset.Store( 0);                                                         // set the file-offset to zero
        fseek( m_Fp, 0L, SEEK_END );
        if ( ftell( m_Fp))                                                                          // file has content
            ReloadStall( BaseArena::m_TopStall, BaseArena::RootStall::PageSz);                                            // reload the root stall.
        else
            ScrubStall( BaseArena::m_TopStall, BaseArena::RootStall::PageSz);
    }    

    Cv_FileArena( FILE  *f) 
        :  m_Fp( f)
    {
        BaseArena::m_TopStall->m_FileOffset.Store( 0);
        fseek( m_Fp, 0L, SEEK_END );
        if ( ftell( m_Fp))
            ReloadStall( BaseArena::m_TopStall, BaseArena::RootStall::PageSz);
        else
            ScrubStall( BaseArena::m_TopStall, BaseArena::RootStall::PageSz);
    }    

    class Janitor
    {
    public:
        Cv_Atomic< uint32_t, MT>        m_FileOffset;
            
        Janitor( void) 
            : m_FileOffset( CV_UINT32_MAX) {}

        void        PreserveRef( uint64_t off)     {  m_FileOffset.Store( !off ? CV_UINT32_MAX : ( off & ~0x1)); }
        uint64_t    GrabRef( void) const { return m_FileOffset.Load() | 0x1; }
    };


    bool    IsOnHeap(  void *s) { return !!s && !( uint64_t( s) & 0x1); }

    //_____________________________________________________________________________________________________________________________


    void    ReloadStall( Cv_MemStall< Cv_FileArena> *spot, uint32_t pageSz)
    {
        fseek( m_Fp, spot->m_FileOffset.Load(), SEEK_SET );
        CV_DEBUG_ASSERT( spot->m_FileOffset.Load() == ftell( m_Fp))
	    size_t    nRd = fread( spot->template PtrAt< uint8_t>( 0), pageSz, 1, m_Fp);
        CV_ERROR_ASSERT( nRd == 1)
        return;
    }

    //_____________________________________________________________________________________________________________________________
    // sync up MemStall with file

    bool    ScrubStall( Cv_MemStall< Cv_FileArena> *spot, uint32_t pageSz)
    {
        if ( spot->m_FileOffset.Load() == CV_UINT32_MAX)                                    // if has not been written to file write the chunk to file
	    {
            int     res = ::fseek( m_Fp, 0L, SEEK_END );                                    // go to the end of file
            CV_ERROR_ASSERT( res == 0)
            spot->m_FileOffset.Store( uint32_t( ::ftell( m_Fp)));                           // store the file-location in the Stall
            size_t    nWr = fwrite(  spot->template PtrAt< uint8_t>( 0), 1, pageSz, m_Fp);           // write the chunk to file
            CV_ERROR_ASSERT( nWr == 1)
            return true;
        } 

        int         res = fseek( m_Fp, spot->m_FileOffset.Load(), SEEK_SET );               // go to the file-offset 
        CV_ERROR_ASSERT( res == 0)
        size_t      nWr = fwrite( spot->template PtrAt< uint8_t>( 0), pageSz, 1,  m_Fp);             // write the page content.
        CV_ERROR_ASSERT( nWr == 1)
        return true;
    }
};

//_____________________________________________________________________________________________________________________________


template < class X>
class Cv_Spot
{
protected: 
    X           *m_Ptr;
    uint32_t    m_Index;
    
public:
    Cv_Spot( X *ptr, uint32_t index)
        :  m_Ptr( ptr), m_Index( index)  
    {}  

    operator X *( void) { return m_Ptr; }

    X           *Ptr( void) const { return m_Ptr; }
    uint32_t    Index( void) const { return m_Index; }
};

//_____________________________________________________________________________________________________________________________


template < class Arena>
class   Cv_ArenaStore
{

    struct  Atom : public Cv_Link< Atom> 
    {
        uint32_t    m_Index;
        
        Atom( uint32_t index)
            : m_Index( index)
        {}
    };

    Cv_LinkStack< Atom>                     m_FreeAtomStack;        // stack of free atoms
    Arena                                   *m_Arena;
    uint32_t                                m_ApprLeafSz;           // Num of leaves appropriasted

    typedef typename Arena::RootStall::LeafStall         LeafStall;
    typedef typename Arena::LeafType                     LeafType;

    enum 
    {
        SzArray     = LeafStall::SzArray,                           // Num objects in leaf
        SzBits      = Arena::RootStall::SzMask,                     // Total bits supported..
        SzTopBits   = SzBits - LeafStall::SzBits,                   // Bits supported by branches
        SzLeaf      = Cv_CExpr::Pow2( SzTopBits),             // Max number of leaves 
    };

    //----------------------------------------------------------------------
    
    void Appropriate( void)
    {
	    LeafStall    *leafStall = m_Arena->Pin( this->m_ApprPageSz << LeafStall::SzBits).Snitch();
        for ( uint32_t i = 0; i < SzArray; ++i)
        {
            LeafType    *spot = leafStall->template PtrAt< LeafType>( SzArray -1 -i);
            Atom        *atom = new ( spot) Atom( ( SzArray -1 -i) | this->m_ApprPageSz);
            m_FreeAtomStack.Push( atom);
        }
        ++this->m_ApprPageSz;

    }

public:
    Cv_ArenaStore( Arena *arena)
        : m_Arena( arena), m_ApprLeafSz( 0)
    {}

template< class X>
    Cv_Spot< X>     Allocate( void) 
    { 
        Atom    *atom =  m_FreeAtomStack.Top() ? m_FreeAtomStack.Pop() : ( Appropriate(), m_FreeAtomStack.Pop());
        return Cv_Spot< X>( reinterpret_cast< X *>( atom), atom->m_Index);
    }

template< class X>
	void            Discard( Cv_Spot< X> *p) 
    { 
        Atom        *atom = reinterpret_cast< Atom*>( p->m_Ptr);
        atom->m_Index = p->Index();
        m_FreeAtomStack.Push( atom); 
        *p = Cv_Spot< X>( nullptr, 0);
        return;
    }
};

//_____________________________________________________________________________________________________________________________
