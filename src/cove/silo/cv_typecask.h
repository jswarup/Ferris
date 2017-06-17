//_______________________________________________ cv_typecask.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include    "cove/silo/cv_typebouqet.h"
#include	"cove/silo/cv_typeusher.h"
#include    "cove/barn/cv_compare.h"
#include    "cove/barn/cv_minion.h"
#include    "cove/silo/cv_pack.h"
#include	"cove/barn/cv_trail.h"
#include    "cove/flux/cv_spritz.h"
#include    "cove/silo/cv_iota.h"
#include	"cove/barn/cv_aid.h"
#include    "cove/barn/cv_ptr.h"
#include    "cove/silo/cv_cstr.h"

//_____________________________________________________________________________________________________________________________

class   Cv_TypeCasket;
class   Cv_TypeCask;

template < class X>
class   Cv_StructCask;

template < class Adaptor>
class   Cv_BasicCask;

class   Cv_ParamCasket;

class   Cv_TypeBouquet;
class   Cv_TypeCaskMgr;
class   Cv_OutDocket;
class   Cv_InDocket;
class  Cv_StructCaskBase;

//_____________________________________________________________________________________________________________________________

template< typename X> 
struct Cv_DefaultTraits  : public Cv_TypeCaskBase
{ 
    enum  {     ParamSize = 0 };

    typedef X       ObjType;         // ObjType may be different from Type  for ObjDef RefPtr

    void                ParamScan( Cv_TypeBouquet *bouquet, const void *thisPtr) {}

    bool                IsParamTrivial( void) const { return true; }

    void                PostScan( Cv_TypeBouquet *bouquet, const void *thisPtr) {}

template< typename T> 
    int32_t             CompareData( const T &a) const {   return 0; }
     
template <typename T, typename = void>
    static uint8_t                         *CreateNewObject(  T *type = nullptr) 
    { 
        T           *t = new T();
        return ( uint8_t *) t; 
    }

template <typename T, typename Cv_TypeEngage::Same< T, typename T::NonInstanstiable>::Note>
    static uint8_t                         *CreateNewObject( typename T::NonInstanstiable *type = nullptr) { return nullptr; }

template <typename T, typename = void>
    static void                             Delete(  T *t) 
    { 
        delete t; 
    }

template< typename T>
    static bool         SerializeObj( Cv_AttribValueReceptor *receptor, const T *thisPtr) 
    { 
        return false;
    }

};

//_____________________________________________________________________________________________________________________________

class  Cv_TypeCaskData   : public Cv_TypeCaskBase
{
public:
    typedef Cv_SRefRole< Cv_TypeCaskData> RefRole;

protected:
    Cv_CStr                     m_Name;

public:
    Cv_TypeCaskData( void)
    {}

    Cv_TypeCaskData( const Cv_CStr &name)
        : m_Name( name)
    {}

    const Cv_CStr           &Name( void) const { return m_Name; }

    void                    SetName( const Cv_CStr &str) { m_Name = str; }

    int32_t                 Compare( const Cv_TypeCaskData &a) const
    {  
        int32_t      res = m_Name.Compare( a.m_Name);
        return res ? ( res < 0 ? -1 : 1) : 0; 
    }

    static Cv_TypeCaskData  *FindStaticInstance( Cv_TypeCaskData *cd);

    friend std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeCaskData &cd)
    {
        ostr << "Cask[ " << cd.m_Name << "] ";
        return ostr;
    }
};

//_____________________________________________________________________________________________________________________________

class Cv_TypeCask : public Cv_Link< Cv_TypeCask>, public Cv_TypeCaskData
{
public:
    struct DerivedCask
    {
        Cv_TypeCaskData         *m_Deriv;
        Cv_CStr                 m_Name;
        uint32_t                m_BOffset;

        DerivedCask( Cv_TypeCaskData *deriv = nullptr, const char *name = nullptr, uint32_t  off = 0)
            : m_Deriv( deriv), m_Name( name), m_BOffset( off)
        {}
    };
    
     

protected:

    uint32_t                    m_Id;
    std::vector< DerivedCask>   *m_DerivedCasks;

public:
    struct DerivedCaskLessTypeInfo
    {
        bool    operator()( const DerivedCask &c1, const DerivedCask &c2)
        {
            return c1.m_Deriv->Name() < c2.m_Deriv->Name();
        }
    };

    
    typedef Cv_TypeCask             NonInstanstiable;

    
    Cv_TypeCask( void);

    virtual ~Cv_TypeCask( void);


    virtual Cv_CStr                 TypeString( void) const = 0;

    virtual const std::type_info    *TypeInfo( void) const = 0;

    virtual Cv_TypeCasket           *CreateCasket( Cv_TypeBouquet *bouquet, const void *thisPtr) = 0;

    virtual uint8_t                 *CreateObject( void) = 0;

    const std::vector< DerivedCask> &DerivedCasks( void) const { return *m_DerivedCasks; }
    void                            AddDerivedCasks( const DerivedCask &dc) {   m_DerivedCasks->push_back( dc); }
    uint32_t                        FindDerivedCask(  Cv_TypeCask *cask);

    virtual void                    DoInitialize( void) ;

    virtual void                    RegisterDerivedToChildrenBases( Cv_TypeCaskData  *derived, uint16_t off)  = 0;
    virtual void                    Freeze( void);

    virtual bool                    IsFlexi( Cv_TypeCasket *casket) const = 0;
    virtual bool                    IsFixed( void) const = 0;
    virtual uint32_t                GetCasketId( Cv_TypeCasket *casket) const = 0;

    virtual int32_t                 CompareData( const Cv_TypeCasket &c1, const Cv_TypeCasket &c2) const = 0;

    virtual uint32_t                Size( Cv_TypeCasket *casket)  = 0;

    virtual bool                    IsParamTrivial( Cv_TypeCasket *casket)  = 0;

    virtual void                    PostScan( Cv_TypeCasket *casket, const void *thisPtr, Cv_TypeBouquet *bouquet)  = 0;

    virtual std::string             StructString( Cv_TypeBouquet *bouquet, Cv_TypeCasket *casket) = 0;

    virtual bool                    StoreObject( Cv_OutDocket *outDocket, Cv_TypeCasket *casket, const void *thisPtr) const = 0;
    virtual bool                    RestoreObject( Cv_InDocket *inDoc, Cv_TypeCasket *casket, void *thisPtr) const = 0;

    virtual bool                    SerializeObj( Cv_AttribValueReceptor *receptor, Cv_TypeCasket *casket, const void *thisPtr) = 0;
    
    virtual bool                    StoreDefinition( Cv_OutSourceDocket *srcDoc, Cv_TypeCasket *casket) const = 0; 
    
    virtual std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, Cv_TypeCasket *casket, const void *thisPtr) const = 0;

    virtual Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base, Cv_TypeCasket *derived)  = 0;

    struct LessTypeInfo
    {
        bool    operator()( const Cv_TypeCask *c1, const Cv_TypeCask *c2)
        {
            return c1->TypeInfo()->before( *c2->TypeInfo());
        }

        bool    operator()( const Cv_TypeCask *c1, const std::type_info *c2)
        {
            return c1->TypeInfo()->before( *c2);
        }

        bool    operator()( const std::type_info *c1, const Cv_TypeCask *c2)
        {
            return c1->before( *c2->TypeInfo());
        }

    };

    
};

//_____________________________________________________________________________________________________________________________

class Cv_TypeCaskMgr : public Cv_LinkStack< Cv_TypeCask>, public Cv_TypeCaskBase
{
    uint32_t                            m_Size;
    std::vector< Cv_TypeCaskData *>     *m_TypeCasks;
    std::vector< Cv_TypeCask *>         *m_TypeInfoCasks;

    Cv_TypeCaskMgr( void);
    ~Cv_TypeCaskMgr( void);

    static Cv_TypeCaskMgr   *PrivSingleton( void);
    void                    DoProcess( void);

public:
    static Cv_TypeCaskMgr   *Singleton( void);

    Cv_TypeCask             *Find( const std::type_info *typeInfo) const;
    Cv_TypeCask             *Find( Cv_TypeCaskData *typeInfo) const;

friend class Cv_TypeCask;
};


//_____________________________________________________________________________________________________________________________

class Cv_TypeCasket  :  public Cv_Minion< Cv_TypeCaskData >, public Cv_TypeCaskBase
{
public:
    typedef Cv_TypeCasket        NonInstanstiable;

    Cv_TypeCasket( Cv_TypeCask *cask)
        : Cv_Minion< Cv_TypeCaskData>( cask)
    {} 

