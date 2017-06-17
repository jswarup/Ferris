//_______________________________________________ cv_typebouqet.cpp _______________________________________________________________

#include    "cove/barn/cv_include.h"
#include    "cove/silo/cv_typebouqet.h"
#include    "cove/silo/cv_typechest.h"

//_____________________________________________________________________________________________________________________________

std::vector< Cv_TypeElement>    Cv_TypeElement::CollectSubElements( Cv_Docket *xDocket) 
{
    if ( !m_Var)
        return std::vector< Cv_TypeElement>();
    return m_Var->m_Casket->CollectSubElements( xDocket, m_Var->m_Ptr);
}

//_____________________________________________________________________________________________________________________________

Cv_Enumerator< Cv_TypeElement>  *Cv_TypeElement::CollectSubElementsEnumberator( Cv_Docket *xDocket)
{
    return new Cv_StdEnumerator< std::vector<Cv_TypeElement> >( CollectSubElements( xDocket));
}

//_____________________________________________________________________________________________________________________________

bool    Cv_TypeElement::Serialize( Cv_AttribValueReceptor *receptor) const
{
    if ( m_Var) 
        return  m_Var->m_Ptr  ? m_Var->m_Casket->SerializeObj( receptor, m_Var->m_Ptr) : true;
    return receptor->Accept( m_Str->m_Value);
}

//_____________________________________________________________________________________________________________________________

Cv_XMlXlemDispensor::Cv_XMlXlemDispensor( const Cv_TypeElement &elem, Cv_Docket *xDocket)
    :   m_Elem( elem), m_XDocket( xDocket), m_Ind( uint32_t( -1))
{
    m_SubElements = m_Elem.CollectSubElements( m_XDocket);

}

//_____________________________________________________________________________________________________________________________

const Cv_CStr   &Cv_XMlXlemDispensor::Name( void)
{
    return m_Elem.Name();;
}

//_____________________________________________________________________________________________________________________________

bool            Cv_XMlXlemDispensor::MoveNextAttrib( Cv_AttribValueReceptor *receptor)
{
    if ( m_Ind != uint32_t( -1))
        m_SubElements[ m_Ind] = Cv_TypeElement();
    while ( ++m_Ind  < m_SubElements.size())
    {
        const Cv_TypeElement  &subElem = m_SubElements[ m_Ind]; 
        if ( subElem.Serialize( receptor))
            return true;
    }
    return false;
}

//_____________________________________________________________________________________________________________________________

const Cv_CStr   &Cv_XMlXlemDispensor::AttribName( void) 
{
    return m_SubElements[ m_Ind].Name();
}

//_____________________________________________________________________________________________________________________________

bool            Cv_XMlXlemDispensor::HasChildren( void)
{
    m_SubElements.erase( std::remove( m_SubElements.begin(), m_SubElements.end(), Cv_TypeElement()), m_SubElements.end());
    return m_SubElements.size() != 0;
}

//_____________________________________________________________________________________________________________________________

int32_t     Cv_TypeBouquet::CaskDictEntry::Compare( const CaskDictEntry &a) const 
{ 
    return m_Cask->Compare( *a.m_Cask); 
}
        
//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeBouquet::CaskDictEntry &cd)
{
    ostr << "DictEntry[ ";  
    ostr << *cd.m_Cask.Ptr() << ", " << cd.m_CaskMapId;
    ostr << "] ";
    return ostr;
}

//_____________________________________________________________________________________________________________________________

int32_t     Cv_TypeBouquet::ParamCasketDictEntry::Compare( const ParamCasketDictEntry &a) const 
{ 
    return m_Casket->Compare( *a.m_Casket); 
}
        
//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeBouquet::ParamCasketDictEntry &cd)
{
    ostr << "CasketDictEntry[ ";  
    ostr << *cd.m_Casket.Ptr() << ", " << cd.m_CasketMapId;
    ostr << "] ";
    return ostr;
}
        
//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeBouquet::ParamCaskData &cd)
{
    ostr << "ParamCaskData[ ";
    for ( uint32_t i = 0; i < cd.m_Caskets.size(); ++i)
        ostr << cd.m_Caskets[ i];
    ostr << "] ";
    return ostr;
}

//_____________________________________________________________________________________________________________________________

