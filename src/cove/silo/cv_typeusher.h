//_______________________________________________ cv_typeusher.h ______________________________________________________________
#pragma once

#include	"cove/barn/cv_aid.h"
#include    "cove/silo/cv_typebouqet.h"

//_____________________________________________________________________________________________________________________________
// Role are asigned by defining a typedef member of class Role.
// Default reference Role. Defined implicity

template < typename Type>
struct Cv_RefRole  
{
    enum   {  BaseFlg = false,  DerivedFlg = false, StaticFlg = false, };
    
    typedef void            BaseType;

    static Cv_TypeCask      *BaseCask( void) { return nullptr; } 
    static Type             *FindStaticInstance( Type *t) { return nullptr; }
};

//_____________________________________________________________________________________________________________________________
// Base reference Role. Defined explicitly in the Base class

template < typename Type>
struct Cv_BRefRole : public Cv_RefRole< Type>
{
    enum   {  BaseFlg = true, };

    typedef Type            BaseType;
    
    static Cv_TypeCask      *BaseCask( void) { return cv_TypeCask< BaseType>(); } 
};

//_____________________________________________________________________________________________________________________________
// Indexed reference Role. Defined explicitly in the Base class : all instances of Type are static 

template < class Type>
struct Cv_IRefRole : public Cv_BRefRole< Type>
{
    enum   {  IndexFlg = true, };

    typedef void    IndexNote;
};

//_____________________________________________________________________________________________________________________________
// Static reference Role. Defined explicitly in the Base class : all instances of Type are static 

template < class Type>
struct Cv_SRefRole : public Cv_BRefRole< Type>
{
    enum   {  StaticFlg = true, };

    typedef void    StaticNote;

    static Type     *FindStaticInstance( Type *t) { return static_cast< Type *>( Type::FindStaticInstance( t)); }
};

//_____________________________________________________________________________________________________________________________
// Ushers are used to deduce operational difference in handling the instances of classes with various roles
// Default RoleUsher for any type, except for Types for which RefRole is defined in it or in its Base class

template < typename Type, typename = void>
struct  Cv_RoleUsher : public Cv_RefRole< Type>
{
};

//_____________________________________________________________________________________________________________________________
// RoleUsher when RefRole is explicity defined and the Type itself is the Base

template < typename Type>
struct  Cv_RoleUsher< Type, typename Cv_TypeEngage::Same< Type, typename Type::RefRole::BaseType>::Note> : public Type::RefRole
{
};

//_____________________________________________________________________________________________________________________________
// RoleUsher when role is explicity defined in its Base and is Derived 

template < typename Type>
struct  Cv_RoleUsher< Type, typename Cv_TypeEngage::NotSame< Type, typename Type::RefRole::BaseType>::Note> : public Type::RefRole
{
    enum   {  DerivedFlg = true, };
};

//_____________________________________________________________________________________________________________________________

struct Cv_TypeUsher : public Cv_TypeCaskBase
{
    static Cv_TypeCask      *TypeInfoCask( const std::type_info  *ti);
    static Cv_CStr          TypeString( Cv_TypeCasket *paramCasket);
    static bool             StoreAtOffset( Cv_OutDocket *outDocket, Cv_TypeCask *baseCask, Cv_TypeCasket *storeCasket, Cv_TypeCasket *objCasket,  const void *pObj);
    static uint32_t         RestoreAtOffset( Cv_InDocket *inDoc, Cv_TypeCask *baseCask, Cv_TypeCasket *storeCasket, void **pObj); 

    static bool             SaveCasketDefSource( Cv_TypeCask *rootCask, Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc);
    static bool             SaveCasketRefSource( Cv_TypeCask *rootCask, Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc);

template < typename Type>
    static Cv_TypeCask     *RootCask( void)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;
        return RoleUsher::BaseFlg ? RoleUsher::BaseCask() : cv_TypeCask< Type>();
    }
    
    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static void             TypeDump( const Type *pObj)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;
        CV_FNPASS(() << RoleUsher::BaseFlg << RoleUsher::DerivedFlg << RoleUsher::StaticFlg )        
    }
    
    //_____________________________________________________________________________________________________________________________
   