    ~Cv_TypeCasket( void)
    {}

    bool                    IsFlexi( void) { return Master()->IsFlexi( this); } 
    uint32_t                GetId( void)  { return Master()->GetCasketId( this); } 

    Cv_TypeCask             *Master( void) const { return static_cast< Cv_TypeCask *>( GetOwner()); }

    int32_t                 Compare( const Cv_TypeCasket &casket) const
    { 
        if ( GetOwner() != casket.GetOwner()) 
            return GetOwner()->Compare( *casket.GetOwner());
        return Master()->CompareData( SELF, casket); 
    }

    uint32_t                Size( void) { return Master()->Size( this); } 

    bool                    IsParamTrivial( void) { return Master()->IsParamTrivial( this); }  
 
    void                    ParamScan( Cv_TypeBouquet *bouquet, const void *thisPtr) {}

    void                    PostScan( Cv_TypeBouquet *bouquet, const void *thisPtr) { Master()->PostScan( this, thisPtr, bouquet); } 

    std::string             StructString( Cv_TypeBouquet *bouquet) { return Master()->StructString( bouquet, this); }

    bool                    StoreObject( const void *thisPtr, Cv_OutDocket *outDocket)
        {   return Master()->StoreObject( outDocket, this, thisPtr); }

    bool                    RestoreObject( void *thisPtr, Cv_InDocket *inDoc)
        {   return Master()->RestoreObject( inDoc, this, thisPtr); }


    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, const void *thisPtr)
        {   return Master()->SerializeObj( receptor, this, thisPtr); }

    bool                    StoreDefinition( Cv_OutSourceDocket *sourceDoc)
        {   return Master()->StoreDefinition( sourceDoc, this); }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const void *thisPtr) 
        {   return Master()->CollectSubElements( xDocket, this, thisPtr); }

    struct FlexiLess
    {
        bool operator ()( Cv_TypeCasket *c1, Cv_TypeCasket *c2)  { return  c1->IsFlexi() < c2->IsFlexi(); }
    };

    Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base) 
        {   return Master()->BaseRecOffset( bouquet, base, this); }
};

//_____________________________________________________________________________________________________________________________

struct Cv_FixedCaskAssist
{

template < typename X>
    std::string         ToString( const X &x)   {  return Cv_Aid::ToString( x); }
    std::string         ToString( const uint8_t &x)   {  return Cv_Aid::ToString( uint32_t ( x)); }

};

//_____________________________________________________________________________________________________________________________

template < class X>
class Cv_FixedCask  : public Cv_TypeCask, public Cv_TypeCasket, public Cv_FixedCaskAssist
{
public:
    typedef X       Type;

    Cv_FixedCask( void)
        : Cv_TypeCask( ), Cv_TypeCasket( this)
    {}

    uint32_t                GetCasketId( Cv_TypeCasket *casket) const { return CV_UINT32_MAX; }
    
    bool                    IsFlexi( Cv_TypeCasket *casket) const { return false; }
    bool                    IsFixed( void) const { return true; }

    const std::type_info    *TypeInfo( void) const { static const std::type_info    *classinfo = &typeid( X); return classinfo; }

    Cv_TypeCask             *Master( void) const { return const_cast< Cv_FixedCask< X> *>( this); }

    uint8_t                 *CreateObject( void) { return ( uint8_t *)(  new X()); }

    Cv_TypeCasket           *CreateCasket( Cv_TypeBouquet *bouquet, const void *thisPtr) { return this; }

    void                    RegisterDerivedToChildrenBases( Cv_TypeCaskData  *derived, uint16_t off) { return ; }

    int32_t                 CompareData( const Cv_TypeCasket &c1, const Cv_TypeCasket &c2) const        
                                {   return &c1 != &c2 ? ( &c1 < &c2 ? -1 : 1) : 0; }

    uint32_t                Size( Cv_TypeCasket *casket) { return sizeof( X); }

    bool                    IsParamTrivial( Cv_TypeCasket *casket) { return true; }


    void                    PostScan( Cv_TypeCasket *casket, const void *thisPtr, Cv_TypeBouquet *bouquet) {}

    std::string             StructString( Cv_TypeBouquet *bouquet, Cv_TypeCasket *casket) { return TypeString(); }
    
    bool                    StoreObject( Cv_OutDocket *outDocket, Cv_TypeCasket *casket, const void *thisPtr) const { return outDocket->Spritz()->Write( thisPtr, sizeof( X)); }
    bool                    RestoreObject( Cv_InDocket *inDoc, Cv_TypeCasket *casket, void *thisPtr) const { return inDoc->Spritz()->Read( thisPtr, sizeof( X)); }

    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, Cv_TypeCasket *casket, const void *thisPtr) 
    {
        return receptor->Accept(  ToString( *( const X *) thisPtr));
    }
     
    bool                    RestoreObject( void *thisPtr, Cv_InDocket *inDoc) const { return RestoreObject( inDoc, nullptr, thisPtr); }

    bool                    StoreDefinition( Cv_OutSourceDocket *srcDoc, Cv_TypeCasket *casket) const { return true; } 

    Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base, Cv_TypeCasket *derived) { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, Cv_TypeCasket *casket, const void *thisPtr) const 
                                        {  return std::vector< Cv_TypeElement>();  }
};



#define CV_CASK_FIXED( X)                                                                                                       \
                                                                                                                                \
template < typename Y>                                                                                                          \
struct  Cv_FixedCask##X : public  Cv_FixedCask< X>                                                                              \
{                                                                                                                               \
    Cv_FixedCask##X( void)                                                                                                      \
        : Cv_FixedCask< X>( )                                                                                                   \
    {}                                                                                                                          \
                                                                                                                                \
    Cv_CStr                 TypeString( void) const { return CV_MAKESTR( X); }                                                  \
                                                                                                                                \
    static Cv_FixedCask##X< Y>    s_Cask;                                                                                       \
                                                                                                                                \
};                                                                                                                              \
                                                                                                                                \
template < typename Y>                                                                                                          \
Cv_FixedCask##X< Y>     Cv_FixedCask##X< Y>::s_Cask;                                                                            \
                                                                                                                                \
inline Cv_TypeCask *cv_fetch_cask( const X *x)                                                                                  \
{                                                                                                                               \
    return &Cv_FixedCask##X< void>::s_Cask;                                                                                     \
}

//_____________________________________________________________________________________________________________________________

CV_CASK_FIXED( char)
CV_CASK_FIXED( float)
CV_CASK_FIXED( double)

CV_CASK_FIXED( int8_t)
CV_CASK_FIXED( int16_t)
CV_CASK_FIXED( int32_t)
CV_CASK_FIXED( int64_t)

CV_CASK_FIXED( uint8_t)
CV_CASK_FIXED( uint16_t)
CV_CASK_FIXED( uint32_t)
CV_CASK_FIXED( uint64_t)

//_____________________________________________________________________________________________________________________________

class  Cv_ParamCasket  : public Cv_TypeCasket
{
protected:
    uint32_t                m_Id;
    std::vector< uint8_t>   m_Data;

public:
    typedef Cv_BRefRole< Cv_ParamCasket>    RefRole;

    Cv_ParamCasket( void)
        : Cv_TypeCasket( nullptr), m_Id( uint32_t( -1))
    {}

    Cv_ParamCasket( Cv_TypeCask *cask, uint32_t sz)
        : Cv_TypeCasket( cask), m_Id( uint32_t( -1)), m_Data( sz)
    {}

    uint32_t        GetId( void) const { return m_Id; }
    void            SetId( uint32_t id) { m_Id = id; }

template < typename X>
    X               *At( uint32_t k) { return reinterpret_cast< X *>( &m_Data[ k]); }

    void            *Data( void) { return &m_Data[ 0]; }

    void            EnsureSize( uint64_t sz) {  m_Data.clear(); m_Data.resize( sz, 0); }


    friend std::ostream     &operator<<( std::ostream &ostr, const Cv_ParamCasket &pc)
    {
        ostr << "PC[ " << pc.m_Id << ", " << *pc.GetOwner() << "] ";
        if ( pc.m_Data.size())
            ostr << "Data[ " << Cv_Aid::PrintBytes( &pc.m_Data[ 0], pc.m_Data.size()) << "] ";
        return ostr;
    }
};

//_____________________________________________________________________________________________________________________________

class Cv_BasicCasketAdaptorIfc
{
public:
    bool                    IsFlexi( void) const;