std::ostream     &operator<<( std::ostream &ostr, const Cv_TypeBouquet &cd)
{
    ostr << "DictEntry[ ";
//            ostr.IncrMargin();
    for ( uint32_t i = 0; i < cd.m_FixedCasks.size(); ++i)
        ostr << '\n' << cd.m_FixedCasks[ i];
    for ( uint32_t i = 0; i < cd.m_ParamCasks.size(); ++i)
        ostr << '\n' << cd.m_ParamCasks[ i];
//          
//            ostr.DecrMargin();
    ostr << "\n] ";
    ostr << "FixedCaskMap[ ";
//            ostr.IncrMargin();
    for ( uint32_t i = 0; i < cd.m_FixedCaskArray.size(); ++i)
        ostr << '\n' << *cd.m_FixedCaskArray[ i];
//            ostr.DecrMargin();
    ostr << "\n] ";
    ostr << "ParamCaskMap[ ";
//          ostr.IncrMargin();
    for ( uint32_t i = 0; i < cd.m_ParamCaskMap.size(); ++i)
        ostr << '\n' << cd.m_ParamCaskMap[ i];
//            ostr.DecrMargin();
    ostr << "\n] ";
    return ostr;
}

//_____________________________________________________________________________________________________________________________

const Cv_CStr   &Cv_TypeBouquet::DerivedCasket::CasketName( void) const 
{ 
    return m_Casket->Master()->Name(); 
}

//_____________________________________________________________________________________________________________________________

Cv_TypeBouquet::Cv_TypeBouquet( void)
{
    Cv_TypeCaskMgr::Singleton();
}

//_____________________________________________________________________________________________________________________________

uint32_t   Cv_TypeBouquet::FetchCaskIndex( Cv_TypeCaskData *typeData)
{
    CV_FNPASS( () << *typeData)

    Cv_TypeCask                             *typeCask = static_cast< Cv_TypeCask *>( typeData);
    CaskDictEntry                           caskDictEntry( typeData);
    std::vector< CaskDictEntry >::iterator  caskIt;
    if ( typeCask->IsFixed())
    {
        caskIt = std::lower_bound( m_FixedCasks.begin(), m_FixedCasks.end(), caskDictEntry, Cv_TLess< void>());
        if (( caskIt == m_FixedCasks.end()) || Cv_TLess< void>()( caskDictEntry, *caskIt))
        {
            caskDictEntry.m_CaskMapId = uint32_t( m_FixedCaskArray.size());
            m_FixedCaskArray.push_back( typeCask); 
            m_FixedCasks.insert( caskIt, caskDictEntry);
            CV_FNLOG( () << Cv_Flume::Format("\nStore : CaskMapId[ %d]", caskDictEntry.m_CaskMapId))
        } else {
            caskDictEntry = *caskIt;
            CV_FNLOG( () << Cv_Flume::Format("\nFound : CaskMapId[ %d]", caskDictEntry.m_CaskMapId))
        }
    } else {
        caskIt = std::lower_bound( m_ParamCasks.begin(), m_ParamCasks.end(), caskDictEntry, Cv_TLess< void>());
        if (( caskIt == m_ParamCasks.end()) || Cv_TLess< void>()( caskDictEntry, *caskIt))
        {
            caskDictEntry.m_CaskMapId = uint32_t( m_ParamCaskMap.size());
            m_ParamCasks.insert( caskIt, caskDictEntry);
            m_ParamCaskMap.push_back( ParamCaskData()); 
            m_ParamCaskMap.back().m_Cask = typeCask;
            CV_FNLOG( () << Cv_Flume::Format("\nStore : CaskMapId[ %d]", caskDictEntry.m_CaskMapId))
        } else {
            caskDictEntry = *caskIt;
            CV_FNLOG( () << Cv_Flume::Format("\nFound : CaskMapId[ %d]", caskDictEntry.m_CaskMapId))
        }
    }
    return caskDictEntry.m_CaskMapId;
}

//_____________________________________________________________________________________________________________________________

