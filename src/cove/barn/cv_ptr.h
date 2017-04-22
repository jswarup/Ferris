//_______________________________________________ cv_optr.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template <class X>
class Cv_Ptr 
{
protected:
    X   *m_Ptr;

public:
	Cv_Ptr( X *ptr = nullptr) 
        : m_Ptr( ptr)
    {}

    Cv_Ptr( const Cv_Ptr &optr)
        : m_Ptr( optr.m_Ptr)
    {}

    Cv_Ptr( Cv_Ptr &&optr)
        : m_Ptr( optr.m_Ptr)
    {
        optr.m_Ptr = NULL;
    }

    Cv_Ptr &operator=( const Cv_Ptr &optr)
    {
         m_Ptr = optr.m_Ptr;
         return *this;
    }

    //_____________________________________________________________________________________________________________________________

    const X         &operator*( void) const { return *m_Ptr; }
    X               &operator*( void) { return *m_Ptr; }

    operator X      *( void) const { return m_Ptr; }   
    X               *operator->( void) const { return m_Ptr; }
    operator        bool( void) const { return m_Ptr != nullptr; }

    X               *Ptr( void) const { return m_Ptr; }
    void            SetPtr( X *x) { m_Ptr = x; }

    int32_t         Compare( const Cv_Ptr &a) const { return ( !m_Ptr || !a.m_Ptr) ?  ( m_Ptr < a.m_Ptr ? -1: 1) : m_Ptr->Compare( *a.m_Ptr); }

    //_____________________________________________________________________________________________________________________________

};

//_____________________________________________________________________________________________________________________________

template <class X>
class Cv_OPtr : public Cv_Ptr< X>
{
protected:
    using Cv_Ptr< X>::m_Ptr;   
    
    Cv_OPtr( const Cv_OPtr &optr);
    
    Cv_OPtr &operator=( const Cv_OPtr &optr);

public:
	Cv_OPtr( X *ptr = nullptr) 
        : Cv_Ptr< X>( ptr)
    {}

    Cv_OPtr( Cv_OPtr< X> &&optr)
        : Cv_Ptr< X>( optr)
    {
        optr.m_Ptr = nullptr;
    }

    ~Cv_OPtr( void) 
    {
        if ( m_Ptr != nullptr)
            delete m_Ptr;
    }

    void    Park( X *ptr)
    {
        if ( m_Ptr != nullptr)
            delete m_Ptr;
        m_Ptr = ptr;        
    } 
};

//_____________________________________________________________________________________________________________________________