    void                    ParamScan( Cv_TypeBouquet *bouquet, const void *thisPtr);

    int32_t                 CompareData( const Cv_BasicCasketAdaptorIfc &a) const;

    uint32_t                Size( void);

    bool                    IsParamTrivial( void) const;

    std::string             StructString( Cv_TypeBouquet *bouquet); 

    void                    PostScan( Cv_TypeBouquet *bouquet, const void *thisPtr);

    bool                    SaveObject( Cv_OutDocket *outDocket, const void *thisPtr) const;

    bool                    LoadObject( Cv_TypeBouquet *bouquet, void *thisPtr, Cv_FileSpritz *source) const;

    bool                    SaveDefinition( Cv_OutSourceDocket *srcDoc) const;

    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, const void *thisPtr);

    Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base);
};

//_____________________________________________________________________________________________________________________________

template < class Adaptor>
class Cv_BasicCasket  : public Cv_ParamCasket
{
public:
    typedef Cv_BasicCasket< Adaptor>                    ThisClass;
    typedef typename Adaptor::Type                      Type;
    
    enum {
        ParamSize = Adaptor::ParamSize
    };

    Cv_BasicCasket( Cv_TypeCask  *master)
        :  Cv_ParamCasket( master, ParamSize)
    {}

    void                    ParamScan( Cv_TypeBouquet *bouquet, const void *thisPtr) 
    {    
        Adaptor( this).ParamScan( bouquet, static_cast< const Type *>( thisPtr)); 
    }

    int32_t                 CompareData( const Cv_BasicCasket< Adaptor> &a) const
    {
        if ( m_Data.size() !=  a.m_Data.size())
            return ( m_Data.size() <  a.m_Data.size()) ? -1 : 1;
        return m_Data.size() ? memcmp( &m_Data[ 0], &a.m_Data[ 0], m_Data.size()) : 0; 
    }

    uint32_t                Size( void)  { return Adaptor( this).Size(); }

    void                    PostScan( Cv_TypeBouquet *bouquet, const void *thisPtr)  { Adaptor( this).PostScan( bouquet, static_cast< const Type *>( thisPtr)); }

    std::string             StructString( Cv_TypeBouquet *bouquet) { return Adaptor( this).StructString( bouquet); }

    bool                    IsParamTrivial( void) const { return Adaptor( this).IsParamTrivial(); }

    Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base) { return Adaptor( this).BaseRecOffset( bouquet, base); }

    bool                    IsFlexi( void) const { return Adaptor( this).IsFlexi(); }

    bool                    StoreObject( const void *thisPtr, Cv_OutDocket *outDocket) const
    {
        const Type  *pThis = static_cast< const Type *>( thisPtr);
        uint32_t    offset = ( uint32_t) outDocket->Spritz()->Offset();
        Cv_TypeUsher::MapObjectDefToOffset( outDocket, pThis, offset);
        outDocket->Spritz()->SetOffset( offset);
        return Adaptor( this).SaveObject( outDocket, pThis);
    }

    bool                    RestoreObject( void *thisPtr, Cv_InDocket *inDoc) const
    {
        Type        *pThis = static_cast< Type *>( thisPtr);
        uint32_t    offset = ( uint32_t) inDoc->Spritz()->Offset();
        bool        rslt = Adaptor( this).LoadObject( inDoc, pThis);
        Cv_TypeUsher::MapOffsetToObjectDef( inDoc, offset, pThis);
        return rslt;
    }

    bool             SerializeObj( Cv_AttribValueReceptor *receptor, const void *thisPtr) 
    {
        const Type  *pThis = static_cast< const Type *>( thisPtr);
        return Adaptor( this).SerializeObj( receptor, pThis);
    }
    
    bool            StoreDefinition( Cv_OutSourceDocket *outsrcDoc) const
    {   
        return Adaptor( this).SaveDefinition( outsrcDoc); 
    }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const void *thisPtr) const 
    { 
        return Adaptor( this).CollectSubElements( xDocket, static_cast< const Type *>( thisPtr)); 
    }

};

//_____________________________________________________________________________________________________________________________

template < class Adaptor>
class Cv_BasicCask : public Cv_TypeCask
{
public:
    typedef typename Adaptor::Type       Type;
    typedef Cv_BasicCask< Adaptor>       NonInstanstiable;

    Cv_BasicCask( void)
        : Cv_TypeCask()
    {}

    bool                    IsFlexi( Cv_TypeCasket *casket) const { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->IsFlexi(); } 
    bool                    IsFixed( void) const { return false; }
    uint32_t                GetCasketId( Cv_TypeCasket *casket) const { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->GetId(); }

    int32_t                 CompareData( const Cv_TypeCasket &casket1, const Cv_TypeCasket &casket2) const 
        {   return static_cast< const Cv_BasicCasket< Adaptor> &>( casket1).CompareData( static_cast< const Cv_BasicCasket< Adaptor> &>( casket2)); }

    uint32_t                Size( Cv_TypeCasket *casket) { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->Size(); }

    bool                    IsParamTrivial( Cv_TypeCasket *casket) { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->IsParamTrivial(); }

    void                    PostScan( Cv_TypeCasket *casket, const void *thisPtr, Cv_TypeBouquet *bouquet) 
                                {   return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->PostScan( bouquet, static_cast< const Type *>( thisPtr)); }

    const std::type_info    *TypeInfo( void) const { static const std::type_info    *classinfo = &typeid( Type); return classinfo; }

    void                    RegisterDerivedToChildrenBases( Cv_TypeCaskData  *derived, uint16_t off) { return ; }

    uint8_t                 *CreateObject( void) {  return Adaptor::CreateNewObject( ( Type *) nullptr); }

    Cv_TypeCasket           *CreateCasket( Cv_TypeBouquet *bouquet, const void *thisPtr)
    {
        Cv_BasicCasket< Adaptor >  *casket = new  Cv_BasicCasket< Adaptor>( this);
        casket->ParamScan( bouquet, static_cast< const Type *>( thisPtr));
        return casket;
    }

    std::string             StructString( Cv_TypeBouquet *bouquet, Cv_TypeCasket *casket) 
        { return  static_cast< Cv_BasicCasket< Adaptor> *>( casket)->StructString( bouquet); }

    bool                    StoreObject( Cv_OutDocket *outDocket, Cv_TypeCasket *casket, const void *thisPtr) const 
        { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->StoreObject( thisPtr, outDocket); }

    bool                    RestoreObject( Cv_InDocket *inDoc, Cv_TypeCasket *casket, void *thisPtr) const 
        { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->RestoreObject( thisPtr, inDoc); }


    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, Cv_TypeCasket *casket, const void *thisPtr)
        { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->SerializeObj( receptor, thisPtr); }

    bool                   StoreDefinition( Cv_OutSourceDocket *srcDoc, Cv_TypeCasket *casket) const 
        { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->StoreDefinition( srcDoc); }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, Cv_TypeCasket *casket, const void *thisPtr) const 
                                { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->CollectSubElements( xDocket, thisPtr); }

    Cv_TypeBouquet::BaseRec BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base, Cv_TypeCasket *casket) 
                                { return static_cast< Cv_BasicCasket< Adaptor> *>( casket)->BaseRecOffset( bouquet, base); }
};

//_____________________________________________________________________________________________________________________________

class  Cv_StructAdaptorBase : public Cv_TypeCaskBase 
{
protected:

    struct FieldCasketData
    {
        uint32_t        m_CasketCode;               //  Code for the casket from Bouquet
        uint32_t        m_FieldId;                  //  FieldId in corresponding Cask
        uint32_t        m_RecEnd;                   //  Offset from the Layout root

        FieldCasketData( void)
            : m_CasketCode( CV_UINT32_MAX), m_FieldId( 0), m_RecEnd( 0)
        {}

        int32_t             Compare( const FieldCasketData &a) const  
        { 
            if ( m_CasketCode != a.m_CasketCode)
                return m_CasketCode < a.m_CasketCode ? -1 : 1;
            return 0;
        }

        
    };

    Cv_ParamCasket     *m_ParamCasket;

    uint8_t             &FlexiFlg( void) const { return *m_ParamCasket->At< uint8_t>( 0); }

    uint8_t             &ParamTrivialFlg( void) const { return *m_ParamCasket->At< uint8_t>( 1); }

    uint32_t            &FieldSz( void) const { return *m_ParamCasket->At< uint32_t>( 2); }
    FieldCasketData     &FieldData( uint32_t k) const { return m_ParamCasket->At< FieldCasketData>( 6)[ k]; }

