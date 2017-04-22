//_______________________________________________ cv_typebouqet.h _______________________________________________________________
#pragma once

#include    "cove/silo/cv_pack.h"
#include	"cove/barn/cv_trail.h"
#include    "cove/flux/cv_xmlencoder.h"
#include	"cove/barn/cv_aid.h"
#include	"cove/barn/cv_ptr.h"
#include    "cove/silo/cv_cstr.h"
#include	"cove/flux/cv_spritz.h"
#include    "cove/silo/cv_iota.h"
#include    "cove/silo/cv_traversor.h"

#include    <map>

//_____________________________________________________________________________________________________________________________

class Cv_TypeCask;
class Cv_TypeCasket;
class Cv_TypeCaskData;
class Cv_ParamCasket;
class Cv_Docket;

//_____________________________________________________________________________________________________________________________

template < class T>
Cv_TypeCask     *cv_TypeCask( void);

//_____________________________________________________________________________________________________________________________

class Cv_TypeCaskBase : public Cv_Trail::Base< Cv_TypeCaskBase>
{
public:
    static const char   *ModuleName( void) { return "TypeCask"; }
    static bool         IsActive( void) { return true; }
};

//_____________________________________________________________________________________________________________________________

class Cv_TypeElement
{
    struct Str
    {
        std::string     m_Value;

        Str( const std::string &str)
            : m_Value( str)
        {}
    };

    struct Var 
    {
        Cv_TypeCasket   *m_Casket;
        const void      *m_Ptr;

        Var( Cv_TypeCasket *casket, const void *ptr)
            : m_Casket( casket), m_Ptr( ptr)
        {}
    };
    Cv_CStr         m_Name;
    Var             *m_Var;
    Str             *m_Str;
    
public:
    Cv_TypeElement( void)
        : m_Var( nullptr), m_Str( nullptr)
    {}
    
    Cv_TypeElement( const Cv_CStr &name, Cv_TypeCasket *casket, const void *ptr)
        : m_Name( name), m_Var( nullptr), m_Str( nullptr)
    {
        m_Var = new Var( casket, ptr);
    }

    Cv_TypeElement( const Cv_CStr &name, const std::string &str)
        : m_Name( name), m_Var( nullptr), m_Str( nullptr)
    {
        m_Str = new Str( str);
    }

    Cv_TypeElement( const Cv_TypeElement &elem)
        : m_Name( elem.m_Name), m_Var( nullptr), m_Str( nullptr)
    {
        if ( elem.m_Var)
            m_Var = new Var( elem.m_Var->m_Casket, elem.m_Var->m_Ptr);
        if ( elem.m_Str)
            m_Str = new Str( elem.m_Str->m_Value);
    }            

    ~Cv_TypeElement( void)
    {
        if ( m_Var)
            delete m_Var;
        if ( m_Str)
            delete m_Str;
    }
    Cv_TypeElement &operator=( const Cv_TypeElement &elem)
    {
        if ( this == &elem)
            return SELF;
        this->Cv_TypeElement::~Cv_TypeElement();
        new (this) Cv_TypeElement( elem);
        return SELF;

    }
    
    bool            IsVar( void) const { return m_Var != nullptr; }
    const Cv_CStr   &Name( void) const { return m_Name; }
    Cv_TypeCasket   *Casket( void) const { return m_Var->m_Casket; }
    
    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket);

    Cv_Enumerator< Cv_TypeElement>  *CollectSubElementsEnumberator( Cv_Docket *xDocket);

    bool                            Serialize( Cv_AttribValueReceptor *receptor) const;

    bool            operator==( const Cv_TypeElement &a) const 
    { 
        int32_t     res = m_Name.Compare( a.m_Name);
        if ( res || ( !m_Var != !a.m_Var) || ( !m_Str != !a.m_Str))
            return false;
        if ( m_Var)
            return ( m_Var->m_Casket == a.m_Var->m_Casket) && ( m_Var->m_Ptr == a.m_Var->m_Ptr);
        if ( m_Str)
            return m_Str->m_Value == a.m_Str->m_Value;
        return true;
    }
};