uint32_t  Cv_TypeBouquet::FetchParamCasketIndex( uint32_t caskIndex, const void *thisPtr)
{
    CV_FNPASS( () << caskIndex )
    
    Cv_TypeCask                     *typeCask = static_cast< Cv_TypeCask *>( m_ParamCaskMap[ caskIndex].m_Cask);
    Cv_TypeCasket                   *casket = typeCask->CreateCasket( this, thisPtr);;
    ParamCasketDictEntry            paramCasket = static_cast< Cv_ParamCasket *>( casket);
    ParamCaskData                   *caskData = &m_ParamCaskMap[ caskIndex];

    std::vector< ParamCasketDictEntry >::iterator  it = std::lower_bound( caskData->m_Caskets.begin(), caskData->m_Caskets.end(), paramCasket, Cv_TLess< void>());
    if (( it == caskData->m_Caskets.end()) || Cv_TLess< void>()( paramCasket, *it))
    {   
        paramCasket.m_Casket->SetId( uint32_t( m_ParamCasketArray.size()));
        paramCasket.m_CasketMapId =  uint32_t( m_ParamCasketArray.size());
        m_ParamCasketArray.push_back( paramCasket.m_Casket);
        caskData->m_Caskets.insert( it, paramCasket);
        if ( thisPtr)
            paramCasket.m_Casket->PostScan( this, thisPtr); 
        return paramCasket.m_Casket->GetId();
    }
    delete paramCasket.m_Casket;
    return  (*it).m_Casket->GetId();
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCasket   *Cv_TypeBouquet::LocateCasket( Cv_TypeCaskData *typeData, const void *thisPtr)
{
    CV_FNPASS( () << *typeData)

    Cv_TypeCask     *typeCask = static_cast< Cv_TypeCask *>( typeData);
    uint32_t        caskIndex = FetchCaskIndex( typeData);

    if ( typeCask->IsFixed())
        return typeCask->CreateCasket( this, thisPtr);
    uint32_t        casketIndex = FetchParamCasketIndex( caskIndex, thisPtr);
    return  m_ParamCasketArray[ casketIndex];
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_TypeBouquet::LocateCaskIndex( Cv_TypeCaskData *typeData) const
{
    CV_FNPASS( () << *typeData)

    Cv_TypeCask                             *typeCask = static_cast< Cv_TypeCask *>( typeData);
    CaskDictEntry                           caskDictEntry( typeData);
    std::vector< CaskDictEntry >::const_iterator   caskIt;
    if ( typeCask->IsFixed())
    {
        caskIt = std::lower_bound( m_FixedCasks.begin(), m_FixedCasks.end(), caskDictEntry, Cv_TLess< void>());
        CV_ERROR_ASSERT(( caskIt != m_FixedCasks.end()) && !Cv_TLess< void>()( caskDictEntry, *caskIt))
    } else {
        caskIt = std::lower_bound( m_ParamCasks.begin(), m_ParamCasks.end(), caskDictEntry, Cv_TLess< void>());
        CV_ERROR_ASSERT(( caskIt != m_ParamCasks.end()) && !Cv_TLess< void>()( caskDictEntry, *caskIt))
    }
    CV_FNLOG( () << caskDictEntry.m_CaskMapId )
    return caskIt->m_CaskMapId;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCasket   *Cv_TypeBouquet::TypeCasket( uint32_t caskInd, uint32_t casketInd) const
{
    CV_FNPASS( () << caskInd << " " << casketInd )

    if ( casketInd == CV_UINT32_MAX)
    {
        Cv_TypeCask     *typeCask = static_cast< Cv_TypeCask *>( m_FixedCaskArray[ caskInd]);
        return typeCask->CreateCasket( nullptr, nullptr);
    } 
    return m_ParamCasketArray[ casketInd];
}

//_____________________________________________________________________________________________________________________________

uint32_t        Cv_TypeBouquet::CodeForCasket( Cv_TypeCasket  *casket) const
{
    Cv_TypeCask     *cask = casket->Master();
    if ( cask->IsFixed())
        return  LocateCaskIndex( cask) & ~( 1 << 31);
    return  ( 1 << 31) | casket->GetId();
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCasket   *Cv_TypeBouquet::CasketForCode( uint32_t code) const
{
    CV_FNPASS( () << code )
    
    uint32_t    index = code &  ~( 1 << 31);
    if ( code &  ( 1 << 31))
        return m_ParamCasketArray[  index];
    Cv_TypeCask     *typeCask = static_cast< Cv_TypeCask *>( m_FixedCaskArray[ index]);
    return typeCask->CreateCasket( nullptr, nullptr);
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_TypeBouquet::CasketIndex( Cv_TypeCasket  *casket) const
{
    Cv_TypeCask             *cask = casket->Master();
    if ( cask->IsFixed() || casket->IsParamTrivial())
        return CV_UINT32_MAX;
     
    CaskDictEntry           caskDictEntry( cask);
    std::vector< CaskDictEntry >::const_iterator  caskIt = std::lower_bound( m_ParamCasks.begin(), m_ParamCasks.end(), caskDictEntry, Cv_TLess< void>());
    
    const ParamCaskData           *caskData = &m_ParamCaskMap[ caskIt->m_CaskMapId];
    ParamCasketDictEntry    paramCasket = static_cast< Cv_ParamCasket *>( casket);

    std::vector< ParamCasketDictEntry >::const_iterator  it = std::lower_bound( caskData->m_Caskets.begin(), caskData->m_Caskets.end(), paramCasket, Cv_TLess< void>());
    uint32_t                index =  uint32_t( it -caskData->m_Caskets.begin());   
    return index;
}
    
//_____________________________________________________________________________________________________________________________

std::vector< Cv_TypeCasket *>     Cv_TypeBouquet::FindCaskets( Cv_TypeCask  *cask) const
{
    if ( cask->IsFixed())
        return  std::vector< Cv_TypeCasket *>( 1, cask->CreateCasket( nullptr, nullptr));
    
    CaskDictEntry           caskDictEntry( cask);
    std::vector< CaskDictEntry >::const_iterator  caskIt = std::lower_bound( m_ParamCasks.begin(), m_ParamCasks.end(), caskDictEntry, Cv_TLess< void>());
    
    const ParamCaskData     *caskData = &m_ParamCaskMap[ caskIt->m_CaskMapId];
    
    std::vector< Cv_TypeCasket *>   paramCaskets;
    std::for_each( caskData->m_Caskets.begin(), caskData->m_Caskets.end(), [ &paramCaskets]( const ParamCasketDictEntry &p){ paramCaskets.push_back( p.m_Casket.Ptr()); });
    return paramCaskets;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCasket   *Cv_TypeBouquet::PositionCasket( Cv_TypeCaskData *typeData, uint32_t casketId)
{
    uint32_t        caskIndex = LocateCaskIndex( typeData);
    return TypeCasket( caskIndex, casketId);
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCasket     *Cv_TypeBouquet::FetchCasketForObj( const std::type_info *objInfo, const void *pObj)
{
    Cv_TypeCaskMgr              *typeInfoMgr = Cv_TypeCaskMgr::Singleton();
    Cv_TypeCask                 *objCask = typeInfoMgr->Find( objInfo);
    Cv_TypeCasket               *derivedCasket = LocateCasket( objCask, pObj);
    return derivedCasket;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeBouquet::DerivedCasket  Cv_TypeBouquet::FetchDerivedCasketForObj( Cv_TypeCask *cask, const std::type_info *objInfo, const void *pObj)
{
    CV_FNPASS( () )

    if ( *objInfo == *cask->TypeInfo())
    {
        Cv_TypeCasket           *dCasket = LocateCasket( cask, pObj);
        return DerivedCasket( dCasket, 0, 0);
    }
    Cv_TypeCaskMgr              *typeInfoMgr = Cv_TypeCaskMgr::Singleton();
    Cv_TypeCask                 *objCask = typeInfoMgr->Find( objInfo);
    Cv_TypeCasket               *derivedCasket = LocateCasket( objCask, pObj);
    uint32_t                    derivIndex = cask->FindDerivedCask( objCask);
    Cv_TypeCask::DerivedCask    dCask = cask->DerivedCasks().at( derivIndex);
    return DerivedCasket( derivedCasket, dCask.m_BOffset, derivIndex + 1); 
}

//_____________________________________________________________________________________________________________________________

Cv_TypeBouquet::DerivedCasket  Cv_TypeBouquet::FetchDerivedCasketForObj( Cv_TypeCask *cask, Cv_TypeCasket *dCasket, const void *pObj)
{
    CV_FNPASS( () )

    if ( dCasket->Master() == cask)
        return DerivedCasket( dCasket, 0, 0);
    
    Cv_TypeCaskMgr              *typeInfoMgr = Cv_TypeCaskMgr::Singleton();
    uint32_t                    derivIndex = cask->FindDerivedCask( dCasket->Master());
    Cv_TypeCask::DerivedCask    dCask = cask->DerivedCasks().at( derivIndex);
    return DerivedCasket( dCasket, dCask.m_BOffset, derivIndex + 1); 
}

//_____________________________________________________________________________________________________________________________

Cv_TypeBouquet::DerivedCasket           Cv_TypeBouquet::FetchDerivedCasketFoCode( Cv_TypeCask *cask, uint32_t derivIndex, uint32_t casketId)
{
    CV_FNPASS( () )

    if ( !derivIndex)
    {
        Cv_TypeCasket           *dCasket = PositionCasket( cask, casketId);
        return DerivedCasket( dCasket, 0, 0);
    }
    Cv_TypeCask::DerivedCask    dCask = cask->DerivedCasks().at( derivIndex -1);
    Cv_TypeCasket               *derivedCasket = PositionCasket( dCask.m_Deriv, casketId);
    return DerivedCasket( derivedCasket, dCask.m_BOffset, 0);
}
    
//_____________________________________________________________________________________________________________________________
// return the offsets of pending references, if any.

std::vector< Cv_OPtr< Cv_OutDocket::DefOffListener> >    Cv_OutDocket::ObjOffsetInfoMap::FetchPendingRefs( const void *pObj, uint32_t defOff)
{
    CV_FNPASS(())
    
    m_ObjOffMap[ pObj] = defOff;
    std::map< const void *, std::vector< Cv_OPtr< DefOffListener> > >::iterator     it = m_ObjReflocs.find( pObj);
    if ( it == m_ObjReflocs.end())
        return std::vector< Cv_OPtr< DefOffListener> >();
    std::vector< Cv_OPtr< DefOffListener> >      pendingRefs;
    it->second.swap( pendingRefs);
    m_ObjReflocs.erase( it);
    return pendingRefs;
            
}

//_____________________________________________________________________________________________________________________________
// return the defOff else add to the the pending refs

uint32_t    Cv_OutDocket::ObjOffsetInfoMap::FetchDefOffset( const void *pObj, DefOffListener *refOff)
{
    CV_FNPASS(())
    
    std::map< const void *, uint32_t>::iterator     it = m_ObjOffMap.find( pObj); 
    if ( it != m_ObjOffMap.end())   
        return it->second;
    m_ObjReflocs[ pObj].push_back( refOff);
    return CV_UINT32_MAX;
}

//_____________________________________________________________________________________________________________________________

void    Cv_OutDocket::RegisterObjectDefToOffset(  Cv_TypeCask *cask, const void *pObj, uint32_t defOff)
{
    CV_FNPASS( () )

    std::vector< Cv_OPtr< DefOffListener> >    pendingRefs = m_ObjOffsetMapSet[ cask].FetchPendingRefs( pObj, defOff);
    if ( !pendingRefs.size())
        return;
    for ( uint32_t  i = 0; i < pendingRefs.size();  ++i)
        pendingRefs[ i]->DefCallback( Spritz(), defOff);
    return;
}

//_____________________________________________________________________________________________________________________________

void    Cv_OutDocket::RegisterObjectRefToOffset(  Cv_TypeCask *cask, const void *pObj, DefOffListener *off)
{
    CV_FNPASS( () )

    uint32_t    defOff = m_ObjOffsetMapSet[ cask].FetchDefOffset( pObj, off);
    if ( defOff != CV_UINT32_MAX)
        off->DefCallback( Spritz(), defOff);
    return;
}

//_____________________________________________________________________________________________________________________________
    
uint32_t    Cv_OutDocket::StoreObject( Cv_TypeCasket *casket, const void *x)
{
    CV_FNPASS( () )

    uint32_t            szPrimary = casket->Size();
    uint32_t            offset = ( uint32_t) Spritz()->EnsureSize( szPrimary);
    Spritz()->SetOffset( offset); 
    casket->StoreObject( x, this);
    return Bouquet()->CodeForCasket( casket);
}

//_____________________________________________________________________________________________________________________________
// return the offsets of pending references, if any.

std::vector< Cv_OPtr< Cv_InDocket::DefObjListener> >     Cv_InDocket::OffObjectInfoMap::FetchPendingRefs( uint32_t defOff, void *pObj)
{
    CV_FNPASS(())

    m_OffObjMap[ defOff] = pObj;
    std::map< uint32_t, std::vector< Cv_OPtr< DefObjListener> > >::iterator     it = m_OffObjRefs.find( defOff);
    if ( it == m_OffObjRefs.end())
        return std::vector< Cv_OPtr< DefObjListener> >();
    std::vector< Cv_OPtr< DefObjListener> >      pendingRefs;
    it->second.swap( pendingRefs);
    m_OffObjRefs.erase( it);
    return pendingRefs;
}

//_____________________________________________________________________________________________________________________________
// return the defOff else add to the the pending refs

void          *Cv_InDocket::OffObjectInfoMap::FetchDefObject( uint32_t objOff, DefObjListener *refCallback)
{
    CV_FNPASS(())

    std::map< uint32_t, void *>::iterator     it = m_OffObjMap.find( objOff); 
    if ( it != m_OffObjMap.end())   
        return it->second;
    m_OffObjRefs[ objOff].push_back( refCallback);
    return nullptr;
}

//_____________________________________________________________________________________________________________________________
    
void    Cv_InDocket::RegisterOffsetToObjectDef( Cv_TypeCask *cask, uint32_t off, void *pObj)
{
    CV_FNPASS(() << cask->Name() << " Off:" << off << " pObj:" << pObj)

    std::vector< Cv_OPtr< DefObjListener> >    pendingRefs = m_OffObjectMapSet[ cask].FetchPendingRefs( off, pObj);
    if ( !pendingRefs.size())
        return;
    for ( uint32_t i = 0; i < pendingRefs.size(); ++i)
        pendingRefs[ i]->RefCallback( pObj);
    return;
}

//_____________________________________________________________________________________________________________________________

void    Cv_InDocket::RegisterRefCallbackForObjectOff( Cv_TypeCask *cask, uint32_t objOff, DefObjListener *refCallback)
{
    CV_FNPASS(() << cask->Name() << " Off:" << objOff)
        
    void          *defObj = m_OffObjectMapSet[ cask].FetchDefObject( objOff, refCallback);
    if ( defObj)
        refCallback->RefCallback( defObj);
    return;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_InDocket::RestoreObject( Cv_TypeCasket *casket, void *x)
{
    return casket->RestoreObject( x, this);
}

//_____________________________________________________________________________________________________________________________

bool    Cv_InDocket::RestoreBouquet( Cv_TypeBouquet *x)
{
    CV_FNPASS(())
    
    Cv_TypeCask     *cask = cv_TypeCask< Cv_TypeBouquet>();
    uint32_t        caskIndex = Bouquet()->FetchCaskIndex( cask);
    uint32_t        casketIndex = Bouquet()->FetchParamCasketIndex( caskIndex, nullptr);
    Cv_TypeCasket   *casket = Bouquet()->TypeCasket( caskIndex, casketIndex);
    return casket->RestoreObject( x, this);
}
    
//_____________________________________________________________________________________________________________________________

bool    Cv_XOutDocket::ElemTravesor( const Cv_TypeElement &elem, Cv_TreeTraversor< Cv_TypeElement> *context, bool entryFlg, void *)
{
    if ( entryFlg)
    {
        Cv_XMlXlemDispensor                     xlemDispensor( elem, this); 
        XEncoder()->StartElement( &xlemDispensor);

        const std::vector< Cv_TypeElement>      &subElems = xlemDispensor.SubElements();
        for ( uint32_t i = 0; i < subElems.size(); ++i)
            context->PushElem( subElems[ i]);
    } else {
        XEncoder()->EndElement();
    }
    return true;
}

//_____________________________________________________________________________________________________________________________
    
uint32_t    Cv_XOutDocket::StoreXDoc( const Cv_CStr &name, Cv_TypeCasket *casket, const void *x)
{
    CV_FNPASS( () )

    Cv_TypeElement        elem( name, casket, x);
    m_TreeTraversor.DoDepthTraversal( elem, this,  &Cv_XOutDocket::ElemTravesor, ( void *) nullptr);
    return 0;
}

//_____________________________________________________________________________________________________________________________

bool    Cv_OutSourceDocket::IsProcessed( Cv_TypeCasket *fieldCasket)
{
    return IsExited( IndexForCasket( fieldCasket));
}

//_____________________________________________________________________________________________________________________________

bool    Cv_OutSourceDocket::EncodeCasket( uint32_t stateId, Cv_GraphTraversor< Cv_OutSourceDocket> *context, bool entryFlg)
{
    Cv_TypeCasket   *casket = CasketForIndex( stateId);
    CV_FNPASS( () << stateId << " " << entryFlg << " " << casket->Master()->Name())

    if ( entryFlg)
    {
        std::string             casketName = casket->StructString( Bouquet());
        Cv_Twin< Cv_CStr>       lsplt =  Cv_Aid::LSplit( casketName, '[');
        if ( lsplt.Second().Size()  && false)
//        if (( lsplt.first == Cv_CStr( "DefPtr")) || ( lsplt.first == Cv_CStr( "RefPtr")))
        {
            CV_ERROR_ASSERT(  lsplt.Second().Size())
            Cv_Twin< Cv_CStr>       rsplt =  Cv_Aid::RSplit( lsplt.Second().Advance( 1), ']');
            Cv_TypeCaskData         subTypeData( rsplt.First().Shorten( 1));
            Cv_TypeCask             *subCask = Cv_TypeCaskMgr::Singleton()->Find( &subTypeData);
            std::vector< Cv_TypeCasket *>      subCaskets = Bouquet()->FindCaskets( subCask);
            for ( uint32_t i = 0; i < subCaskets.size(); ++i)
            {
                Cv_TypeCasket           *subCasket = subCaskets[ i];
                uint32_t                subId = IndexForCasket( subCasket); 
                if ( !context->IsEntered( subId))
                    context->DoQueState( subId);
            }
        }
        return true;
    }
    
    casket->StoreDefinition( this);
    return true;
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_OutSourceDocket::IndexEnumertor::Current( void) const 
{ 
    return m_OutSourceDocket->IndexForCasket( m_ElemEnumerator->Current().Casket()); 
}

//_____________________________________________________________________________________________________________________________

Cv_OutSourceDocket::Cv_OutSourceDocket(  Cv_Spritz *spritz, Cv_TypeBouquet *bouquet)
    :  Cv_Docket( spritz, bouquet), Cv_GraphTraversor< Cv_OutSourceDocket>( this)
{   
    m_SourceLog.SetStream( spritz);
	m_SourceLog << "#pragma once                                                                                    \n";
    m_SourceLog << "                                                                                                \n";
    m_SourceLog << "#include <stdint.h>                                                                             \n";
    m_SourceLog << "                                                                                                \n";
    m_SourceLog << "#pragma pack(push,1)                                                                            \n";
    m_SourceLog << "                                                                                                \n";
    m_SourceLog << "template < typename T>                                                                          \n";
    m_SourceLog << "T       *ReCast( const void *data) { return static_cast< T *>( const_cast< void *>( data)); }   \n";
    m_SourceLog << "                                                                                                \n";
}

//_____________________________________________________________________________________________________________________________

Cv_OutSourceDocket::~Cv_OutSourceDocket( void)
{
	m_SourceLog << "#pragma pack(pop)                                                \n\n";
    m_SourceLog.Flush();
}

//_____________________________________________________________________________________________________________________________


Cv_Enumerator< uint32_t>    *Cv_OutSourceDocket::StateEnumerator( uint32_t stateId) 
{ 
    Cv_TypeElement  dummy( "Dummy", CasketForIndex( stateId), nullptr);
    return new IndexEnumertor( this, dummy.CollectSubElementsEnumberator( this));
}

//_____________________________________________________________________________________________________________________________
    
void    Cv_OutSourceDocket::StoreDefinition( uint32_t casketcode )
{
    DoDepthTraversal( IndexForCasketCode( casketcode), this, &Cv_OutSourceDocket::EncodeCasket);

    Cv_TypeCasket       *casket = Bouquet()->CasketForCode( casketcode );
    std::string         rootStructString = Cv_Aid::CPPTypeStringCleanup( casket->StructString( Bouquet()));

    m_SourceLog << "                                                                                                    \n";
    m_SourceLog << "struct TypeFile                                                                                     \n";
    m_SourceLog << "{                                                                                                   \n";
    m_SourceLog << "    uint32_t                m_Offset;                                                               \n";
    m_SourceLog << "    uint32_t                m_CasketCode;                                                           \n";
    m_SourceLog << "    " << rootStructString << "       m_Root;                                                       \n";
    m_SourceLog << "};                                                                                                  \n";
    m_SourceLog << "                                                                                                    \n";
    m_SourceLog << "                                                                                                    \n";

    return;
}

//_____________________________________________________________________________________________________________________________