    uint32_t            RecOffset( uint32_t i) const {  return i ? FieldData( i -1).m_RecEnd : 0; }

public:
    Cv_StructAdaptorBase( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}

    Cv_ParamCasket      *ParamCasket( void) const { return  m_ParamCasket; }

    struct FieldCasket
    {
        Cv_TypeCasket   *m_FieldCasket;
        uint32_t        m_FieldId;

        FieldCasket( Cv_TypeCasket *casket = nullptr, uint32_t fieldId = 0)
            : m_FieldCasket( casket), m_FieldId( fieldId)
        {}

    int32_t         Compare( const FieldCasket &a) const  { return m_FieldCasket->Compare( *a.m_FieldCasket);}


        struct FlexiLess
        {
            bool operator ()( const FieldCasket &c1, const FieldCasket &c2)  { return  c1.m_FieldCasket->IsFlexi() < c2.m_FieldCasket->IsFlexi(); }
        };
    };

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, Cv_StructCaskBase *caskBase, const void *thisPtr) const ; 
};

//_____________________________________________________________________________________________________________________________

class  Cv_StructCaskBase
{
protected:
    enum FieldFlagBit
    {   
        FieldBaseBit =  15,
    };

    struct FieldCask
    {
        const char       *m_Name;                    // Name of field
        Cv_TypeCask      *m_Cask;                    // the field Cask
        uint16_t         m_FOffset;                  // offset from the struct root.
        uint16_t         m_Flags;                  // offset from the struct root.

        struct OffsetSignLess
        {
            bool    operator()( const FieldCask &f1, const FieldCask &f2)
            {
                return f1.m_Flags > f2.m_Flags;
            }
        };
    };

    

    Cv_Pack< FieldCask>                 m_FieldCasks;               
    
    typedef Cv_TypeCask::DerivedCask            DerivedCask;
    typedef Cv_StructAdaptorBase::FieldCasket   FieldCasket;

public:
    const char                  *FieldName( uint32_t i) const { return m_FieldCasks[ i].m_Name; }
    bool                        IsBase( uint32_t i) const { return Cv_Frager< Cv_StructCaskBase::FieldBaseBit, 1>( m_FieldCasks[ i].m_Flags).Get(); }

    const uint8_t               *FieldStructptr( const void *thisPtr, uint32_t i) const { return thisPtr ? ((const uint8_t *) thisPtr) + m_FieldCasks[ i].m_FOffset : nullptr; }
    uint8_t                     *FieldStructptr( void *thisPtr, uint32_t i) const { return thisPtr ? ((uint8_t *) thisPtr) + m_FieldCasks[ i].m_FOffset : nullptr; }

    std::vector< FieldCasket>   FetchSubCaskets( Cv_TypeBouquet *bouquet, const void *thisPtr);
        
    void                        ProcessFieldCasks( void) ;      // Move the bases to the begining.
};

//_____________________________________________________________________________________________________________________________

template < class X>
class  Cv_StructAdaptor : public Cv_DefaultTraits< X>, public Cv_StructAdaptorBase
{
public: 
    Cv_StructAdaptor( const Cv_ParamCasket *paramCasket)
        : Cv_StructAdaptorBase( paramCasket)
    {}

    typedef X           Type;

    bool                IsFlexi( void) const { return FlexiFlg() != 0; }

    bool                IsParamTrivial( void) const { return ParamTrivialFlg() != 0; }

    void                ParamScan( Cv_TypeBouquet *bouquet, const X *thisPtr)
    {
        Cv_StructCask< X>           *structCask = static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner());
        std::vector< FieldCasket>   fieldCaskets = structCask->FetchSubCaskets( bouquet, thisPtr);
        m_ParamCasket->EnsureSize( sizeof( FieldCasketData) * fieldCaskets.size() +6);
        FieldSz() = uint32_t( fieldCaskets.size()); 
        FlexiFlg() = fieldCaskets.size() ? fieldCaskets.back().m_FieldCasket->IsFlexi() : false;
        ParamTrivialFlg() = true;
        uint32_t    recSize = 0;
        for ( uint32_t i = 0; i < fieldCaskets.size(); ++i)
        {
            FieldCasketData     *fieldData = &FieldData( i);
            fieldData->m_FieldId = fieldCaskets[ i].m_FieldId;
            
            Cv_TypeCasket           *casket = fieldCaskets[ i].m_FieldCasket;
            if ( !casket->IsParamTrivial())
                ParamTrivialFlg() = false;
            recSize = fieldData->m_RecEnd  = recSize + casket->Size();
            //bouquet->LocateCaskIndex( casket->Master()), casket->GetId(); 
            fieldData->m_CasketCode = bouquet->CodeForCasket( casket);
            CV_ERROR_ASSERT( casket == bouquet->CasketForCode( fieldData->m_CasketCode))
        }
        return;   
    }

    uint32_t            Size( void)   { return FieldSz() ? FieldData( FieldSz() -1).m_RecEnd : 0; }

    void                PostScan( Cv_TypeBouquet *bouquet, const X *thisPtr)
    {
        return;
    }

    
    Cv_TypeBouquet::BaseRec     BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask *base)
    {
        Cv_StructCask< X>   *structCask = static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner());
        uint32_t            recSize = 0;
        for ( uint32_t i = 0; i <  FieldSz(); ++i)
        {
            FieldCasketData         *fieldData = &FieldData( i);
            if ( !structCask->IsBase( fieldData->m_FieldId) )
                continue;
            Cv_TypeCasket           *fieldCasket = bouquet->CasketForCode( fieldData->m_CasketCode);
            if ( fieldCasket->Master() == base)
                return Cv_TypeBouquet::BaseRec( fieldCasket, recSize);

            Cv_TypeBouquet::BaseRec    relBaseOff = fieldCasket->BaseRecOffset( bouquet, base);
            if ( relBaseOff.second != CV_UINT32_MAX)
            {
                relBaseOff.second += recSize;
                return relBaseOff;
            }
            recSize += fieldCasket->Size();        
        }
        return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX);
    }
    
    std::string             StructString( Cv_TypeBouquet *bouquet)
    {
        uint32_t                index = bouquet->CasketIndex( m_ParamCasket);

        if ( index == CV_UINT32_MAX)
            return m_ParamCasket->Master()->TypeString();
        std::stringstream       typeStrStream;
        typeStrStream << m_ParamCasket->Master()->TypeString() << index ;
        return typeStrStream.str();
    }

    bool                SaveObject( Cv_OutDocket *outDocket, const X *thisPtr)  const
    {
        Cv_StructCask< X>   *structCask = static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner());
        uint32_t            baseOffset = ( uint32_t) outDocket->Spritz()->Offset();
        for ( uint32_t j = 0; j <  FieldSz(); ++j)
        {
            uint32_t            i = FieldSz() -1 -j;
            FieldCasketData     *fieldData = &FieldData( i);
            outDocket->Spritz()->SetOffset( baseOffset + RecOffset( i));
            Cv_TypeCasket       *fieldCasket = outDocket->Bouquet()->CasketForCode( fieldData->m_CasketCode);
            const uint8_t       *fieldPtr = structCask->FieldStructptr( thisPtr, fieldData->m_FieldId) ;
            fieldCasket->StoreObject( fieldPtr, outDocket);
        }
        return true;
    }

    bool                LoadObject( Cv_InDocket  *inDocket, X *thisPtr) const
    {
        Cv_StructCask< X>   *structCask = static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner());
        uint32_t            baseOffset = ( uint32_t) inDocket->Spritz()->Offset();
        for ( uint32_t i = 0; i < FieldSz(); ++i )
        {
            FieldCasketData     *fieldData = &FieldData( i);
            inDocket->Spritz()->SetOffset( baseOffset + RecOffset( i));
            Cv_TypeCasket       *fieldCasket = inDocket->Bouquet()->CasketForCode( fieldData->m_CasketCode);
            uint8_t             *fieldPtr = structCask->FieldStructptr( thisPtr, fieldData->m_FieldId);
            fieldCasket->RestoreObject( fieldPtr, inDocket);
        }
        return true;
    }

    bool                    SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    {   
        Cv_StructCask< X>   *structCask = static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner());
        Cv_TypeBouquet      *bouquet = srcDoc->Bouquet();
        std::string         casketName = m_ParamCasket->StructString( bouquet);
        Cv_Twin< Cv_CStr>   lsplt =  Cv_Aid::LSplit( casketName, '[');
        Cv_SpritzLog        &spritzLog = srcDoc->SourceLog();
        
        if ( lsplt.Second().Size())
        {
            spritzLog << "template < typename X>                                                                                            \n";
            spritzLog << "struct " << Cv_Aid::CPPTypeStringCleanup( lsplt.First()) << ";                                                   \n";
            spritzLog << "                                                                                                                  \n";
            spritzLog << "template <>                                                                                                       \n";
        }

        spritzLog << "struct" << '\t' << Cv_Aid::CPPTypeStringCleanup( casketName) << "\n{";
        spritzLog.IncrMargin();
        for ( uint32_t i = 0; i < FieldSz(); ++i )
        {
            FieldCasketData     *fieldData = &FieldData( i);
            Cv_TypeCasket       *fieldCasket = bouquet->CasketForCode( fieldData->m_CasketCode);
            spritzLog << '\n' << Cv_Aid::CPPTypeStringCleanup( fieldCasket->StructString( bouquet))  << '\t' << structCask->FieldName( fieldData->m_FieldId) << ';';
        }
        spritzLog.DecrMargin();
        spritzLog << "\n};\n";

        spritzLog << "\n";
        return true; 
    }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const void *thisPtr) const
    {
        return Cv_StructAdaptorBase::CollectSubElements( xDocket, static_cast< Cv_StructCask< X> *>( m_ParamCasket->GetOwner()), thisPtr);
    }
}; 

