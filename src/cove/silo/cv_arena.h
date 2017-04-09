//___________________________________ cv_arena.h _____________________________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include	"cove/barn/cv_minion.h" 
#include	"cove/barn/cv_stdsupport.h"
#include	"cove/barn/cv_shared.h"
#include	"cove/barn/cv_cexpr.h"
#include	"cove/barn/cv_aid.h"
#include	"cove/barn/cv_frager.h"
#include	"cove/silo/cv_slist.h"
#include	"cove/silo/cv_arenatools.h"

//_____________________________________________________________________________________________________________________________
// HeapStall is a heap construct;  Declaration parameter
//      ParentStall : the parent stall refering it.
//      TValueType : Pages hold the objects of of this type
//      SzBits : Number of bits used to address the objects

template < class Arena, class ParentStall, typename TValueType , uint32_t SzBits>
class Cv_HeapStall : public Cv_Shared< Arena::MT>, public Cv_Minion< ParentStall>, public Arena::Janitor,
                     public Cv_PtrIteratorImpl< Cv_HeapStall< Arena, ParentStall, TValueType, SzBits>>
{
public:
    enum
    {
        SzArray = Cv_CExpr::Pow2( SzBits),                          // size of the supported array
        PageSz = sizeof(TValueType) * SzArray,                     	// size of the page
        SzMask = SzBits,                                           	// Size of mask
        Mask = Cv_CExpr::LowMask(SzMask),                       	// bitmask used  used to extract the index in the page.
    };
    
    typedef ParentStall		                Parent;
    typedef TValueType    	                ValueType;
    typedef Cv_HeapStall                    LeafStall; 
    
protected:
    Cv_Atomic< uint16_t, Arena::MT>        m_Info;                 	// 15-bit for offset in Parent and 1-bit for clean status; Max of 32K addresses support 
    uint8_t                                m_Page[ PageSz];

public:
	Cv_HeapStall( Parent *parent, uint16_t pParentlink, const ValueType &iVal = Cv_CExpr::InitVal< ValueType>())
		: Cv_Minion< ParentStall>( parent), m_Info(0)
    {
        Cv_Aid::Set( this->template At< ValueType>( 0), this->template At< ValueType>( SzArray), iVal);     // Initialize the entire array by given iVal
        SetParentOff( pParentlink);                          		// Store the parent offset
        if ( this->GetOwner())                                      // It has a parent then then raise reference so that parent does not get deleted.
            this->GetOwner()->RaiseRef();
    }

    ~Cv_HeapStall( void)
    {}

    uint16_t    ParentOff( void) const { return Cv_Frager< 1, 15>(m_Info.Load()).Get(); }
    void        SetParentOff( uint16_t off) { m_Info.Store(Cv_Frager< 1, 15>(m_Info.Load()).Set(off)); }

    bool        IsClean( void) const { return Cv_Frager< 0, 1>(m_Info.Load()).Get(); }
    void        SetClean( bool t) { m_Info.Store(Cv_Frager< 0, 1>(m_Info.Load()).Set(t)); }
     

template < class X>
    const X     &AccAt(uint32_t i) const { return reinterpret_cast< X*>( &m_Page[0])[i]; }

template < class X>
    X           *PtrAt(uint32_t i) { return &reinterpret_cast< X*>( &m_Page[0])[i]; }

template < class X>
    void        SetAt(uint32_t i, const X &x) { reinterpret_cast< X*>( &m_Page[0])[i] = x; }

    bool    Evict( Arena *arena)    
    {
        if ( this->IsInUse())                                       // No eviction
            return false;
            
        if ( !this->IsClean())                                      // if not clean scrub it by writing to file if Arena is attached to file
            arena->ScrubStall( this);
            
        this->GetOwner()->DetachChild( arena, this);                // Get parent to detach this stall
        
        if ( !this->GetOwner()->LowerRef())                         // lower parent reference and if parent is to evicted add it to backlog
            arena->BackLog()->push_back( [ parent = this->GetOwner()]( Arena *arena) { return  parent->Evict( arena); }); 

        arena->Discard( this);                                      // discard the struct 
        return true;
    }

    auto    Spot( Arena *arena, uint32_t l)
    { 
        return  Cv_Spot< Arena, Cv_HeapStall>( arena, l, this);      // pin an object  in the heap
    }
};

//_____________________________________________________________________________________________________________________________
// BranchStall are special HeapStall that store pointers to HeapStalls or offsets in the file

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

    Cv_BranchStall( Parent *parent, uint16_t pParentlink)
		: BaseStall( parent, pParentlink, nullptr)
	{}

    auto    Spot( Arena *arena, uint32_t l)
    {
        uint16_t        childOff = uint16_t( (l & Mask) >> SubChunk::SzMask);               // we would not be index in 64k pointers ever
        CV_DEBUG_ASSERT( childOff < SzArray)
	    SubChunk        **childLink = this->template PtrAt< SubChunk *>( childOff);	        // top SzBits bits only
        bool            heapFlg = arena->IsOnHeap( *childLink);

	    if ( heapFlg)	                                                                    // The page in memory    	                                    
	        return (*childLink)->Spot( arena, l);
	  
        
        SubChunk        *subChunk =  arena->template Allocate< SubChunk>( static_cast< Derived*>( this), childOff);     // Page is not in memory, allocate memory for the page

        if ( *childLink)                                                                    // page on file
        {
            subChunk->PreserveRef( uint64_t( *childLink));                                  // preserve the file offset in subChunk
            arena->ReloadStall( subChunk);                                                  // load the data from the offset
        }	    
        *childLink = subChunk;		                                                        // change the childLink value 
        return subChunk->Spot(arena, l);
    }

