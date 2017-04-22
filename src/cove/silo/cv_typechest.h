//_______________________________________________ cv_typechest.h _______________________________________________________________
#pragma once

//_____________________________________________________________________________________________________________________________

#include    "cove/silo/cv_typecask.h" 
#include	"cove/silo/cv_crate.h"

//_____________________________________________________________________________________________________________________________

#define CV_CASK_BEGIN( PAL, X)                                                                                              \
                                                                                                                            \
template < class Y>                                                                                                         \
struct PAL : public X                                                                                                       \
{                                                                                                                           \
    typedef  X              This;                                                                                           \
    typedef  PAL            Pal;                                                                                            \
    static Cv_CStr  TypeName( void) { return CV_MAKESTR(X);  }                                                              \

//_____________________________________________________________________________________________________________________________

#define CV_CASK_MEMBER_BEGIN()                                                                                              \
    struct Cask : public Cv_StructCask< This>                                                                               \
    {                                                                                                                       \
        Cask( void)                                                                                                         \
            : Cv_StructCask< This>()                                                                                        \
        {                                                                                                                   \
            static  FieldCask   entries[] =                                                                                 \
            {                                                                                                               \

//_____________________________________________________________________________________________________________________________
// Declare  Field : Params [ Name, Cask, and offset from the  struct-root

#define CV_CASK_MEMBER( FIELD)                                                                                              \
                { #FIELD, cv_fetch_cask( &(( Pal *) 0x8)->FIELD), uint16_t( int64_t( &((( Pal *) 0x8)->FIELD)) -0x8), 0 },      \


//_____________________________________________________________________________________________________________________________
// Declare  Field : Params [ Name, Cask With explicit Selector[ X], and offset from the  struct-root 

#define CV_CASK_MEMBER2(FIELD, FLX)                                                                                           \
                { #FIELD, cv_fetch_cask_##FLX( &(( Pal *) 0x8)->FIELD), uint16_t( int64_t( &((( Pal *) 0x8)->FIELD)) -0x8), 0 },  \


//_____________________________________________________________________________________________________________________________
// Cast of -1 to Base and cast to  Struct-root : Adding 1 and negating the value woulkd give back the offset of Base from struct-root

#define CV_CASK_BASE(BASE)                                                                                                 \
                { CV_CONCAT(b_,BASE), cv_fetch_cask( ( BASE *) 0x8), uint16_t( int64_t( static_cast< BASE *>(reinterpret_cast< This *>( 0x8))) -0x8), cv_BitMask< uint16_t>( FieldBaseBit) },    \

//_____________________________________________________________________________________________________________________________
                                                                                                                            
#define CV_CASK_MEMBER_END()                                                                                                \
                { 0, 0}                                                                                                     \
            };                                                                                                              \
            static uint32_t szFields = ( sizeof( entries)/ sizeof( FieldCask  ))-1;                                         \
            m_FieldCasks = Cv_Pack< FieldCask>( entries, entries +szFields);                                                \
            ProcessFieldCasks();                                                                                            \
        }                                                                                                                   \
        Cv_CStr  TypeString( void) const { return Pal::TypeName();  }                                                       \
    };                                                                                                                      \


//_____________________________________________________________________________________________________________________________

#define CV_CASK_END( PAL)                                                                                                   \
                                                                                                                            \
    friend  struct Pal::Cask;                                                                                               \
                                                                                                                            \
    static Cask   s_Cask;                                                                                                   \
};                                                                                                                          \
                                                                                                                            \
template < class Y>                                                                                                         \
typename PAL< Y>::Cask      PAL< Y>::s_Cask;                                                                                \
                                                                                                                            \
                                                                                                                            \
inline Cv_TypeCask *cv_fetch_cask( const PAL< int>::This *x)                                                                \
{                                                                                                                           \
    return &PAL< int>::s_Cask;                                                                                              \
}                                                                                                                           \
                                                                                                                            \

//_____________________________________________________________________________________________________________________________

#define CV_CASK_STRUCT_BEGIN( X)                                                                                            \
                                                                                                                            \
    CV_CASK_BEGIN( Cv_CaskPal##X, X)   CV_CASK_MEMBER_BEGIN()                                                               \
                                                                                                                            \
 
#define CV_CASK_STRUCT_END( X)       CV_CASK_MEMBER_END()               CV_CASK_END( Cv_CaskPal##X)                         \
                                                                                                                            \
    typedef Cv_CaskPal##X< int>::Cask           Cv_StructCask##X;                                                           \
                                                                                                                            \
    CV_CASK_BEGIN( Cv_StructCask##X##Cask, Cv_StructCask##X)                                                                \
        CV_CASK_MEMBER_BEGIN()                                                                                              \
        CV_CASK_MEMBER_END()                                                                                                \
    CV_CASK_END( Cv_StructCask##X##Cask)                                                                                    \
                                                                                                                            \


//_____________________________________________________________________________________________________________________________

CV_CASK_BEGIN( Cv_CaskPalDerivedCask, Cv_TypeCask::DerivedCask)   
    CV_CASK_MEMBER_BEGIN()
        CV_CASK_MEMBER( m_Deriv)
        CV_CASK_MEMBER( m_Name)
        CV_CASK_MEMBER( m_BOffset)
    CV_CASK_MEMBER_END()               
CV_CASK_END( Cv_CaskPalDerivedCask)   

CV_CASK_STRUCT_BEGIN( Cv_TypeCaskData)
    CV_CASK_MEMBER( m_Name)
  //  CV_CASK_MEMBER( m_Id)
CV_CASK_STRUCT_END( Cv_TypeCaskData)

CV_CASK_STRUCT_BEGIN( Cv_ParamCasket)
    CV_CASK_MEMBER( m_Owner)
    CV_CASK_MEMBER( m_Id)
    CV_CASK_MEMBER2( m_Data, Flex)
CV_CASK_STRUCT_END( Cv_ParamCasket)

CV_CASK_BEGIN( Cv_TypeBouquetCaskData, Cv_TypeBouquet::ParamCaskData)   
    CV_CASK_MEMBER_BEGIN()
        CV_CASK_MEMBER( m_Cask)   
        CV_CASK_MEMBER( m_Caskets)
    CV_CASK_MEMBER_END()               
CV_CASK_END( Cv_TypeBouquetCaskData)   

CV_CASK_BEGIN( Cv_TypeBouquetCaskDictEntry, Cv_TypeBouquet::CaskDictEntry)   
    CV_CASK_MEMBER_BEGIN()   
        CV_CASK_MEMBER( m_Cask)
        CV_CASK_MEMBER( m_CaskMapId)
    CV_CASK_MEMBER_END()               
CV_CASK_END( Cv_TypeBouquetCaskDictEntry)

CV_CASK_BEGIN( Cv_TypeBouquetParamCasketDictEntry, Cv_TypeBouquet::ParamCasketDictEntry)   
    CV_CASK_MEMBER_BEGIN()   
        CV_CASK_MEMBER( m_Casket)
        CV_CASK_MEMBER( m_CasketMapId)
    CV_CASK_MEMBER_END()               
CV_CASK_END( Cv_TypeBouquetParamCasketDictEntry)


CV_CASK_STRUCT_BEGIN( Cv_TypeBouquet)
    CV_CASK_MEMBER( m_FixedCaskArray)
    CV_CASK_MEMBER( m_FixedCasks)
    CV_CASK_MEMBER( m_ParamCasketArray)
    CV_CASK_MEMBER( m_ParamCaskMap)
    CV_CASK_MEMBER( m_ParamCasks)
CV_CASK_STRUCT_END( Cv_TypeBouquet)

//_____________________________________________________________________________________________________________________________

CV_CASK_STRUCT_BEGIN( Cv_CrateEntry)
    CV_CASK_MEMBER( m_Id)
CV_CASK_STRUCT_END( Cv_CrateEntry)

//_____________________________________________________________________________________________________________________________

template < typename X >           
struct Cv_CrateCaskPal : public Cv_Crate< X> 
{
    typedef  X                      This;
    typedef  Cv_CrateCaskPal< X>    Pal;

    struct Cask : public Cv_StructCask< This>                                                                            
    {
        
        Cask( void)                                                                                                      
            : Cv_StructCask< This>()                                                                                     
        {                                                                                                                
            static  Cv_StructCaskBase::FieldCask   entries[] =                                                                              
            {                                                                                                                                                                                                   \
                { "Elems", cv_fetch_cask(( std::vector< Cv_OPtr< X>> *) nullptr), int32_t( int64_t( &((( Pal *) 0x8)->m_Elems))) -0x8 }, 
                { 0, 0}                                                            
            };                                                                     
            static uint32_t szFields = ( sizeof( entries)/ sizeof( Cv_StructCaskBase::FieldCask  ))-1;
            Cv_StructCaskBase::m_FieldCasks = Cv_Pack< Cv_StructCaskBase::FieldCask>( entries, entries +szFields);       
            this->ProcessFieldCasks();                                                   
        }                 
        
        Cv_CStr  TypeString( void) const 
        {
            static  char    s_Buf[ 256];
            static  char    sz = 0;
            if ( sz)
                return Cv_CStr( &s_Buf[ 0], sz);
            sz = snprintf( &s_Buf[ 0], 256, "Crate[%s]", cv_TypeCask< X>()->TypeString().CStr()); 
            return Cv_CStr( &s_Buf[ 0], sz); 
        }              
    };                                                                             
    static Cask            s_Cask;
    
    friend  struct Pal::Cask;

};
template < typename X>                                                                                                         \
typename Cv_CrateCaskPal< X>::Cask      Cv_CrateCaskPal< X>::s_Cask; 

template < typename X>
Cv_TypeCask         *cv_fetch_cask( const Cv_Crate< X> *x) { return &Cv_CrateCaskPal< X>::s_Cask; }
 
//_____________________________________________________________________________________________________________________________