//_____________________________________________________________________________________________________________________________

template < class X>
class  Cv_StructCask : public Cv_BasicCask< Cv_StructAdaptor< X> >, public Cv_StructCaskBase
{
public:
    

    typedef typename Cv_StructAdaptor< X>::FieldCasket      FieldCasket;
    typedef Cv_TypeCask::DerivedCask                        DerivedCask;
    typedef Cv_StructCask< X>                               NonInstanstiable;

public:
    typedef X       Type;

    Cv_StructCask( void)
        : Cv_BasicCask< Cv_StructAdaptor< X> >()
    {}
    
    void            RegisterDerivedToChildrenBases( Cv_TypeCaskData  *derived, uint16_t off)
    {
        // get all the bases and insert the struct offset into the base-cask.
        for ( FieldCask *fc = m_FieldCasks.Begin(); fc != m_FieldCasks.End(); ++fc)
        {
            if ( !Cv_Frager< FieldBaseBit, 1>( fc->m_Flags).Get())
                continue;

            DerivedCask         dc = DerivedCask( derived, fc->m_Name, fc->m_FOffset +off); 
            fc->m_Cask->AddDerivedCasks( dc);       
            fc->m_Cask->RegisterDerivedToChildrenBases( derived, fc->m_FOffset +off);
        }
        return;
    }    
};

//_____________________________________________________________________________________________________________________________

template < class Seq>
class Cv_SeqVector    : public Cv_DefaultTraits< Seq>
{
public:
    typedef Seq                                     Type;
    typedef typename Seq::value_type                Value;
    
protected:
    Cv_ParamCasket     *m_ParamCasket;

public:
    Cv_SeqVector( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}
   
    bool                IsFlexi( void) const { return true; }

    bool                IsParamTrivial( void) const { return true; }

    uint32_t            Size( void)  { return 2 * sizeof( uint32_t); }

    Cv_TypeCasket       *ValueCasket( Cv_TypeBouquet *bouquet) const
                                {   return  bouquet->LocateCasket( cv_TypeCask< Value>(), nullptr); }

    void                ParamScan( Cv_TypeBouquet *bouquet, const Seq *seq)
    {
        ValueCasket( bouquet);
        return;
    }

    void                PostScan( Cv_TypeBouquet *bouquet, const Seq *pSeq)  
    { 
        Cv_TypeCasket       *casket = ValueCasket( bouquet);
        for ( uint32_t i = 0; i < pSeq->size(); ++i)
            casket->ParamScan( bouquet, &pSeq->at( i)) ;
        return;
    }
    
    std::string         StructString( Cv_TypeBouquet *bouquet)
    {
        uint32_t                index = bouquet->CasketIndex( m_ParamCasket);

        std::stringstream       typeStrStream;
        typeStrStream << "Vector[ " << ValueCasket( bouquet)->StructString( bouquet) << "]";
        return typeStrStream.str();
    }

    bool               SaveObject( Cv_OutDocket *outDocket, const Seq *pSeq) const
    {
        Cv_Spritz           *sink = outDocket->Spritz();
        uint32_t            fileOffset = ( uint32_t) sink->Offset();

        if ( !pSeq || !pSeq->size())
        {
            sink->SetOffset( fileOffset  + 2 * sizeof( uint32_t));
            return true;
        }
        uint32_t            sz = uint32_t( pSeq->size());
        sink->Write( &sz, sizeof( uint32_t));
        uint32_t            dist = ( uint32_t) sink->Size() -fileOffset;
        sink->Write( &dist, sizeof( uint32_t));

        Cv_TypeBouquet      *bouquet = outDocket->Bouquet();
        Cv_TypeCasket       *casket = ValueCasket( bouquet);
        uint32_t            szPrimary = casket->Size();
        uint32_t            auxOffset = ( uint32_t) sink->EnsureSize( szPrimary * sz);
        for ( uint32_t i = 0; i < sz; ++i, auxOffset += szPrimary)
        {
            sink->SetOffset( auxOffset);
            casket->StoreObject( &pSeq->at( i), outDocket);
        }
        return true;
    }

    bool                LoadObject( Cv_InDocket  *inDocket, Seq *pSeq) const
    {
        Cv_TypeCasket       *casket = inDocket->Bouquet()->LocateCasket( cv_TypeCask< Value>(), nullptr);
   
        uint32_t            fileOffset = ( uint32_t) inDocket->Spritz()->Offset();
        uint32_t            sz = 0;
        inDocket->Spritz()->Read( &sz, sizeof( uint32_t));
        uint32_t            dist = 0;
        inDocket->Spritz()->Read( &dist, sizeof( uint32_t)); 
        if ( !sz)
            return true;
        pSeq->resize( sz);
        fileOffset += dist;

        
        uint32_t            szPrimary = casket->Size();

        for ( uint32_t i = 0; i < sz; ++i)
        {
            inDocket->Spritz()->SetOffset( fileOffset);
            casket->RestoreObject( &*(pSeq->begin() + i), inDocket);
            fileOffset += szPrimary;
        }
        return true;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    {      
        Cv_SpritzLog        &spritzLog = srcDoc->SourceLog();
        std::string         typeString = this->m_ParamCasket->StructString(  srcDoc->Bouquet());
        Cv_CStr             templateName = Cv_Aid::LSplit( typeString, '[').First();
        uint32_t            *pFlag = srcDoc->FlagForTemplate( templateName);

        if ( *pFlag)
            return true;
        *pFlag = true;

        spritzLog << "                                                                                                                      \n";
        spritzLog << "template < class X>                                                                                                   \n";   
        spritzLog << "struct  " << templateName << "                                                                                        \n";
        spritzLog << "{                                                                                                                     \n";
        spritzLog << "    uint32_t		m_Sz;                                                                                               \n";
        spritzLog << "    uint32_t		m_Dist;                                                                                             \n";
        spritzLog << "                                                                                                                      \n";
        spritzLog << "    uint32_t        Size( void) const { return m_Sz; }                                                                \n";
        spritzLog << "    const X         &operator[]( uint32_t k) const { return  ReCast< X>( ReCast< uint8_t>( this) + m_Dist)[ k]; }     \n";                
        spritzLog << "};                                                                                                                    \n";
        spritzLog << "                                                                                                                      \n";

        return true; 
    }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const Seq *pSeq) const
    {
        Cv_TypeCasket                       *casket = xDocket->Bouquet()->LocateCasket( cv_TypeCask< Value>(), nullptr);
        std::vector< Cv_TypeElement>        subElems( pSeq ? uint32_t( pSeq->size()) : 1);
        if ( !pSeq)
        {
            subElems[ 0] = Cv_TypeElement( casket->Master()->Name(), casket, nullptr);      // null element
            return subElems;
        }
        
        for ( uint32_t i = 0; i < subElems.size(); ++i)
            subElems[ i] = Cv_TypeElement( casket->Master()->Name(), casket, &pSeq->at( i));
        return subElems;
    } 

