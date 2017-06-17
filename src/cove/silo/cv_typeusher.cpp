//_______________________________________________ cv_typeusher.cpp _______________________________________________________________

#include	"cove/barn/cv_include.h"
#include	"cove/silo/cv_typeusher.h"
#include	"cove/silo/cv_typecask.h"

//_____________________________________________________________________________________________________________________________

Cv_TypeCask     *Cv_TypeUsher::TypeInfoCask( const std::type_info  *ti)
{
    return Cv_TypeCaskMgr::Singleton()->Find( ti);
}

//_____________________________________________________________________________________________________________________________

Cv_CStr     Cv_TypeUsher::TypeString( Cv_TypeCasket *paramCasket)
{
    return paramCasket->Master()->TypeString();
}

//_____________________________________________________________________________________________________________________________

bool    Cv_TypeUsher::StoreAtOffset( Cv_OutDocket *outDocket, Cv_TypeCask *baseCask, Cv_TypeCasket *storeCasket, Cv_TypeCasket *objCasket, const void *pObj)
{
    CV_FNPASS(())
    
    uint32_t                curOff = ( uint32_t) outDocket->Spritz()->Offset();

    if ( !pObj)
    {
        outDocket->Spritz()->FillNull( sizeof( uint32_t));
        return true;
    }
    // adjustments relative to storeCasket with baseCask
    uint32_t                        code = 0;
    

    // we need to encode the objectCasket Code if it not same as the baseCasket or is non-trivial
    if (( objCasket->Master() != baseCask) || !objCasket->IsParamTrivial())
        code =  uint16_t( outDocket->Bouquet()->CodeForCasket( objCasket)) +1;        

    // If the store casket is not sa
    if ( storeCasket->Master() != baseCask)
    {
        Cv_TypeBouquet::BaseRec    baseRec = storeCasket->BaseRecOffset( outDocket->Bouquet(), baseCask);
        code = code | ( baseRec.second << 16);                  // offset of ThisClass from  the Base
    }
        
    uint32_t                        objSz = objCasket->Size();   
    if ( code)
        objSz += sizeof( uint32_t);
    uint32_t                        auxOffset = ( uint32_t) outDocket->Spritz()->EnsureSize( objSz);
    uint32_t                        dist = auxOffset -curOff;

    if ( code)                      // if code is valid set the 31st bit.
        dist = dist | ( 1 << 31);

    outDocket->Spritz()->Write( &dist, sizeof( uint32_t));
    outDocket->Spritz()->SetOffset( auxOffset);
    if ( code)
        outDocket->Spritz()->Write( &code, sizeof( uint32_t));
    
    uint32_t        storeOffset =  ( storeCasket->Master() != baseCask) ? baseCask->DerivedCasks().at( baseCask->FindDerivedCask( storeCasket->Master())).m_BOffset : 0;
    uint32_t        objOffset =  ( objCasket->Master() != baseCask) ? baseCask->DerivedCasks().at( baseCask->FindDerivedCask( objCasket->Master())).m_BOffset : 0;
    
    return objCasket->StoreObject( ( const uint8_t *)( pObj) -storeOffset + objOffset, outDocket);
}

//_____________________________________________________________________________________________________________________________
    
uint32_t    Cv_TypeUsher::RestoreAtOffset( Cv_InDocket *inDoc, Cv_TypeCask *baseCask, Cv_TypeCasket *storeCasket, void **pObj) 
{
    CV_FNPASS(())
    
    *pObj = nullptr;
    uint32_t                        curOff = ( uint32_t) inDoc->Spritz()->Offset();
    uint32_t                        dist = 0;
    uint32_t                        code = 0;
    inDoc->Spritz()->Read( &dist, sizeof( uint32_t));
    if ( !dist)
        return 0;
    bool                            codeFlg =  !!( dist & ( 1 << 31));
    inDoc->Spritz()->SetOffset( curOff +( dist & ~( 1 << 31)));

    if ( codeFlg)
        inDoc->Spritz()->Read( &code, sizeof( uint32_t));
    
    Cv_TypeCasket                   *objCasket = NULL;
    if ( code == 0)
        objCasket = baseCask->CreateCasket( inDoc->Bouquet(), nullptr);
    else 
        objCasket = inDoc->Bouquet()->CasketForCode( uint32_t( uint16_t( code) -1) | ( 1 << 31));
    
    uint8_t             *obj = objCasket->Master()->CreateObject();
    uint32_t            objOff = ( uint32_t) inDoc->Spritz()->Offset();
    objCasket->RestoreObject( obj, inDoc);
    uint32_t        storeOffset =  ( storeCasket->Master() != baseCask) ? baseCask->DerivedCasks().at( baseCask->FindDerivedCask( storeCasket->Master())).m_BOffset : 0;
    uint32_t        objOffset =  ( objCasket->Master() != baseCask) ? baseCask->DerivedCasks().at( baseCask->FindDerivedCask( objCasket->Master())).m_BOffset : 0;
    if ( !code && !baseCask->IsFixed())
        delete objCasket;
    
    *pObj = ( void *)( obj +storeOffset -objOffset);
    return objOff;
}

//_____________________________________________________________________________________________________________________________
    