//_____________________________________________________________________________________________________________________________

class Cv_XMlXlemDispensor : public Cv_XlemDispensor
{
    Cv_TypeElement                  m_Elem;
    Cv_Docket                       *m_XDocket;
    std::vector< Cv_TypeElement>    m_SubElements;
    uint32_t                        m_Ind;

public:
    Cv_XMlXlemDispensor( const Cv_TypeElement &elem, Cv_Docket *xDocket);

    const Cv_CStr           &Name( void);
    bool                    MoveNextAttrib( Cv_AttribValueReceptor *);
    const Cv_CStr           &AttribName( void);
    bool                    HasChildren( void);

    const std::vector< Cv_TypeElement>  &SubElements( void) const { return m_SubElements; }

};
    
//_____________________________________________________________________________________________________________________________

class Cv_TypeBouquet : public Cv_TypeCaskBase
{
public:
    typedef std::pair< Cv_TypeCasket *, uint32_t>   BaseRec;
    
    struct CaskDictEntry
    {
        Cv_Ptr< Cv_TypeCaskData>                m_Cask;
        uint32_t                                m_CaskMapId;

        CaskDictEntry( Cv_TypeCaskData *typeData = nullptr)
            : m_Cask( typeData), m_CaskMapId( CV_UINT32_MAX)
        {}
        
        int32_t                 Compare( const CaskDictEntry &a) const;
        
        friend std::ostream     &operator<<( std::ostream &ostr, const CaskDictEntry &cd);
    };
    
    struct ParamCasketDictEntry
    {
        Cv_Ptr< Cv_ParamCasket>             m_Casket;
        uint32_t                            m_CasketMapId;

        ParamCasketDictEntry( Cv_ParamCasket *typeData = nullptr)
            : m_Casket( typeData), m_CasketMapId( CV_UINT32_MAX)
        {}
        
        int32_t                 Compare( const ParamCasketDictEntry &a) const;
        
        friend std::ostream     &operator<<( std::ostream &ostr, const ParamCasketDictEntry &cd);
    };

    struct ParamCaskData
    {
        Cv_TypeCaskData                     *m_Cask;
        std::vector< ParamCasketDictEntry > m_Caskets;

        friend std::ostream     &operator<<( std::ostream &ostr, const ParamCaskData &cd);
    };
    
protected:
    std::vector< CaskDictEntry>             m_FixedCasks;
    std::vector< Cv_TypeCaskData *>         m_FixedCaskArray;

    std::vector< CaskDictEntry>             m_ParamCasks;
    std::vector< ParamCaskData>             m_ParamCaskMap;
    std::vector< Cv_ParamCasket *>          m_ParamCasketArray;

public:
    Cv_TypeBouquet( void);

    struct DerivedCasket
    {
        Cv_TypeCasket                           *m_Casket;
        uint32_t                                m_BOffset;
        uint32_t                                m_Deriv;

        DerivedCasket( Cv_TypeCasket *casket, uint32_t offset, uint32_t deriv)
            : m_Casket( casket), m_BOffset( offset), m_Deriv( deriv)
        {}

        const Cv_CStr           &CasketName( void) const;

        friend std::ostream     &operator<<( std::ostream &ostr, const DerivedCasket &cd)
        {
            ostr << "DerivedCasket[ ";
            ostr << "Code[ " << cd.m_Deriv << "] ";
            ostr << "Offset[ " << cd.m_BOffset << "] ";
            ostr << "] ";
            return ostr;
        }
    };

    uint32_t                SzParamCasketArray( void) const { return uint32_t( m_ParamCasketArray.size()); }
    uint32_t                SzFixedCaskArray( void) const { return uint32_t( m_FixedCaskArray.size()); }

    uint32_t                FetchCaskIndex( Cv_TypeCaskData *typeData);
    uint32_t                FetchParamCasketIndex( uint32_t caskIndex, const void *thisPtr);

    uint32_t                LocateCaskIndex( Cv_TypeCaskData *typeData) const;
    Cv_TypeCasket           *TypeCasket( uint32_t caskInd, uint32_t casketInd) const;

