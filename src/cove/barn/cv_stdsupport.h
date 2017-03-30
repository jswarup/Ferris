//___________________________________________________  cv_stdsupport.h  ________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template < class Derived>
class Cv_PtrIteratorImpl
{
public:

template < class X>
    struct iterator : std::iterator< std::forward_iterator_tag,  X>
    {
        typedef X  value_type;
        
        X           *ptr;
        
        explicit iterator( X *p = nullptr)  : ptr( p) {}

        reference   operator*( void) { return *ptr; }

        iterator    &operator++( void) { ++ptr; return *this; }
        iterator    operator++( int) { iterator tmp = *this; ++*this; return tmp; }

        bool operator== (const iterator& other) const { return ptr == other.ptr; }
        bool operator!= (const iterator& other) const { return ptr != other.ptr; }
    };


template < class X>
    struct const_iterator : std::iterator< std::forward_iterator_tag, X>
    { 
        typedef X  value_type;
        
        const X     *ptr;

        explicit const_iterator (const X *p = nullptr) : ptr( p) {}
        const_iterator( const iterator &i) : ptr( i.ptr) {}

        reference operator*( void) { return ptr->val; }

        const_iterator  &operator++ () { ptr = ptr->next; return *this; }
        const_iterator  operator++ (int) { const_iterator tmp = *this; ++*this; return tmp; }

        bool operator==( const const_iterator &other) const { return ptr == other.ptr; }
        bool operator!=( const const_iterator &other) const { return ptr != other.ptr; }
    };

template < class X>
    friend bool operator==( iterator< X> a, const_iterator< X> b) { return a.ptr == b.ptr; }                   

template < class X>         
    friend bool operator!=( iterator< X> a, const_iterator< X> b) { return a.ptr != b.ptr; }

template < class X>         
    friend bool operator==( const_iterator< X> a, iterator< X> b) { return a.ptr == b.ptr; }

template < class X>         
    friend bool operator!=( const_iterator< X> a, iterator< X> b) { return a.ptr != b.ptr; }

    
template < class X>
    iterator< X>        At( uint32_t i) { return iterator< X>( static_cast< X *>( static_cast< Derived *>( this)->PtrAt< X>( i))); }

template < class X>
    const_iterator< X>  At( uint32_t i) const 
        { return const_iterator< X>( static_cast< const X *>( static_cast< const Derived *>( this)->PtrAt< X>( i))); }
};

//_____________________________________________________________________________________________________________________________