template < typename Type>
    static bool             StoreObjectAtOffset( Cv_OutDocket *outDocket, Cv_TypeCasket  *storeCasket, const Type *pObj)
    {   
        Cv_TypeCask         *objCask = TypeInfoCask( &typeid( *pObj));
        Cv_TypeCasket       *objCasket = outDocket->Bouquet()->LocateCasket( objCask, pObj);
        typedef Cv_RoleUsher< Type>     RoleUsher;
        if ( !RoleUsher::BaseFlg)
            return StoreAtOffset( outDocket, cv_TypeCask< Type>(), storeCasket, objCasket, pObj);
        return StoreAtOffset( outDocket, RootCask< Type>(), storeCasket, objCasket, pObj);
    }

    //_____________________________________________________________________________________________________________________________


template < typename Type>
    static uint32_t         RestoreObjectAtOffset_( Cv_InDocket *inDoc, Cv_TypeCasket *storeCasket, Type **pObj)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;
        if ( !RoleUsher::BaseFlg)
            return RestoreAtOffset( inDoc, cv_TypeCask< Type>(), storeCasket, ( void **) pObj);

        uint32_t                        off = RestoreAtOffset( inDoc, RoleUsher::BaseCask(), storeCasket, ( void **) pObj);
        return off;
    }

template < typename Type>
    static uint32_t         RestoreObjectAtOffset( Cv_InDocket *inDoc, Cv_TypeCasket *storeCasket, Type **pObj)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;

        uint32_t            objOff = RestoreObjectAtOffset_( inDoc, storeCasket, pObj);
        if ( !RoleUsher::StaticFlg)
            return objOff;

        Type                *t = *pObj;
        *pObj = static_cast< Type *>( RoleUsher::FindStaticInstance( t));
        delete t;
        inDoc->RegisterOffsetToObjectDef( cv_TypeCask< Type>(), objOff, *pObj);
        return objOff;
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static uint32_t         DefPtrSize( void) { return sizeof( uint32_t); }

    // propagate it to all waiting references to encode the offset at which the object is encoded, store for use by future references
template < typename Type>
    static void             MapObjectDefToOffset( Cv_OutDocket *outDocket, const Type *pObj, uint32_t off)   
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;
        if ( RoleUsher::BaseFlg && !RoleUsher::DerivedFlg)                              // register offset for object-pointer only when the call is for BaseType
            outDocket->RegisterObjectDefToOffset(  cv_TypeCask< Type>(),  pObj, off);   
        return;
    }

    // register newly create object to the offset at which it was located, store for translating all future offset refenences to the object pointer.
template < typename Type>
    static void             MapOffsetToObjectDef( Cv_InDocket *inDoc, uint32_t off, Type *pObj) // restore time   
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;                                      // register offset to newly created object, when the call is for BaseType
        if ( !RoleUsher::StaticFlg && RoleUsher::BaseFlg && !RoleUsher::DerivedFlg)     
            inDoc->RegisterOffsetToObjectDef( cv_TypeCask< Type>(), off, pObj);
        return;
    }

    //_____________________________________________________________________________________________________________________________


template < typename Type>
    static uint32_t         RefPtrSize( void) { return sizeof( uint32_t); }