    uint32_t                CodeForCasket( Cv_TypeCasket  *casket) const;
    Cv_TypeCasket           *CasketForCode( uint32_t code) const;

    std::vector< Cv_TypeCasket *>   FindCaskets( Cv_TypeCask  *cask) const;

    uint32_t                CasketIndex( Cv_TypeCasket  *casket) const;

    Cv_TypeCasket           *LocateCasket( Cv_TypeCaskData *typeCask, const void *thisPtr);

    Cv_TypeCasket           *PositionCasket( Cv_TypeCaskData *typeCask, uint32_t casket);

    Cv_TypeCasket           *FetchCasketForObj( const std::type_info *objInfo, const void *pObj);

    DerivedCasket           FetchDerivedCasketForObj( Cv_TypeCask *cask, Cv_TypeCasket *dCasket, const void *pObj);

    DerivedCasket           FetchDerivedCasketForObj( Cv_TypeCask *cask, const std::type_info *objInfo, const void *pObj);

    DerivedCasket           FetchDerivedCasketFoCode( Cv_TypeCask *cask, uint32_t derivIndex, uint32_t casketId);


    friend std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeBouquet &cd);

    //_____________________________________________________________________________________________________________________________

    struct CasketStore
    {
        Cv_TypeBouquet      *m_Bouquet;
        
        CasketStore( Cv_TypeBouquet *bouquet)
            :   m_Bouquet( bouquet)
        {} 

        uint32_t    CasketCode( uint32_t stateId) const 
        {
            if ( stateId < m_Bouquet->m_FixedCaskArray.size())  
                return stateId;
            stateId -= uint32_t( m_Bouquet->m_FixedCaskArray.size());
            return stateId | ( 1 << 31);
        }
        
        uint32_t    StateId( uint32_t casketCode) const 
        {
            uint32_t    index = casketCode &  ~( 1 << 31);
            if ( casketCode &  ( 1 << 31))
                return index + uint32_t( m_Bouquet->m_FixedCaskArray.size());
            return index;
        }
         
        uint32_t    Size( void) const { return uint32_t( m_Bouquet->m_FixedCaskArray.size() + m_Bouquet->m_ParamCasketArray.size()); }
    };
};

//_____________________________________________________________________________________________________________________________

class  Cv_Docket : public Cv_TypeCaskBase
{
public:
    Cv_Spritz           *m_FileSpritz;
    Cv_TypeBouquet      *m_Bouquet;

public:
    Cv_Docket( Cv_Spritz *spritz, Cv_TypeBouquet *bouquet = nullptr)
        :  m_FileSpritz( spritz), m_Bouquet( bouquet)
    {}

    Cv_TypeBouquet          *Bouquet( void) {  return m_Bouquet; }
    const Cv_TypeBouquet    *Bouquet( void) const {  return m_Bouquet; }
    void                    SetBouquet( Cv_TypeBouquet *bouquet) {  m_Bouquet = bouquet; }

    Cv_Spritz               *Spritz( void) {  return m_FileSpritz; }

    virtual uint32_t        ObjectId( const void *p) { return 0; }
};

//_____________________________________________________________________________________________________________________________

class  Cv_OutDocket : public  Cv_Docket
{
public:
    struct DefOffListener
    {
        virtual ~DefOffListener( void) {}
        virtual void   DefCallback( Cv_Spritz *sink, uint32_t defOff)  {}
    };

protected:
    
    struct  ObjOffsetInfoMap
    {
        std::map< const void *, uint32_t>                                   m_ObjOffMap;
        std::map< const void *, std::vector< Cv_OPtr< DefOffListener> > >   m_ObjReflocs;


        // return the offsets of pending references, if any.
        std::vector< Cv_OPtr< DefOffListener> >    FetchPendingRefs( const void *pObj, uint32_t defOff);

        // return the defOff else add to the the pending refs
        uint32_t    FetchDefOffset( const void *pObj, DefOffListener *refOff);
    };

