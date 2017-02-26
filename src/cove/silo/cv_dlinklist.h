//_______________________________________________ cv_dlinklist.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

template <class DLink>
class Cv_DLink
{
protected:
    DLink    *m_Prev;
    DLink    *m_Next;

public:
    Cv_DLink(void) { Initialize(); }

    DLink       *Initialize(void)
    {
        m_Prev = static_cast<DLink *>(this);
        m_Next = NULL;
        return m_Prev;
    }

    DLink       *GetPrev(void) const { return m_Prev; }
    DLink       *GetNext(void) const { return m_Next; }

    void        SetPrev(DLink *d) { m_Prev = d; }
    void        SetNext(DLink *d) { m_Next = d; }

    //_____________________________________________________________________________________________________________________________
    // returns the head element of this List and if the list is circular

    DLink   *GetHeadLink(void) const
    {
        DLink	*dl = (DLink *) this;
        for (DLink *pr = dl->m_Prev; pr; pr = pr->m_Prev)              // prev field s always filled
        {
            if (!pr->m_Next)                                          // prev of head tail which has NULL next.
                return dl;
            if (pr == this)                                            // cirular list
                return pr;
            dl = pr;
        }
        //CV_ERROR_ASSERT(false)
        return NULL;
    }
};

//_____________________________________________________________________________________________________________________________
// DList holds the head, 
// The prev of head is always the tail. The next of tail is head if the list is circular. 
// This implies that It is only possible to hold circularity condn  internally 
// if the list has atleast one element.
// Hence, Cv_DList, independently, should always be  open.
// Ax_CList is derived from Cv_DList to enforce the circularity.

template <class DLink>
class Cv_DList
{
protected:

    DLink   *m_Head;

public:
    class Iterator;

    Cv_DList(DLink *h = NULL)
        : m_Head(h)
    {}


    DLink   *GetHead(void) const { return m_Head; }
    void    SetHead( DLink *dl) { m_Head = dl; }

    DLink   *GetTail(void) const
    {
        if (!m_Head)
            return NULL;
        return m_Head->GetPrev();
    }

    

    bool isSingle(void) const { return (GetTail() == GetHead()); }

    int nEntry(void)const
    {
        int n = 0;
        for (Iterator it = Begin().anchor(); it != NULL; ++n, ++it)
            ;
        return n;
    }

    //_____________________________________________________________________________________________________________________________

