//_______________________________________________ cv_typecask.cpp _______________________________________________________________

#include    "cove/barn/cv_include.h"
#include    "cove/silo/cv_typecask.h"

//_____________________________________________________________________________________________________________________________

Cv_TypeCaskData  *Cv_TypeCaskData::FindStaticInstance( Cv_TypeCaskData *cd)
{
    CV_FNPASS(() << *cd)

    Cv_TypeCask     *typeCask = static_cast< Cv_TypeCask *>( Cv_TypeCaskMgr::Singleton()->Find( cd));

    CV_FNLOG(() << typeCask->TypeString())

    return typeCask;
}
   
//_____________________________________________________________________________________________________________________________

Cv_TypeCask::Cv_TypeCask( void)
    :   m_Id( 0), m_DerivedCasks( nullptr) 
{
    Cv_TypeCaskMgr      *caskMgr = Cv_TypeCaskMgr::PrivSingleton();
    m_Id = caskMgr->m_Size;
    ++caskMgr->m_Size;
    caskMgr->Push( this);
}

//_____________________________________________________________________________________________________________________________
    
Cv_TypeCask::~Cv_TypeCask( void)
{
    if ( m_DerivedCasks)
        delete m_DerivedCasks;
}

//_____________________________________________________________________________________________________________________________

void    Cv_TypeCask::Freeze( void)
{
    std::sort( m_DerivedCasks->begin(), m_DerivedCasks->end(), DerivedCaskLessTypeInfo());
    return;
}

//_____________________________________________________________________________________________________________________________

uint32_t    Cv_TypeCask::FindDerivedCask( Cv_TypeCask *cask)
{
    CV_FNPASS(())
    
    std::vector< DerivedCask>::iterator   dCaskIt = std::lower_bound( m_DerivedCasks->begin(), m_DerivedCasks->end(), cask, DerivedCaskLessTypeInfo());
    CV_ERROR_ASSERT(( dCaskIt != m_DerivedCasks->end()) && !DerivedCaskLessTypeInfo()( cask, *dCaskIt))
    return  uint32_t( dCaskIt -m_DerivedCasks->begin());
}

//_____________________________________________________________________________________________________________________________