    std::map< Cv_TypeCaskData *, ObjOffsetInfoMap, Cv_TLess< void> >   m_ObjOffsetMapSet;

public:
    Cv_OutDocket(  Cv_Spritz *spritz)
        :  Cv_Docket( spritz) 
    {}

    uint32_t    StoreObject( Cv_TypeCasket *casket, const void *x);

template < typename X>
    uint32_t    StoreObject( X *x)  {   return StoreObject( Bouquet()->LocateCasket( cv_TypeCask< X>(), x), x); }

    void        RegisterObjectDefToOffset(  Cv_TypeCask *cask, const void *pObj, uint32_t defOff);

    void        RegisterObjectRefToOffset(  Cv_TypeCask *cask, const void *pObj, DefOffListener *off);

template < class T>
    auto        Serialize( T *pObj)
    {
        uint32_t            baseOff = this->Spritz()->Offset();
        Cv_TypeBouquet      typeBouquet;
        SetBouquet( &typeBouquet);
        Spritz()->SetOffset( baseOff + 8);
        uint32_t            casketCode = StoreObject( pObj);
        Spritz()->SetOffsetAtEnd();
        uint32_t            off = Spritz()->Offset();
        Spritz()->SetOffset( baseOff);
        Spritz()->Write( &off, 4);
        Spritz()->Write( &casketCode, 4);
        Spritz()->SetOffset( baseOff +off);
        Cv_TypeBouquet      typeBouquet1 = typeBouquet;
        uint32_t            tbCasketCode = StoreObject( &typeBouquet1);
        return std::make_tuple( casketCode, typeBouquet);
    }
    
};

//_____________________________________________________________________________________________________________________________

class  Cv_InDocket : public  Cv_Docket
{
public:
    struct DefObjListener
    {
        virtual ~DefObjListener( void) {}
        virtual void   RefCallback( void *)  {}
    };

protected:
    struct  OffObjectInfoMap
    {
        std::map< uint32_t, void *>                                     m_OffObjMap;
        std::map< uint32_t, std::vector< Cv_OPtr< DefObjListener> > >   m_OffObjRefs;

        // return the offsets of pending references, if any.
        std::vector< Cv_OPtr< DefObjListener> >     FetchPendingRefs( uint32_t defOff, void *pObj);

        // return the defOff else add to the the pending refs
        void          *FetchDefObject( uint32_t objOff, DefObjListener *refCallback);

    };
    std::map< Cv_TypeCask *, OffObjectInfoMap>              m_OffObjectMapSet;

public:
    Cv_InDocket(  Cv_Spritz *spritz)
        :  Cv_Docket( spritz) 
    {}

    bool    RestoreObject( Cv_TypeCasket *casket, void *x);

template < typename X>
    bool    RestoreObject( X *x, uint32_t casketCode)
                {   return RestoreObject( Bouquet()->CasketForCode( casketCode), x); }

    bool    RestoreBouquet( Cv_TypeBouquet *x);

    void    RegisterOffsetToObjectDef( Cv_TypeCask *cask, uint32_t off, void *pObj);

    void    RegisterRefCallbackForObjectOff( Cv_TypeCask *cask, uint32_t objOff, DefObjListener *refCallback);

template < class T>
    void    LoadObject( T *pObj)
    {
        uint32_t        baseOff = Spritz()->Offset();
        uint32_t        off = 0;
        Spritz()->Read( &off, 4);
        
        uint32_t        casketCode = 0;
        Spritz()->Read( &casketCode, 4);
        Spritz()->SetOffset( off);

        Cv_TypeBouquet      typeBouquet;
        SetBouquet( &typeBouquet); 
        Cv_TypeBouquet      typeBouquet2;
        RestoreBouquet( &typeBouquet2);
        
        Spritz()->SetOffset( baseOff +8);
        
        SetBouquet( &typeBouquet2);    
        RestoreObject( pObj, casketCode);
        return;
    }
    
};

//_____________________________________________________________________________________________________________________________