    Cv_TypeBouquet::BaseRec         BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask  *base) { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    struct Cask : public Cv_BasicCask< Cv_SeqVector< Seq> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_SeqVector< Seq> >()
        {}  

        Cv_CStr  TypeString( void) const
        {
            static  char    s_Buf[ 256];
            static  char    sz = 0;
            if ( sz)
                return Cv_CStr( &s_Buf[ 0], sz);
            sz = snprintf( &s_Buf[ 0], 256, "Vector[%s]", cv_TypeCask< Value>()->TypeString().CStr()); 
            return Cv_CStr( &s_Buf[ 0], sz);
        }
    };
    static Cask            s_Cask;
};

//_____________________________________________________________________________________________________________________________

template < typename X>
typename Cv_SeqVector< X>::Cask   Cv_SeqVector< X>::s_Cask;


template < typename X>
Cv_TypeCask         *cv_fetch_cask_Flex( const std::vector< X> *x)
{
    return &Cv_SeqVector< std::vector< X> >::s_Cask;
}

//_____________________________________________________________________________________________________________________________

template < class Seq>
class Cv_SeqIotaArray : public Cv_IotaArrayDef, public Cv_SeqVector< Seq>
{
public:
    typedef Cv_IotaArrayDef                         IotaDef;
    typedef Seq                                     Type;
    typedef typename Seq::value_type                Value;

protected:
    typedef typename std::tuple< BitWidth, uint32_t>     Parameters;  

    Parameters      &Params( void) const { return *( Parameters *) this->m_ParamCasket->Data(); }
     
public:
    Cv_SeqIotaArray( const Cv_ParamCasket *paramCasket)
        : Cv_SeqVector< Seq>( const_cast< Cv_ParamCasket *>( paramCasket))
    {}

    
    enum {      ParamSize = sizeof( Parameters), };

    typedef Cv_SeqVector< Seq>                  Base;


    bool                IsFlexi( void) const { return ( DSize() == CV_UINT32_MAX) ? Base::IsFlexi() : false; }

    bool                IsParamTrivial( void) const { return ( DSize() == CV_UINT32_MAX) ? Base::IsParamTrivial() : false; }

    std::string         StructString( Cv_TypeBouquet *bouquet)
    {
        if ( DSize() == CV_UINT32_MAX)
            return Base::StructString( bouquet);

        uint32_t                index = bouquet->CasketIndex( this->m_ParamCasket);
        std::stringstream       typeStrStream;
        typeStrStream << "IVector" << index << "[ " << this->ValueCasket( bouquet)->StructString( bouquet) << "]";
        return typeStrStream.str();
    }

    void                ParamScan( Cv_TypeBouquet *bouquet, const Seq *seq)
    {
        Params() = Parameters( BitWidth_Unknown, CV_UINT32_MAX);

        bool    useIotaFlg = false;
        if ( seq && seq->size())
        {
            BitWidth    wdIndex = BitWidthForValue( *std::max_element( seq->begin(), seq->end()));
            uint32_t    size = uint32_t( seq->size());
            if ( MemSize( size, wdIndex) < 1024)
                Params() = Parameters( wdIndex, size);
        }
        if ( DSize() == CV_UINT32_MAX)
            return Base::ParamScan( bouquet, seq);
        return;      
    }

    BitWidth            WdIndex( void) const { return std::get< 0>( Params()); }
    uint32_t            DSize( void) const { return std::get< 1>( Params()); }

    uint32_t            Size( void)  { return ( DSize() == CV_UINT32_MAX) ? Base::Size() : MemSize( DSize(), WdIndex()); }

    void                PostScan( Cv_TypeBouquet *bouquet, const Seq *pSeq) 
    {
        if ( DSize() == CV_UINT32_MAX)
            Base::PostScan( bouquet, pSeq);
        return;
    }

    bool                SaveObject( Cv_OutDocket *outDocket, const Seq *seq) const
    {
        if ( DSize() == CV_UINT32_MAX)
            return Base::SaveObject( outDocket, seq);

        uint32_t        memSz = MemSize( DSize(), WdIndex());
        uint8_t         *buf = ( uint8_t *) alloca( memSz);
        for ( uint32_t i = 0; i < seq->size(); ++i)
            IotaDef::SetAt( WdIndex(), buf, i, seq->at( i));
        outDocket->Spritz()->Write( buf, memSz);
        return true;
    }
    bool                LoadObject( Cv_InDocket  *inDocket, Seq *seq) const
    {
        if ( DSize() == CV_UINT32_MAX)
            return Base::LoadObject( inDocket, seq);
        
        uint32_t        memSz = MemSize( DSize(), WdIndex());
        uint8_t         *buf = ( uint8_t *) alloca( memSz);
        inDocket->Spritz()->Read( buf, memSz);
        seq->resize( DSize());
        for ( uint32_t i = 0; i < DSize(); ++i)
            seq->at( i) =  Value( IotaDef::GetAt( WdIndex(), buf, i));
        return true;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    {  
        if ( DSize() == CV_UINT32_MAX)
            return Base::SaveDefinition( srcDoc);
        
        std::string         typeString = this->m_ParamCasket->StructString(  srcDoc->Bouquet());
        std::string         valueTypeString = this->ValueCasket( srcDoc->Bouquet())->StructString( srcDoc->Bouquet());
        Cv_CStr             templateName = Cv_Aid::LSplit( typeString, '[').First();
        uint32_t            *pFlag = srcDoc->FlagForTemplate( templateName);
        Cv_SpritzLog        &spritzLog = srcDoc->SourceLog();

        if ( !*pFlag)
        {
            spritzLog << "template < class X>                                                           \n";   
            spritzLog << "struct  " << templateName << ";                                               \n";
            spritzLog << "                                                                                  \n";

        }
        *pFlag = true;
        typeString = Cv_Aid::CPPTypeStringCleanup( typeString);
        spritzLog << "template <>                                                                       \n";    
        spritzLog << "struct " << typeString << "                                                       \n";
        spritzLog << "{                                                                                 \n";
        spritzLog << "    typedef  " << valueTypeString << "               Value;                       \n";
        spritzLog << "                                                                                  \n";
        spritzLog << "    uint8_t       m_Data[ " << MemSize( DSize(), WdIndex()) << "];                \n";
        spritzLog << "                                                                                  \n";
        spritzLog << "    static const uint32_t BitSize = " <<  IotaDef::BitSize( WdIndex()) << ";      \n";
        spritzLog << "    static const uint64_t BitMask  = ( uint64_t( 1) << BitSize) -1;               \n";
        spritzLog << "                                                                                  \n";
        spritzLog << "    constexpr uint32_t    Size( void) const { return " <<  DSize() << "; }        \n";
        spritzLog << "    Value                 operator[]( uint32_t i) const { return Value( ( ReCast< uint64_t>( this)[ ( BitSize * i) / 64 ] >> ( BitSize * i) % 64 ) & BitMask); } \n";
        spritzLog << "                                                                                  \n";
        spritzLog << "};                                                                                \n";
        spritzLog << "                                                                                  \n";

        return true; 
    }
    
    struct Cask : public Cv_BasicCask< Cv_SeqIotaArray< Seq> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_SeqIotaArray< Seq> >()
        {}  

        Cv_CStr  TypeString( void) const 
        {
            static  char    s_Buf[ 256];
            static  char    sz = 0;
            if ( sz)
                return Cv_CStr( &s_Buf[ 0], sz);
            sz = snprintf( &s_Buf[ 0], 256, "IVector[%s]", cv_TypeCask< Value>()->TypeString().CStr()); 
            return Cv_CStr( &s_Buf[ 0], sz);
        }
    };
    static Cask            s_Cask;
};

//_____________________________________________________________________________________________________________________________

template < typename Seq>
typename Cv_SeqIotaArray< Seq>::Cask        Cv_SeqIotaArray< Seq>::s_Cask;

template < typename X>
Cv_TypeCask         *cv_fetch_caskImpl( const std::vector< X> *x, std::false_type)
{
    return &Cv_SeqVector< std::vector< X> >::s_Cask;
}

template < typename X>
Cv_TypeCask         *cv_fetch_caskImpl( const std::vector< X> *x, std::true_type)
{
    return &Cv_SeqIotaArray< std::vector< X>>::s_Cask;
}
 
template < typename X>
Cv_TypeCask         *cv_fetch_cask( const std::vector< X> *x)
{
//      return &Cv_SeqVector< std::vector< X> >::s_Cask;
        return cv_fetch_caskImpl( x, std::is_unsigned< X>());
}

//_____________________________________________________________________________________________________________________________

template < class X>
class  Cv_RefPtrAdaptor : public Cv_DefaultTraits< X>
{
protected:

    Cv_ParamCasket     *m_ParamCasket;
    uint32_t            &PtrCasketCode( void) const { return *( uint32_t *) this->m_ParamCasket->Data(); }

public:
    enum {      ParamSize = sizeof( uint32_t), };

    Cv_RefPtrAdaptor( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}

    typedef X           *Type;

    bool                IsFlexi( void) const  { return true;    }

    bool                IsParamTrivial( void) const { return PtrCasketCode() == 0; }

    uint32_t            Size( void) { return Cv_TypeUsher::RefPtrSize< X>();}

    Cv_TypeCasket       *PtrCasket( Cv_TypeBouquet *bouquet) const
                            {   return PtrCasketCode() ? bouquet->CasketForCode( PtrCasketCode()) : bouquet->LocateCasket( cv_TypeCask< X>(), nullptr); }

    std::string         StructString( Cv_TypeBouquet *bouquet) 
    { 
        std::stringstream   sstrm;
        sstrm << Cv_TypeUsher::RefPtrTemplateName< X>() << "[ " << PtrCasket( bouquet)->StructString( bouquet) << "]";
        return sstrm.str(); 
    }

    void                ParamScan( Cv_TypeBouquet *bouquet, const X *const *pObj)
    {
        if ( !pObj || !*pObj)
            return;
        Cv_TypeCask         *objCask = Cv_TypeCaskMgr::Singleton()->Find( &typeid( **pObj));
        Cv_TypeCasket       *objCasket = bouquet->LocateCasket( objCask, *pObj);
        
        //PtrCasketCode() = bouquet->CodeForCasket( objCasket);

        return;
    }

    void                PostScan( Cv_TypeBouquet *bouquet, const X *const *pObj)
    {
        if ( !*pObj)
            return;
        PtrCasket( bouquet)->ParamScan( bouquet, *pObj);
        return;
    }

    
    bool                SaveObject( Cv_OutDocket *outDocket, const X *const *pObj) const
    {
        Cv_TypeCasket       *storeCasket = PtrCasket( outDocket->Bouquet());
        return Cv_TypeUsher::MapObjectRefToOffset( outDocket, storeCasket, *pObj);
    }

    bool                LoadObject( Cv_InDocket *inDocket, X  **pObj) const
    {
        Cv_TypeCasket       *storeCasket = PtrCasket( inDocket->Bouquet());
        Cv_TypeUsher::MapOffsetToObjectRef( inDocket, storeCasket, pObj);
        return !!*pObj;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    {   
        Cv_TypeCasket       *storeCasket = PtrCasket( srcDoc->Bouquet());
        return Cv_TypeUsher::SaveRefSource< X>( storeCasket, srcDoc); 
    }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const X *const *pObj) const
    {
        Cv_TypeCasket       *storeCasket = PtrCasket( xDocket->Bouquet());
        return Cv_TypeUsher::CollectRefSubElements( xDocket, storeCasket, pObj ? *pObj : (( const X *) nullptr) );
    } 

    Cv_TypeBouquet::BaseRec     BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask  *base) { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    struct Cask : public Cv_BasicCask< Cv_RefPtrAdaptor< X> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_RefPtrAdaptor< X> >()
        {}  

        Cv_CStr     TypeString( void)  const
        {
            static  char    s_Buf[ 256];
            static  char    sz = 0;
            if ( sz)
                return Cv_CStr( &s_Buf[ 0], sz);
            sz = snprintf( &s_Buf[ 0], 256, "%s[%s]", Cv_TypeUsher::RefPtrTemplateName< X>(), cv_TypeCask< X>()->TypeString().CStr()); 
            return Cv_CStr( &s_Buf[ 0], sz);
        }
    };
    static Cask            s_Cask;
};

//_____________________________________________________________________________________________________________________________

template < typename X>
typename Cv_RefPtrAdaptor< X>::Cask   Cv_RefPtrAdaptor< X>::s_Cask;

template < typename X>
Cv_TypeCask     *cv_fetch_cask( X *const *x)
{
    return &Cv_RefPtrAdaptor< X>::s_Cask;
}

//_____________________________________________________________________________________________________________________________

template < class X>
class  Cv_PtrDefAdaptor : public Cv_DefaultTraits< X> 
{
    Cv_ParamCasket     *m_ParamCasket;
    
    uint32_t            &PtrCasketCode( void) const { return *( uint32_t *) this->m_ParamCasket->Data(); }

public:
    enum {      ParamSize = sizeof( uint32_t), };
 
    Cv_PtrDefAdaptor( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}

    typedef Cv_Ptr< X>                                 Type;

    bool                IsFlexi( void) const  { return true;    }

    bool                IsParamTrivial( void) const { return PtrCasketCode() == 0; }

    uint32_t            Size( void) { return Cv_TypeUsher::DefPtrSize< X>(); }
    
    Cv_TypeCasket       *PtrCasket( Cv_TypeBouquet *bouquet) const
                            {   return PtrCasketCode() ? bouquet->CasketForCode( PtrCasketCode()) : bouquet->LocateCasket( cv_TypeCask< X>(), nullptr); }

    std::string         StructString( Cv_TypeBouquet *bouquet) 
    { 
        std::stringstream   sstrm;
        sstrm << "DefPtr[ " << PtrCasket( bouquet)->StructString( bouquet) << "]";
        return sstrm.str(); 
    }

    void                ParamScan( Cv_TypeBouquet *bouquet, const Cv_Ptr< X> *pObj)
    {
        if ( !pObj || !*pObj)
            return;
        Cv_TypeCask         *objCask = Cv_TypeCaskMgr::Singleton()->Find( &typeid( **pObj));
        Cv_TypeCasket       *objCasket = bouquet->LocateCasket( objCask, *pObj);
        Cv_TypeCasket       *defCasket = bouquet->LocateCasket( cv_TypeCask< X>(), nullptr);
        if ( defCasket != objCasket)
            PtrCasketCode() = bouquet->CodeForCasket( objCasket);
        return;
    }
    void                PostScan( Cv_TypeBouquet *bouquet, const void *thisPtr)
    {

        const Cv_Ptr< X>    *pObj( static_cast< const Cv_Ptr< X> *>( thisPtr)); 
        if ( *pObj)
            PtrCasket( bouquet)->PostScan( bouquet, pObj->Ptr());
        return;
    }

    bool                SaveObject( Cv_OutDocket *outDocket, const Cv_Ptr< X> *pObj) const
    {
        Cv_TypeCasket       *storeCasket = PtrCasket( outDocket->Bouquet());
        return Cv_TypeUsher::StoreObjectAtOffset( outDocket, storeCasket, pObj->Ptr());
    }

    bool                LoadObject( Cv_InDocket *inDocket, Cv_Ptr< X> *pObj) const
    {
        X               *obj = nullptr;
        Cv_TypeCasket   *storeCasket = PtrCasket( inDocket->Bouquet());
        uint32_t        res = Cv_TypeUsher::RestoreObjectAtOffset( inDocket, storeCasket, &obj) ;
        pObj->SetPtr( const_cast< X *>( obj));
        return !!obj;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    {   
        Cv_TypeCasket       *storeCasket = PtrCasket( srcDoc->Bouquet());
        return  Cv_TypeUsher::SaveDefSource< X>( storeCasket, srcDoc); 
    }
    
    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const Cv_Ptr< X> *pObj) const
    {
        Cv_TypeCasket       *storeCasket = PtrCasket( xDocket->Bouquet());
        return Cv_TypeUsher::CollectSubElements( xDocket,  storeCasket, pObj ? pObj->Ptr() : (( const X *) nullptr));
    } 

    Cv_TypeBouquet::BaseRec     BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask  *base) 
                                                    { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    struct Cask : public Cv_BasicCask< Cv_PtrDefAdaptor< X> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_PtrDefAdaptor< X> >()
        {} 
        
        Cv_CStr  TypeString( void) const
        {
            static  char    s_Buf[ 256];
            static  char    sz = 0;
            if ( sz)
                return Cv_CStr( &s_Buf[ 0], sz);
            sz = snprintf( &s_Buf[ 0], 256, "DefPtr[%s]", cv_TypeCask< X>()->TypeString().CStr()); 
            return Cv_CStr( &s_Buf[ 0], sz);
        } 
    };
    static Cask            s_Cask;
    
};

//_____________________________________________________________________________________________________________________________

template < typename X>
typename Cv_PtrDefAdaptor< X>::Cask   Cv_PtrDefAdaptor< X>::s_Cask;