bool    Cv_TypeUsher::SaveCasketDefSource( Cv_TypeCask *rootCask, Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc)
{
    static std::string      s_DefPtrString = "DefPtr";
    Cv_SpritzLog            &spritzLog = srcDoc->SourceLog();  
    uint32_t                *pFlag = srcDoc->FlagForTemplate( s_DefPtrString);
    Cv_TypeBouquet::BaseRec baseRec = paramCasket->BaseRecOffset( srcDoc->Bouquet(), rootCask);

    if ( !*pFlag)
    {
        spritzLog << "template < class X>                                                                                               \n";   
        spritzLog << "struct  " << s_DefPtrString << ";                                                                                 \n";   
        spritzLog << "                                                                                                                  \n";
    }
    *pFlag = true;
    std::string         typeName = Cv_Aid::CPPTypeStringCleanup( paramCasket->StructString( srcDoc->Bouquet()));
    uint32_t             *pTFlag = srcDoc->FlagForTemplate( s_DefPtrString + ":" + typeName );
    if ( *pTFlag)
        return true;
    *pTFlag = true;

    spritzLog << "template <>                                                                                                           \n";   
    spritzLog << "struct  " << s_DefPtrString << "< " << typeName << ">                                                                 \n";
    spritzLog << "{                                                                                                                     \n";
    spritzLog << "    uint32_t		m_DistCode;                                                                                         \n";
    spritzLog << "                                                                                                                      \n";
    spritzLog << "    typedef  " << typeName << "               TClass;                                                                 \n";
    spritzLog << "                                                                                                                      \n";
    spritzLog << "    TClass       *Ptr( void) const                                                                                    \n";
    spritzLog << "    {                                                                                                                 \n";
    spritzLog << "        bool              codeFlg =  !!( m_DistCode & ( 1 << 31));                                                    \n";
    spritzLog << "        uint8_t           *pLoc = ReCast< uint8_t>( this) + ( m_DistCode & ~( 1 << 31));                              \n";
    if ( baseRec.first)                                                                                                                 
        spritzLog << "        uint32_t          baseOff = " << baseRec.second << ";                                                     \n";
    else
        spritzLog << "        uint32_t          baseOff = 0;                                                                            \n";
    spritzLog << "        if ( !codeFlg)                                                                                                \n";
    spritzLog << "            return ReCast< TClass >( pLoc +baseOff);                                                                  \n";
    spritzLog << "        uint32_t          code = *ReCast< uint32_t>( pLoc);                                                           \n";
    spritzLog << "        uint32_t          recOff = ( code >> 16);                                                                     \n";
    spritzLog << "        return reinterpret_cast< TClass *>( pLoc + 4 -recOff +baseOff);                                               \n";
    spritzLog << "    }                                                                                                                 \n";
    spritzLog << "};                                                                                                                    \n";
    spritzLog << "                                                                                                                      \n";
    return true;
}

//_____________________________________________________________________________________________________________________________
    
bool    Cv_TypeUsher::SaveCasketRefSource( Cv_TypeCask *rootCask, Cv_TypeCasket *paramCasket, Cv_OutSourceDocket *srcDoc)
{
    static const char       *s_RefPtrString = "RefPtr";
    Cv_SpritzLog            &spritzLog = srcDoc->SourceLog(); 
    uint32_t                *pFlag = srcDoc->FlagForTemplate( s_RefPtrString);
    Cv_TypeBouquet::BaseRec baseRec = paramCasket->BaseRecOffset( srcDoc->Bouquet(), rootCask);
    uint32_t                baseOff = ( baseRec.first) ? baseRec.second : 0;
    
    if ( !*pFlag)
    {
        spritzLog << "template < class X>                                                                                               \n";   
        spritzLog << "struct  " << s_RefPtrString << ";                                                                                 \n";   
        spritzLog << "                                                                                                                  \n";
    }
    *pFlag = true;

    std::string         typeName = Cv_Aid::CPPTypeStringCleanup( paramCasket->StructString( srcDoc->Bouquet()));
    spritzLog << "struct  " << typeName << ";                                                                                           \n";
    spritzLog << "                                                                                                                      \n";
    spritzLog << "template <>                                                                                                           \n";   
    spritzLog << "struct  " << s_RefPtrString << "< " << typeName << ">                                                                 \n";
    spritzLog << "{                                                                                                                     \n";
    spritzLog << "    typedef  " << typeName << "               TClass;                                                                 \n";
    spritzLog << "    uint32_t		    m_RefOff;                                                                                       \n";

    spritzLog << "    TClass       *Ptr( void) const                                                                                    \n";
    spritzLog << "    {                                                                                                                 \n";
    if ( baseRec.first)                                                                                                                 
    {
        spritzLog << "        return ReCast< TClass >( ReCast< uint8_t>( this) + m_RefOff -" << baseOff << ");                          \n";
    }
    else
    {    
        spritzLog << "        return ReCast< TClass >( ReCast< uint8_t>( this) + m_RefOff);                                             \n";
    }
    spritzLog << "    }                                                                                                                 \n";
    spritzLog << "};                                                                                                                    \n";
    spritzLog << "                                                                                                                      \n";
    return true;
}

//_____________________________________________________________________________________________________________________________
// sedge, segue