template < typename MemStall>    
    void    DetachChild( Arena *arena, MemStall *stall)                                     // detach a child stall      
    { 
        uint16_t        childOff = stall->ParentOff();
        SubChunk        **childLink = this->template PtrAt< SubChunk *>( childOff);
        CV_ERROR_ASSERT( *childLink == stall)    
        *childLink = reinterpret_cast< SubChunk *>( stall->GrabRef()) ;                     // store the child file reference.         
        return;
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
	
    Cv_ArenaStall( Parent *parent, uint16_t pParentlink)
        : BaseStall( parent, pParentlink)
    {}
};


template < class Arena, class Parent, typename LeafType, uint8_t SzBits>
class Cv_ArenaStall< Arena, Parent, LeafType, SzBits> : public  Cv_HeapStall< Arena, Parent, LeafType, SzBits>
{	
public:
    typedef Cv_HeapStall< Arena, Parent, LeafType, SzBits> 	BaseStall;
	
    Cv_ArenaStall( Parent *parent, uint16_t pParentlink)
        : BaseStall( parent, pParentlink)
    {}
};

//_____________________________________________________________________________________________________________________________ 

template< class Arena, class LeafType, bool MTh, uint8_t... Rest>
class Cv_BaseArena : public Cv_Shared< MTh>
{
public:
    typedef Cv_ArenaStall< Arena, Arena, LeafType, Rest...>     RootStall;
    
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
        : m_TopStall( nullptr)
    {
    }

    bool        Evict( Arena *arena) { return true; }
    void        DetachChild( Arena *arena, void *spot)  {  m_TopStall = nullptr; }

    bool        IsOnHeap( void *s) { return !!s; }


    RootStall   *FetchTopStall( void)
    {
        Arena       *arena = static_cast< Arena*>( this);
        if ( !m_TopStall)
            m_TopStall =  Allocate< RootStall>( arena, 0);
        return m_TopStall;
    }

    auto    Spot( uint32_t k)  { return static_cast< Arena *>( this)->FetchTopStall()->Spot( static_cast< Arena*>( this), k); }


    Cv_CallBacklog< Arena *>         *BackLog( void) { return &m_FnBacklog; }

template < typename Stall, typename... X>
    Stall     *Allocate( X... x)
    {
        return new Stall(x...);
    }

template < typename Stall>
    void     Discard( Stall *obj)
    {
        delete obj;
    }

    class Janitor
    {
    public:
        
        void        PreserveRef( uint64_t off)     {  ; }
        uint64_t    GrabRef( void) const { return 0x0; }
    };

template < typename MemStall>
    void    ReloadStall( MemStall *spot, uint32_t pageSz)
    {
    }

template < typename MemStall>
    void    ScrubStall( MemStall *spot, uint32_t sz)
    {
        return;
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
	typedef typename BaseArena::RootStall                                                   RootStall;

    Cv_FileArena( const char *nm, bool freshFlg)
    {
        m_Fp = fopen( nm, !freshFlg && Cv_Aid::FileExists( nm) ? "r+b" :  "w+b");                   // if the file exists and user intends to use it
    }    

    Cv_FileArena( FILE  *f) 
        :  m_Fp( f)
    {}    
	
    RootStall   *FetchTopStall( void)
    {
        if ( !BaseArena::m_TopStall)
        {
            RootStall   *rootStall =  BaseArena::FetchTopStall();
            rootStall->m_FileOffset.Store( 0);
            fseek( m_Fp, 0L, SEEK_END );
            if ( ftell( m_Fp))
                ReloadStall( rootStall );
            else
                ScrubStall( rootStall);
        }
        return BaseArena::m_TopStall;
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

template < typename MemStall>
    void    ReloadStall( MemStall  *spot)
    {
        fseek( m_Fp, spot->m_FileOffset.Load(), SEEK_SET );
        CV_DEBUG_ASSERT( spot->m_FileOffset.Load() == ftell( m_Fp))
	    size_t    nRd = fread( spot->template PtrAt< uint8_t>( 0), MemStall::PageSz, 1, m_Fp);
        CV_ERROR_ASSERT( nRd == 1)
        return;
    }

    //_____________________________________________________________________________________________________________________________
    // sync up MemStall with file

template < typename MemStall>
    void    ScrubStall( MemStall  *spot)
    {
        if ( spot->m_FileOffset.Load() == CV_UINT32_MAX)                                    // if has not been written to file write the chunk to file
	    {
            int     res = ::fseek( m_Fp, 0L, SEEK_END );                                    // go to the end of file
            CV_ERROR_ASSERT( res == 0)
            spot->m_FileOffset.Store( uint32_t( ::ftell( m_Fp)));                           // store the file-location in the Stall
            size_t    nWr = fwrite(  spot->template PtrAt< uint8_t>( 0), 1, MemStall::PageSz, m_Fp);           // write the chunk to file
            CV_ERROR_ASSERT( nWr == 1)
            return;
        } 

        int         res = fseek( m_Fp, spot->m_FileOffset.Load(), SEEK_SET );               // go to the file-offset 
        CV_ERROR_ASSERT( res == 0)
        size_t      nWr = fwrite( spot->template PtrAt< uint8_t>( 0), MemStall::PageSz, 1,  m_Fp);             // write the page content.
        CV_ERROR_ASSERT( nWr == 1)
        return;
    }
};

//_____________________________________________________________________________________________________________________________