template < typename X>
Cv_TypeCask     *cv_fetch_cask( const Cv_Ptr< X> *x)
{
    return &Cv_PtrDefAdaptor< X>::s_Cask;
}

//_____________________________________________________________________________________________________________________________

template < typename X>
class Cv_CStrAdaptor :  public Cv_DefaultTraits< Cv_CStr>
{
    Cv_ParamCasket     *m_ParamCasket;

public:
    Cv_CStrAdaptor( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}
 
    typedef Cv_CStr     Type;

    bool                IsFlexi( void) const { return true; }

    bool                IsParamTrivial( void) const { return true; }

    uint32_t            Size( void)  { return 2 * sizeof( uint32_t); }
    
    std::string         StructString( Cv_TypeBouquet *bouquet) { return m_ParamCasket->Master()->TypeString(); }

    bool                SaveObject( Cv_OutDocket *outDocket, const Cv_CStr *pSeq) const
    {
        Cv_Spritz           *sink = outDocket->Spritz();
        uint32_t            fileOffset = ( uint32_t) sink->Offset();

        if ( !pSeq->Size())
        {
            sink->SetOffset( fileOffset  + 2 * sizeof( uint32_t));
            return true;
        }
        uint32_t            sz = pSeq->Size();
        uint32_t            auxOffset = ( uint32_t) outDocket->Spritz()->EnsureSize( sz);
        uint32_t            dist = auxOffset -fileOffset;
        sink->Write( &dist, sizeof( uint32_t));
        sink->Write( &sz, sizeof( uint32_t));
        sink->SetOffset( auxOffset);
        sink->Write( pSeq->CStr(), sz);
        return true;
    }

    bool                LoadObject( Cv_InDocket  *inDocket, Cv_CStr *pSeq) const
    {
        uint32_t            fileOffset = ( uint32_t) inDocket->Spritz()->Offset();
        uint32_t            dist = 0;
        inDocket->Spritz()->Read( &dist, sizeof( uint32_t));
        if ( !dist)
            return true;
        uint32_t            sz = 0;
        inDocket->Spritz()->Read( &sz, sizeof( uint32_t));
        inDocket->Spritz()->SetOffset( fileOffset + dist);
        char                *pStr = new char[ sz +1];
        pStr[ sz] = '\0';
        inDocket->Spritz()->Read( pStr, sz);       
        *pSeq = pStr;
        
        return true;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    { 
        Cv_SpritzLog        &spritzLog = srcDoc->SourceLog();
        
        spritzLog << "                                                                                                                          \n";
        spritzLog << "struct CStr                                                                                                               \n";
        spritzLog << "{                                                                                                                         \n";
	    spritzLog << "    uint32_t  m_Dist;                                                                                                     \n";
	    spritzLog << "    uint32_t  m_Sz;                                                                                                       \n";
        spritzLog << "                                                                                                                          \n";
        spritzLog << "    uint32_t        Size( void) const { return m_Sz; }                                                                    \n";
        spritzLog << "    char            *StrData( void) const { return  ReCast< char>( ReCast< uint8_t>( this) + m_Dist); }                   \n";
        spritzLog << "};                                                                                                                        \n";
        spritzLog << "                                                                                                                          \n";
       
        return true; 
    }
    
    Cv_TypeBouquet::BaseRec     BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask  *base) { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const void *thisPtr) const
    {
        return std::vector< Cv_TypeElement>();
    }

    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, const void *thisPtr) 
    {
        return receptor->Accept( *( const Cv_CStr*) thisPtr);
    }

    struct Cask : public Cv_BasicCask< Cv_CStrAdaptor< X> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_CStrAdaptor< X> >()
        {}  

        Cv_CStr                 TypeString( void) const { return  "CStr"; }
    };
    static Cask            s_Cask;
    
};

template < class X>
typename Cv_CStrAdaptor< X>::Cask     Cv_CStrAdaptor< X>::s_Cask;

inline Cv_TypeCask  *cv_fetch_cask( const Cv_CStr *x)
{
    return &Cv_CStrAdaptor< int>::s_Cask;
}


//_____________________________________________________________________________________________________________________________

template < typename X>
class Cv_StringAdaptor :  public Cv_DefaultTraits< Cv_CStr>
{
    Cv_ParamCasket     *m_ParamCasket;

public:
    Cv_StringAdaptor( const Cv_ParamCasket *paramCasket)
        : m_ParamCasket( const_cast< Cv_ParamCasket *>( paramCasket))
    {}
 
    typedef std::string     Type;

    bool                IsFlexi( void) const { return true; }

    bool                IsParamTrivial( void) const { return true; }

    uint32_t            Size( void)  { return 2 * sizeof( uint32_t); }
    
    std::string         StructString( Cv_TypeBouquet *bouquet) { return m_ParamCasket->Master()->TypeString(); }

    bool                SaveObject( Cv_OutDocket *outDocket, const std::string *pSeq) const
    {
        Cv_Spritz           *sink = outDocket->Spritz();
        uint32_t            fileOffset = ( uint32_t) sink->Offset();

        if ( !pSeq->size())
        {
            sink->SetOffset( fileOffset  + 2 * sizeof( uint32_t));
            return true;
        }
        uint32_t            sz = uint32_t( pSeq->size());
        uint32_t            auxOffset = ( uint32_t) outDocket->Spritz()->EnsureSize( sz);
        uint32_t            dist = auxOffset -fileOffset;
        sink->Write( &dist, sizeof( uint32_t));
        sink->Write( &sz, sizeof( uint32_t));
        sink->SetOffset( auxOffset);
        sink->Write( pSeq->c_str(), sz);
        return true;
    }

    bool                LoadObject( Cv_InDocket  *inDocket, std::string *pSeq) const
    {
        uint32_t            fileOffset = ( uint32_t) inDocket->Spritz()->Offset();
        uint32_t            dist = 0;
        inDocket->Spritz()->Read( &dist, sizeof( uint32_t));
        if ( !dist)
            return true;
        uint32_t            sz = 0;
        inDocket->Spritz()->Read( &sz, sizeof( uint32_t));
        inDocket->Spritz()->SetOffset( fileOffset + dist);
        char                *pStr = new char[ sz +1];
        pStr[ sz] = '\0';
        inDocket->Spritz()->Read( pStr, sz);       
        *pSeq = pStr;
        delete [] pStr;
        return true;
    }

    bool                SaveDefinition( Cv_OutSourceDocket *srcDoc) const
    { 
        Cv_SpritzLog        &spritzLog = srcDoc->SourceLog();
        
        spritzLog << "                                                                                                                          \n";
        spritzLog << "struct String                                                                                                               \n";
        spritzLog << "{                                                                                                                         \n";
	    spritzLog << "    uint32_t  m_Dist;                                                                                                     \n";
	    spritzLog << "    uint32_t  m_Sz;                                                                                                       \n";
        spritzLog << "                                                                                                                          \n";
        spritzLog << "    uint32_t        Size( void) const { return m_Sz; }                                                                    \n";
        spritzLog << "    char            *StrData( void) const { return  ReCast< char>( ReCast< uint8_t>( this) + m_Dist); }                   \n";
        spritzLog << "};                                                                                                                        \n";
        spritzLog << "                                                                                                                          \n";
       
        return true; 
    }
    
    Cv_TypeBouquet::BaseRec     BaseRecOffset( Cv_TypeBouquet *bouquet, Cv_TypeCask  *base) { return Cv_TypeBouquet::BaseRec( nullptr, CV_UINT32_MAX); }

    std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, const void *thisPtr) const
    {
        return std::vector< Cv_TypeElement>();
    }

    bool                    SerializeObj( Cv_AttribValueReceptor *receptor, const void *thisPtr) 
    {
        return receptor->Accept( *( const std::string*) thisPtr);
    }

    struct Cask : public Cv_BasicCask< Cv_StringAdaptor< X> >     
    {
        Cask( void)
            :   Cv_BasicCask< Cv_StringAdaptor< X> >()
        {}  

        Cv_CStr         TypeString( void) const { return  "String"; }
    };

    static Cask            s_Cask;
};

template < class X>
typename Cv_StringAdaptor< X>::Cask     Cv_StringAdaptor< X>::s_Cask;

inline Cv_TypeCask  *cv_fetch_cask( const std::string *x)
{
    return &Cv_StringAdaptor< int>::s_Cask;
}


//_____________________________________________________________________________________________________________________________

template < class T>
Cv_TypeCask     *cv_TypeCask( void)
{
    return cv_fetch_cask( ( const T *) nullptr);
}

//_____________________________________________________________________________________________________________________________

