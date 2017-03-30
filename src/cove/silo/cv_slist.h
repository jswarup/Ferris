//_______________________________________________ cv_slist.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template <class Link>
class  Cv_Link 
{
protected:
	Link            *m_Below;   // the link to next

public:
	Cv_Link( Link *b = NULL)  
        : m_Below( b) 
    {}
	
    ~Cv_Link( void) 
    { Initialize(); }

	void            Initialize( void) { m_Below = NULL; }
	Link            *GetBelow( void) const { return m_Below; }
	void            SetBelow( Link *b) { m_Below = b; }

};

//------------------------------------------------------------------------------------------------

template <class Link>
class  Cv_LinkStack 
{
protected:

	Link    *m_Top;             // the head of the linked-list

public :

	Cv_LinkStack( Link *tp = NULL) : m_Top( tp) {}
	void        Initialize( void) { m_Top = NULL; }

	Link    *Top( void)  const { return m_Top; }

	// fetch the last element
	Link    *Bottom( void) const 
	{
		Link        *tmp = m_Top;
		while ( tmp && tmp->GetBelow())
			tmp = tmp->GetBelow();
		return tmp;
	}

	// take out the head and put the following one in its place.
	Link    *Pop( void)
	{
		if ( ! m_Top)
			return NULL;
		Link        *tmp = m_Top;
		m_Top = m_Top->GetBelow();
		return tmp;
	}


	// put a new element at the head and make it point to old head.
	void         Push( Link *v) 
	{       
		if ( ! v) 
			return;
		v->SetBelow( m_Top); 
		m_Top = v; 
        return;
	}

    // put a new element at the head and make it point to old head.
	void         Append( Link *v) 
	{       
		if ( !m_Top)
            Push( v);
		else
        {
            Link    *tmp = m_Top;
            for ( ; tmp->GetBelow(); tmp = tmp->GetBelow())
                ;
            tmp->SetBelow( v);
        }
        return;
	}

	// scan the list and Remove the given element from it.
	void         Remove( Link *v)
	{
		if ( ! m_Top)
			return;         // list is Empty

		if ( m_Top == v)    
		{   // the element is at head
			Pop();
			return;
		}
		Link    *tmp = m_Top;
		do 
		{
			if ( tmp->GetBelow() == v)
				break;
			tmp = tmp->GetBelow();
		} while( tmp);
		assert( tmp && tmp->GetBelow() && ( tmp->GetBelow() == v));
		tmp->SetBelow( v->GetBelow());
		return;
	}


	// Transfer all elements from stk to this => stk becomes Empty
	int  Transfer(  Cv_LinkStack< Link>  *stk)
	{
		int             i = 0;
		for ( Link  *t = nullptr; ( t = stk->Pop()) != nullptr; ++i)
			Push( t);
		return i;
	}

	// Reverse ourselves.
	int Reverse( void)
	{       
		Cv_LinkStack< Link>      tmp;
		int                     n = tmp.Transfer( this);
		m_Top = tmp.m_Top;
		return n;
	}

	int  SzVar(  void)
	{
		int             i = 0;
		for ( Link  *t = Top(); (t = t->GetBelow()) ; ++i)
			;
		return i;
	}

	bool    Find( Link *s) const
	{
		for ( Link    *v = Top(); v; v = v->GetBelow())
			if (v == s)
				return true;
		return false;
	}

	Link    *AboveOf( Link *s, Link *t = NULL) const
	{
        Link    *v = t;
        if ( !v)
            v = Top();
		for ( ; v; v = v->GetBelow())
			if (v->GetBelow() == s)
				return v;
		return NULL;
	}


    Link *PromoteAfter( Link *s, Link *beg, Link *end = NULL)
	{
        if ( !end)
            end = AboveOf( end, beg);

        assert( end);
        
        Link *sPrev = AboveOf( beg, s);
        if ( !sPrev)
            return NULL;
        sPrev->SetBelow( end->GetBelow());      // if end was last in chain, sPrev would now be last
        end->SetBelow( s->GetBelow());
        s->SetBelow(  beg);
        return sPrev;
    }

    
};


//_____________________________________________________________________________________________________________________________
