//___________________________________ cv_stadium.h _____________________________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include    "cove/silo/cv_arena.h"
#include    "cove/silo/cv_extent.h"

//_____________________________________________________________________________________________________________________________ 

template< class Stad, uint32_t Index>
class Cv_StadRootStall 
{ 
};

template< class Stad>
class Cv_StadRootStall< Stad, 0>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 1>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 2>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};
template< class Stad>
class Cv_StadRootStall< Stad, 3>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 4>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 5>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 6>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 7>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 8>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};

template< class Stad>
class Cv_StadRootStall< Stad, 9>
{ 
    typedef Cv_MemStall< Stad, Stad, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::BranchBits, Stad::LeafBits>     RootStall;
};
 

//_____________________________________________________________________________________________________________________________ 

template < typename Leaf>
class Cv_StadArena
{
public:
   enum {
        MT = false,
        LeafBits = 3,
        BranchBits = 8
    };
    typedef Leaf    LeafType; 
};


/*
class Cv_Stadia 
{
public:
    enum {
        MT = false
    }; 

    typedef Cv_MemStall< Cv_Stadia, Cv_Stadia, Rest...>     RootStall;
    
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


class Cv_Stadia 
{
 //   Cv_FileArena< uint64_t, true, 10, 10, 12>   m_FileArena;
    Cv_Extent< uint32_t>                        m_Extent;
public:
    Cv_Stadium( void)
    {}
    
       
};
*/
//_____________________________________________________________________________________________________________________________