class  Cv_XOutDocket : public  Cv_Docket
{
    Cv_XmlEncoder                       m_XEncoder;
    Cv_TreeTraversor< Cv_TypeElement>   m_TreeTraversor;
    std::map< const void *, uint32_t>   m_ObjMap;

public:
    Cv_XOutDocket(  Cv_Spritz *spritz)
        :  Cv_Docket( spritz), m_XEncoder( spritz) 
    {}

    Cv_XmlEncoder   *XEncoder( void) { return &m_XEncoder; }
    
    uint32_t        StoreXDoc( const Cv_CStr &name, Cv_TypeCasket *casket, const void *x);

template < typename X>
    uint32_t        StoreXDoc( const Cv_CStr &name, X *x)  
    {       
        return StoreXDoc( name, Bouquet()->LocateCasket( cv_TypeCask< X>(), x), x); 
    }

    uint32_t        ObjectId( const void *p) 
    { 
        std::map< const void *, uint32_t>::iterator   it = m_ObjMap.find( p);
        if ( it != m_ObjMap.end())
            return it->second;
        uint32_t    id = uint32_t( m_ObjMap.size()) + 1;
        m_ObjMap.insert( it, std::pair< const void *, uint32_t>( p, id));
        return id;
    }

    bool            ElemTravesor( const Cv_TypeElement &elem, Cv_TreeTraversor< Cv_TypeElement> *context, bool entryFlg, void *);  
};

//_____________________________________________________________________________________________________________________________

class  Cv_OutSourceDocket : public  Cv_Docket, public Cv_GraphTraversor< Cv_OutSourceDocket>
{
    Cv_SpritzLog                        m_SourceLog;
    std::map< std::string, uint32_t>    m_CasketIndFlagMap;

    uint32_t        IndexForCasketCode( uint32_t code) const
        {   return (( code &  ( 1 << 31)) ? Bouquet()->SzFixedCaskArray() : 0) + ( code &  ~( 1 << 31));}

    uint32_t        CasketCodeForIndex( uint32_t index) const
        {   return ( index < Bouquet()->SzFixedCaskArray()) ? index : (( 1 << 31) | ( index -Bouquet()->SzFixedCaskArray())); }
    
    struct IndexEnumertor : public Cv_Enumerator< uint32_t>
    {
        Cv_OutSourceDocket              *m_OutSourceDocket;
        Cv_Enumerator< Cv_TypeElement>  *m_ElemEnumerator;

        IndexEnumertor( Cv_OutSourceDocket *osDocket, Cv_Enumerator< Cv_TypeElement> *enm)
            : m_OutSourceDocket( osDocket), m_ElemEnumerator( enm)
        {}

        bool        MoveNext( void)  
        { 
            bool    res = m_ElemEnumerator->MoveNext();
            while ( res && !m_ElemEnumerator->Current().IsVar())
                res = m_ElemEnumerator->MoveNext();
            return res; 
        }
        
        uint32_t    Current( void) const;
    
    };

public:
    
    Cv_OutSourceDocket( Cv_Spritz *spritz, Cv_TypeBouquet *bouquet);

    ~Cv_OutSourceDocket( void);

    Cv_Enumerator< uint32_t>    *StateEnumerator( uint32_t stateId);

    bool            EncodeCasket( uint32_t stateId, Cv_GraphTraversor< Cv_OutSourceDocket> *context, bool entryFlg);
    
    uint32_t        Size( void) const { return Bouquet()->SzFixedCaskArray() + Bouquet()->SzParamCasketArray(); }

    bool            IsProcessed( Cv_TypeCasket *fieldCasket);

    uint32_t        IndexForCasket( Cv_TypeCasket *fieldCasket) const
        {   return IndexForCasketCode( Bouquet()->CodeForCasket( fieldCasket)); }

    Cv_TypeCasket   *CasketForIndex( uint32_t index) const
        {   return Bouquet()->CasketForCode( CasketCodeForIndex( index)); }

    void            StoreDefinition( uint32_t casketCode);
    Cv_SpritzLog    &SourceLog( void) { return m_SourceLog; }

    uint32_t        *FlagForTemplate( const std::string &templateName) { return  &m_CasketIndFlagMap[ templateName]; }
};

//_____________________________________________________________________________________________________________________________
