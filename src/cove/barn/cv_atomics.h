//________________________________________________ cv_atomics.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include    <atomic>
#include    <cassert>
#include    <thread>

//_____________________________________________________________________________________________________________________________
template < class X, bool MT = true>
class Cv_Atomic
{
};

template < class X>
class Cv_Atomic< X, true>
{
    std::atomic< X>   m_X;

public:
    Cv_Atomic( const X &x)
        : m_X( x)
    {}
    void    Store( const X &x) { m_X =  x; }

    bool    Busy( const X &lock) const { return m_X.load() == lock; }
    
    X       Load( void) const { return m_X.load(); }

    X       Incr( void) { return ++m_X; }
    X       Decr( void) { return --m_X; }

    X       SpinGrab( const X &lock)
    {
        X        x = m_X.load();

        // null means someone grabbed it already and we wait our turn
        // if x is null then yeild and reload else try compare_exchange with lock to tell others to keep off.
        while ( ( ( x == lock) && ( std::this_thread::yield(), x = m_X.load(), true)) || ( ( x != lock) && !m_X.compare_exchange_weak( x, lock)))
        {} 
        assert( m_X.load() == lock && x != lock);
        return x;
    }
};

template < class X>
class Cv_Atomic< X, false>
{
    X       m_X;

public:
    Cv_Atomic( const X &x)
        : m_X( x)
    {}
    void    Store( const X &x) { m_X =  x; }

    bool    Busy( const X &lock) const { m_X == lock; }
    
    X       Load( void) const { return m_X; }

    X       Incr( void) { return ++m_X; }
    X       Decr( void) { return --m_X; }

    X       SpinGrab( const X &lock)
    {
        X        x = m_X;
        m_X = lock;
        return x;
    }
};

template < class X>
class Cv_Atomic< X *, true>  
{
    std::atomic< X *>   m_X;

    static X  *LockValue( void) { return reinterpret_cast<  X*>( -1); }

public:
    Cv_Atomic( X *x)
        : m_X( x)
    {}
    void    Store( X *x) { m_X =  x; }

    bool    Busy( X *lock = LockValue()) const { return m_X.load() == lock; }
    
    X       *Load( void) const { return m_X.load(); }

    X       *SpinGrab( X *lock = LockValue())
    {
        X        *x = m_X.load();

        // null means someone grabbed it already and we wait our turn
        // if x is null then yeild and reload else try compare_exchange with null to tell others to keep off.
        while ( ( ( x == lock) && ( std::this_thread::yield(), x = m_X.load(), true)) || ( ( x != lock) && !m_X.compare_exchange_weak( x, lock)))
        {} 
        assert( m_X.load() == lock && x != lock);
        return x;
    }
}; 

template < class X>
class Cv_Atomic< X *, false>  
{
    X       *m_X;

    static X  *LockValue( void) { return reinterpret_cast<  X*>( -1);  }

public:
    Cv_Atomic( X *x)
        : m_X( x)
    {}

    void    Store( X *x) { m_X = x; }
    
    bool    Busy( X *lock = LockValue()) const { return m_X  == lock; }
    
    X       *Load( void) const { return m_X; }

    X       *SpinGrab( X *lock = LockValue())
    {
        X        *x = m_X;
        m_X = lock;
        return x;
    }
};

template < class X>
class Cv_NAtomic : public Cv_Atomic< X, false>  
{

public:
    Cv_NAtomic( X *x)
        : Cv_Atomic< X, false>( x)
    {}

};

//_____________________________________________________________________________________________________________________________