void    Cv_TypeCask::DoInitialize( void) 
{ 
    SetName( TypeString()); 
    m_DerivedCasks = new std::vector< DerivedCask>();
    return;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCaskMgr::Cv_TypeCaskMgr( void)
    : m_Size( 0), m_TypeCasks( nullptr), m_TypeInfoCasks( nullptr)
{
}

//_____________________________________________________________________________________________________________________________

 Cv_TypeCaskMgr::~Cv_TypeCaskMgr( void)
 {
    if ( m_TypeCasks)
        delete m_TypeCasks;
    if ( m_TypeInfoCasks)
        delete m_TypeInfoCasks;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCaskMgr   *Cv_TypeCaskMgr::PrivSingleton( void)
{
    static Cv_TypeCaskMgr      s_Singleton;

    return &s_Singleton;
}

//_____________________________________________________________________________________________________________________________

void    Cv_TypeCaskMgr::DoProcess( void)
{
    CV_FNPASS(())
    
    uint32_t            numProc = 0;
    m_TypeCasks = new std::vector< Cv_TypeCaskData *>();
    m_TypeInfoCasks = new std::vector< Cv_TypeCask *>();

    for ( Cv_TypeCask   *cask = Top(); cask && ( m_Size > m_TypeCasks->size())  ; cask = cask->GetBelow(), ++numProc) 
    {
        cask->DoInitialize();
        m_TypeCasks->push_back( cask);
        m_TypeInfoCasks->push_back( cask);
    }
    
    uint32_t            numProc1 = numProc;
    for ( Cv_TypeCask   *cask = Top(); cask && numProc1; cask = cask->GetBelow(), --numProc1) 
        cask->RegisterDerivedToChildrenBases( cask, 0);
        
    std::sort( m_TypeCasks->begin(), m_TypeCasks->end(), Cv_TLess<void>());
    std::sort( m_TypeInfoCasks->begin(), m_TypeInfoCasks->end(), Cv_TypeCask::LessTypeInfo());
    
    for ( Cv_TypeCask   *cask = Top(); cask && numProc; cask = cask->GetBelow(), --numProc) 
        cask->Freeze();
    return;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCaskMgr   *Cv_TypeCaskMgr::Singleton( void)
{
    CV_FNPASS(())
    
    static Cv_TypeCaskMgr   *typeCaskMgr = PrivSingleton();
    
    if ( typeCaskMgr->m_TypeCasks && ( typeCaskMgr->m_Size == typeCaskMgr->m_TypeCasks->size()))
        return typeCaskMgr;
    
    typeCaskMgr->DoProcess();
    return typeCaskMgr;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCask     *Cv_TypeCaskMgr::Find( const std::type_info *typeInfo) const
{
    CV_FNPASS(())
    
    std::vector< Cv_TypeCask *>::const_iterator   caskIt = std::lower_bound( m_TypeInfoCasks->begin(), m_TypeInfoCasks->end(), typeInfo, Cv_TypeCask::LessTypeInfo());
    CV_ERROR_ASSERT(( caskIt != m_TypeInfoCasks->end()) && !Cv_TypeCask::LessTypeInfo()( typeInfo, *caskIt))
    return *caskIt;
}

//_____________________________________________________________________________________________________________________________

Cv_TypeCask     *Cv_TypeCaskMgr::Find( Cv_TypeCaskData *typeData) const
{
    CV_FNPASS(())
    
    std::vector< Cv_TypeCaskData *>::const_iterator   caskIt = std::lower_bound( m_TypeCasks->begin(), m_TypeCasks->end(), typeData, Cv_TLess<void>());
    CV_ERROR_ASSERT(( caskIt != m_TypeCasks->end()) && !Cv_TLess<void>()( typeData, *caskIt))
    return static_cast< Cv_TypeCask *>( *caskIt);
}

//_____________________________________________________________________________________________________________________________
   
std::vector< Cv_TypeElement>    Cv_StructAdaptorBase::CollectSubElements( Cv_Docket *xDocket, Cv_StructCaskBase *structCask, const void *thisPtr) const 
{
    std::vector< Cv_TypeElement>        elems( FieldSz());
    for ( uint32_t i = 0; i < FieldSz(); ++i)
    {
        FieldCasketData     *fieldData = &FieldData( i);
        Cv_TypeCasket       *fieldCasket = xDocket->Bouquet()->CasketForCode( fieldData->m_CasketCode);
        const void          *fieldPtr = structCask->FieldStructptr( thisPtr, fieldData->m_FieldId);
        const char          *fieldName = structCask->FieldName( fieldData->m_FieldId);
        elems[ i] = Cv_TypeElement( fieldName, fieldCasket, fieldPtr);   
    }
    return elems; 
}

//_____________________________________________________________________________________________________________________________

std::vector< Cv_StructCaskBase::FieldCasket>   Cv_StructCaskBase::FetchSubCaskets( Cv_TypeBouquet *bouquet, const void *thisPtr)
    {
    std::vector< FieldCasket>       subCaskets( m_FieldCasks.Size());
    for ( uint32_t i = 0; i < m_FieldCasks.Size(); ++i)
        subCaskets[ i] = FieldCasket( bouquet->LocateCasket( m_FieldCasks[ i].m_Cask, FieldStructptr( thisPtr, i)), i);
    std::stable_sort( subCaskets.begin(), subCaskets.end(), typename FieldCasket::FlexiLess());
    return  subCaskets;
}


//_____________________________________________________________________________________________________________________________
        
void    Cv_StructCaskBase::ProcessFieldCasks( void)
{
    std::stable_sort( m_FieldCasks.Begin(), m_FieldCasks.End(), typename FieldCask::OffsetSignLess());
    return;
}

//_____________________________________________________________________________________________________________________________