    void    Prepend(DLink *dl)
    {
        DLink   *tl = GetTail();
        if (tl)
        {
            m_Head->SetPrev(dl);
            dl->SetNext(m_Head);
            dl->SetPrev(tl);
        }
        else
        {
            CV_ERROR_ASSERT(m_Head == NULL)
                dl->SetPrev(dl); // make dl the GetTail
        }
        m_Head = dl;
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    Append(DLink *dl)
    {
        DLink   *tl = GetTail();
        if (tl)
        {
            tl->SetNext(dl);
            dl->SetPrev(tl);
        }
        else
        {
            CV_ERROR_ASSERT(m_Head == NULL)
                m_Head = dl;
        }
        m_Head->SetPrev(dl); // make dl the tail
        return;
    }

    //_____________________________________________________________________________________________________________________________

    DLink    *Remove(DLink *dl)
    {
        if (isSingle())
        {
            CV_ERROR_ASSERT(dl == m_Head);
            m_Head = NULL;
            return dl->Initialize();
        }
        
        DLink   *tl = GetTail();
        if ((dl != tl) && (dl != m_Head))
        {
            dl->GetNext()->SetPrev(dl->GetPrev());
            dl->GetPrev()->SetNext(dl->GetNext());
            return dl->Initialize();
        }
        if (dl == tl)
            m_Head->SetPrev(dl->GetPrev()); // make dl the tail
        else if (dl == m_Head)
        {
            m_Head = m_Head->GetNext();
            m_Head->SetPrev(tl);
        }
        return dl->Initialize();
    }

    //_____________________________________________________________________________________________________________________________

    void    InsertAfter(DLink *ref, DLink *dl)
    {
        DLink   *tl = GetTail();
        if (ref == tl)
        {
            append(dl);
            return;
        }
        dl->SetNext(ref->GetNext());
        dl->GetNext()->SetPrev(dl);
        dl->SetPrev(ref);
        ref->SetNext(dl);
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    InsertBefore(DLink *ref, DLink *dl)
    {
        if (ref == m_Head)
        {
            prepend(dl);
            return;
        }
        dl->SetPrev(ref->GetPrev());
        dl->GetPrev()->SetNext(dl);
        dl->SetNext(ref);
        ref->SetPrev(dl);
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    Transfer(Cv_DList< DLink> *dlist)
    {
        DLink   *tl = GetTail();
        DLink   *dh = dlist->GetHead();
        DLink   *dt = dlist->GetTail();
        if (tl)
        {
            tl->SetNext(dh);
            dh->SetPrev(tl);
        }
        else
        {
            //CV_ERROR_ASSERT(m_Head == NULL)
            m_Head = dh;
        }
        m_Head->SetPrev(dt); // make dt the tail
        dlist->m_Head = NULL;
        return;
    }

    //_____________________________________________________________________________________________________________________________

    
};


template <class DLink, bool CircularFlag = false>
class Cv_CList
{
protected:

    DLink   *m_Head;

    void    MkClose(void) { GetTail()->SetNext(GetHead()); }
    void    MkOpen(void)
    {
        DLink*  tl = GetTail();
        if (tl)
            tl->SetNext(NULL);
    }
public:
    class Iterator;

    Cv_CList(DLink *h = NULL)
        : m_Head(h)
    {}


    DLink   *GetHead(void) const { return m_Head; }
    void    SetHead( DLink *dl) { m_Head = dl; }

    DLink   *GetTail(void) const
    {
        if (!m_Head)
            return NULL;
        return m_Head->GetPrev();
    }

    bool isOpen(void) const
    {
        if (!m_Head)
            return !CircularFlag;
        if (!GetTail()->GetNext())
            return true;
        CV_ERROR_ASSERT(((DLink *)GetTail()->GetNext()) == m_Head)
            return false;
    }

    bool isSingle(void) const { return (GetTail() == GetHead()); }

    int nEntry(void)const
    {
        int n = 0;
        for (Iterator it = Begin().anchor(); it != NULL; ++n, ++it)
            ;
        return n;
    }

    //_____________________________________________________________________________________________________________________________

    void    Prepend(DLink *dl)
    {
        if (CircularFlag && (GetHead() == NULL))
        {
            m_Head = dl;
            MkClose();
            return;
        }
        bool    opn = isOpen();
        DLink   *tl = GetTail();
        if (tl)
        {
            m_Head->SetPrev(dl);
            dl->SetNext(m_Head);
            dl->SetPrev(tl);
        }
        else
        {
            CV_ERROR_ASSERT(m_Head == NULL)
                dl->SetPrev(dl); // make dl the GetTail
        }
        m_Head = dl;
        if (opn)
            MkOpen();
        else
            MkClose();
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    Append(DLink *dl)
    {
        if (CircularFlag && (GetHead() == NULL))
        {
            m_Head = dl;
            MkClose();
            return;
        }
        bool    opn = isOpen();
        DLink   *tl = GetTail();
        if (tl)
        {
            tl->SetNext(dl);
            dl->SetPrev(tl);
        }
        else
        {
            CV_ERROR_ASSERT(m_Head == NULL)
                m_Head = dl;
        }
        m_Head->SetPrev(dl); // make dl the tail
        if (opn)
            MkOpen();
        else
            MkClose();
        return;
    }

    //_____________________________________________________________________________________________________________________________

    Cv_CList    *Remove(DLink *dl)
    {
        if (isSingle())
        {
            CV_ERROR_ASSERT(dl == m_Head);
            m_Head = NULL;
            return dl->Initialize();
        }
        bool    opn = isOpen();
        DLink   *tl = GetTail();
        if ((dl != tl) && (dl != m_Head))
        {
            dl->GetNext()->SetPrev(dl->GetPrev());
            dl->GetPrev()->SetNext(dl->GetNext());
            return dl->Initialize();
        }
        if (dl == tl)
            m_Head->SetPrev(dl->GetPrev()); // make dl the tail
        else if (dl == m_Head)
        {
            m_Head = m_Head->GetNext();
            m_Head->SetPrev(tl);
        }
        if (opn)
            MkOpen();
        else
            MkClose();
        return dl->Initialize();
    }

    //_____________________________________________________________________________________________________________________________

    void    InsertAfter(DLink *ref, DLink *dl)
    {
        bool    opn = isOpen();
        DLink   *tl = GetTail();
        if (ref == tl)
        {
            append(dl);
            return;
        }
        dl->SetNext(ref->GetNext());
        dl->GetNext()->SetPrev(dl);
        dl->SetPrev(ref);
        ref->SetNext(dl);
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    InsertBefore(DLink *ref, DLink *dl)
    {
        bool    opn = isOpen();
        if (ref == m_Head)
        {
            prepend(dl);
            return;
        }
        dl->SetPrev(ref->GetPrev());
        dl->GetPrev()->SetNext(dl);
        dl->SetNext(ref);
        ref->SetPrev(dl);
        return;
    }

    //_____________________________________________________________________________________________________________________________

    void    Transfer(Cv_DList< DLink> *dlist)
    {
        bool    opn = isOpen();
        DLink   *tl = GetTail();
        DLink   *dh = dlist->GetHead();
        DLink   *dt = dlist->GetTail();
        if (tl)
        {
            tl->SetNext(dh);
            dh->SetPrev(tl);
        }
        else
        {
            CV_ERROR_ASSERT(m_Head == NULL)
                m_Head = dh;
        }
        m_Head->SetPrev(dt); // make dt the tail
        if (opn)
            MkOpen();
        else
            MkClose();
        dlist->m_Head = NULL;
        return;
    }

    //_____________________________________________________________________________________________________________________________

    
};

//----------------------------------------------------------------------



