//_______________________________________________ cv_crate.h _______________________________________________________________
#pragma once
 
//_____________________________________________________________________________________________________________________________

class Cv_CrateEntry
{
protected:
    uint32_t                         m_Id;

public:
    Cv_CrateEntry( uint32_t id = CV_UINT32_MAX)
        :   m_Id( id)
    {}

    uint32_t        GetId( void) const { return m_Id; }
    void            SetId( uint32_t k) { m_Id = k; }
};   

//_____________________________________________________________________________________________________________________________

template < class X>
class Cv_Crate 
{
public:
    std::vector< X *>     m_Elems;

public:
    //_____________________________________________________________________________________________________________________________

    typedef typename std::vector< X *>::iterator                Iterator;
    typedef typename std::vector< X *>::const_iterator          CIterator;

    //_____________________________________________________________________________________________________________________________

    Cv_Crate( void)
    {
        m_Elems.push_back( nullptr);
    }

    //_____________________________________________________________________________________________________________________________

    ~Cv_Crate( void)
    {
	    Cleanup();
    }

    //_____________________________________________________________________________________________________________________________

    void Cleanup( void)
    {
        for ( uint32_t i = 0; i < Size(); ++i)
	    {
		    X     *si = m_Elems[ i];
            m_Elems[ i] = nullptr;
		    if ( si && (si != ( X*) -1))
			    delete si;
	    }
        m_Elems = std::vector< X *>();
        m_Elems.push_back( nullptr);

    }

    //_____________________________________________________________________________________________________________________________

    uint32_t    Size( void) const { return uint32_t( m_Elems.size()); }

    //_____________________________________________________________________________________________________________________________

    void        EnsurePos( uint32_t k)
    {
        if ( k >= Size())
            m_Elems.resize( k +1);
    }

    //_____________________________________________________________________________________________________________________________
    
    void        Resize( uint32_t sz)
    {
        m_Elems.resize( sz);
    }

    //_____________________________________________________________________________________________________________________________

    X           *Fetch( uint32_t k)
    {
        if ( k == 0)
            return nullptr;
        
        EnsurePos( k);
        return m_Elems[ k] ? m_Elems[ k] :  ( m_Elems[ k] = new X( k));
    }

    //_____________________________________________________________________________________________________________________________

template < class Y>
    Y           *Fetch( uint32_t k)
    {
        if ( k == 0)
            return nullptr;

        EnsurePos( k);
        if ( m_Elems[ k] == nullptr)
            return static_cast< Y *>( m_Elems[ k] = new Y( k));
        return dynamic_cast< Y *>( m_Elems[ k]);
    }

    //_____________________________________________________________________________________________________________________________

template < class Compare>
    X           *Locate( X *y, Compare cmp) const
    {
        typename std::vector<X *>::const_iterator   cIt = std::lower_bound( m_Elems.begin(), m_Elems.end(), y, cmp);
        if (( cIt != m_Elems.end()) &&  !cmp( *cIt, y))
            return *cIt;
        return  nullptr;
    }

    //_____________________________________________________________________________________________________________________________

    void    Store( X *x, uint32_t k = CV_UINT32_MAX)
    {
        if ( k == CV_UINT32_MAX)
            k = Size();

        EnsurePos( k);
        if ( m_Elems[ k])
            delete m_Elems[ k];
        m_Elems[ k] = x;
        x->SetId( k);
        return;

    }
    
    //_____________________________________________________________________________________________________________________________

template < class Y>
    Y           *Create( uint32_t k = CV_UINT32_MAX)
    {
        if ( k == CV_UINT32_MAX)
            k = Size();

        Y       *x = new Y( k);
        Store( x, k);
        return x;
    }
    
    //_____________________________________________________________________________________________________________________________

    void        Destroy( uint32_t k)
    {
        CV_ERROR_ASSERT( k < Size() && m_Elems[ k])
        X       *x = m_Elems[ k];
        m_Elems[ k] = nullptr;
        delete x;
        return;
    }
    
    //_____________________________________________________________________________________________________________________________

    bool        IsAt( uint32_t k) const
    {
        if (( k >= Size()) || !m_Elems[ k])
            return false;
        return true;
    }

    //_____________________________________________________________________________________________________________________________

    X           *GetAt( uint32_t i) const 
    {
        return ( uint32_t( i) < Size()) ? m_Elems[ i] : nullptr;
    }

    //_____________________________________________________________________________________________________________________________

template < class Y>
    Y           *GetAt( uint32_t i) const 
    {
        return ( uint32_t( i) < Size()) ? dynamic_cast< Y *>( m_Elems[ i]) : nullptr;
    }

    //_____________________________________________________________________________________________________________________________

    X           *AccAt( uint32_t i) const 
    {
        return  m_Elems[ i];
    }

    //_____________________________________________________________________________________________________________________________

template < class Y>
    Y           *AccAt( uint32_t i) const 
    {
        return  static_cast< Y *>( m_Elems[ i]);
    }

    //_____________________________________________________________________________________________________________________________

    void        SetAt( uint32_t i, X  *x) 
    {
        EnsurePos( i);
        m_Elems[ i] = x;
    }

    //_____________________________________________________________________________________________________________________________

    void    Swap( Cv_Crate< X> *tc) {   m_Elems.swap( tc->m_Elems);}
        
    //_____________________________________________________________________________________________________________________________

    std::vector< uint32_t>      Herd( void) const 
    {
        std::vector<  uint32_t>     arr;
        arr.reserve( Size());
        for ( uint32_t i = 0; i < Size(); ++i)
            if ( m_Elems[ i])
                arr.push_back( i);
        return arr;
    }

    //_____________________________________________________________________________________________________________________________

template < class Y>
    uint32_t    NumElem( uint32_t start = 0) const 
    {
        uint32_t    sz = 0;
        for ( uint32_t i = start; i < Size(); ++i)
            if ( GetAt< Y>( i))
                ++sz;
        return sz;
    }

    //_____________________________________________________________________________________________________________________________
    // idMap hold the new destination Ids : idmap[] == 0 => They get assigned NULL

    void        Reorder( const std::vector< uint32_t> &idMap, uint32_t beg)
    {
        std::vector< X *>     elems( m_Elems.begin() +beg,  m_Elems.end());
        for ( uint32_t i = beg; i < Size(); ++i)
        {
            X               *x = elems[ i -beg];
            uint32_t        k = idMap[ i -beg];
            if ( !x || !k)
            {
                m_Elems[ k] = nullptr;
                continue;
            }
            m_Elems[ k] = x;
            x->SetId( k); 
        }
        return;
    }

    //_____________________________________________________________________________________________________________________________
template < class BinOp>
    std::vector< uint32_t>    SortElems( const BinOp &binFn)
    {    
        std::vector< X *>          ecList;
        for ( uint32_t i = 1; i <  Size(); ++i)
        {
            X                   *ec =  AccAt( i);
            if ( ec)
                ecList.push_back( ec);
        }
        std::sort( ecList.begin(), ecList.end(), binFn);

        std::vector< uint32_t>          ecIdList( Size(), 0);
        for ( uint32_t i = 0; i <  ecList.size(); ++i)  
            ecIdList[  ecList[ i]->GetId()]  = i +1;
    
        Reorder( ecIdList, 0);
        Resize( ecList.size() +1);
        return ecIdList;
    }
};

//_____________________________________________________________________________________________________________________________