template < typename Type>
    static bool             MapObjectRefToOffset( Cv_OutDocket *outDocket, Cv_TypeCasket *storeCasket, const Type *pObj)   
    {    
        typedef Cv_RoleUsher< Type>     RoleUsher;
        if ( !RoleUsher::BaseFlg)
            return Cv_TypeUsher::StoreObjectAtOffset( outDocket, storeCasket, pObj); 

        uint32_t            curOff = ( uint32_t) outDocket->Spritz()->Offset();
        if ( !pObj)
        {
            outDocket->Spritz()->FillNull( sizeof( uint32_t));
            return true;
        }
        
        struct DefOffListener : public Cv_OutDocket::DefOffListener, public Cv_TypeCaskBase
        {
            uint32_t        m_RefOff;

            DefOffListener( uint32_t refOff) : m_RefOff( refOff) {}

            void   DefCallback( Cv_Spritz *sink, uint32_t defOff)  
            {
                sink->SetOffset( m_RefOff);
                defOff -= m_RefOff;
                sink->Write( &defOff, sizeof( uint32_t));
            }                                           
        };
        outDocket->RegisterObjectRefToOffset( RoleUsher::BaseCask(),  static_cast< const typename RoleUsher::BaseType *>( pObj), new DefOffListener( curOff));
        return true;
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static uint32_t     MapOffsetToObjectRef( Cv_InDocket *inDoc, Cv_TypeCasket *storeCasket, Type **pObj)    
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;
        if ( !RoleUsher::BaseFlg)
            return Cv_TypeUsher::RestoreObjectAtOffset( inDoc, storeCasket, pObj);
        *pObj = nullptr;
        uint32_t            curOff = ( uint32_t) inDoc->Spritz()->Offset();
        uint32_t            objOff = 0;
        inDoc->Spritz()->Read( &objOff, sizeof( uint32_t));
        if ( !objOff)
            return true;
        objOff += curOff;
        Cv_TypeCask         *cask = cv_TypeCask< Type>(); 

        struct DefObjListener : public Cv_InDocket::DefObjListener, public Cv_TypeCaskBase
        {
            Type          **m_PObj;

            DefObjListener( Type **pObj) : m_PObj( pObj) {}

            void   RefCallback(  void *thisPtr)  
            {   
                *m_PObj = static_cast< Type *>( reinterpret_cast< typename RoleUsher::BaseType *>( thisPtr)); 
            }
        }; 
        inDoc->RegisterRefCallbackForObjectOff( RoleUsher::BaseCask(), objOff, new DefObjListener( pObj));
        return objOff;
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static std::vector< Cv_TypeElement>    CollectSubElements( Cv_Docket *xDocket, Cv_TypeCasket *objCasket, const Type *pObj) 
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;

        if ( !RoleUsher::BaseFlg)
        {
            Cv_TypeBouquet::DerivedCasket   dCasket = xDocket->Bouquet()->FetchDerivedCasketForObj( cv_TypeCask< Type>(), objCasket, pObj);
            std::vector< Cv_TypeElement>    subElems( 1, Cv_TypeElement( "Ptr", objCasket, ( const uint8_t *)( pObj) + dCasket.m_BOffset));
            return subElems;
        }

        Cv_TypeBouquet::DerivedCasket   dCasket = xDocket->Bouquet()->FetchDerivedCasketForObj( RoleUsher::BaseCask(), objCasket, pObj);
        
        const uint8_t       *ptr = ( const uint8_t *)pObj + dCasket.m_BOffset;

        std::vector< Cv_TypeElement>    subElems( 3);
        subElems[ 0] = Cv_TypeElement( "Casket", dCasket.CasketName());
        subElems[ 1] = Cv_TypeElement( "ID", Cv_Aid::ToString( xDocket->ObjectId( static_cast< const typename RoleUsher::BaseType *>( pObj))));
        subElems[ 2] = Cv_TypeElement( "Ptr", objCasket, ptr);
        return subElems;
    }
    
    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static std::vector< Cv_TypeElement>    CollectRefSubElements( Cv_Docket *xDocket, Cv_TypeCasket *objCasket, const Type *pObj)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;

        if ( !RoleUsher::BaseFlg)
            return Cv_TypeUsher::CollectSubElements( xDocket, objCasket, pObj);
        std::vector< Cv_TypeElement>    subElems( 1);
        subElems[ 0] = Cv_TypeElement( "IDRef", Cv_Aid::ToString( xDocket->ObjectId( static_cast< const typename RoleUsher::BaseType *>( pObj))));
        return subElems;
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static bool        SaveDefSource( Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc)
    {
        return SaveCasketDefSource( RootCask< Type>(), paramCasket, srcDoc);
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static bool        SaveRefSource( Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;

        if ( !RoleUsher::BaseFlg)
            return SaveCasketDefSource( RootCask< Type>(), paramCasket, srcDoc);
        return SaveCasketRefSource( RootCask< Type>(), paramCasket, srcDoc);
    }

    //_____________________________________________________________________________________________________________________________

template < typename Type>
    static const char *RefPtrTemplateName( void)
    {
        typedef Cv_RoleUsher< Type>     RoleUsher;

        if ( !RoleUsher::BaseFlg)
            return "DefPtr";
        return "RefPtr";
    }
};
//_____________________________________________________________________________________________________________________________
