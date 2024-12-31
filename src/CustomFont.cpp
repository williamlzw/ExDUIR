#include "stdafx.h"

IDWriteFontCollectionLoader* Resource_FontCollectionLoader::instance_(
    new(std::nothrow) Resource_FontCollectionLoader());

IDWriteFontFileLoader* Resource_FontFileLoader::instance_(new(std::nothrow)
                                                              Resource_FontFileLoader());


HRESULT STDMETHODCALLTYPE Resource_FontFileLoader::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileLoader)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

// AddRef
ULONG STDMETHODCALLTYPE Resource_FontFileLoader::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

// Release
ULONG STDMETHODCALLTYPE Resource_FontFileLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&m_refCount);
    if (newCount == 0) delete this;

    return newCount;
}

// CreateStreamFromKey
//
//      Creates an IDWriteFontFileStream from a key that identifies the file. The
//      format and meaning of the key is entirely up to the loader implementation.
//      The only requirements imposed by DWrite are that a key must remain valid
//      for as long as the loader is registered. The same key must also uniquely
//      identify the same file, so for example you must not recycle keys so that
//      a key might represent different files at different times.
//
//      In this case the key is a UINT which identifies a font resources.
//
HRESULT STDMETHODCALLTYPE Resource_FontFileLoader::CreateStreamFromKey(
    void const* fontFileReferenceKey,   // [fontFileReferenceKeySize] in bytes
    UINT32 fontFileReferenceKeySize, OUT IDWriteFontFileStream** fontFileStream)
{
    *fontFileStream = NULL;
    // Make sure the key is the right size.
    if (fontFileReferenceKeySize != 8 || fontFileReferenceKeySize == 0) return E_FAIL;
    int  ptr  = (int)(fontFileReferenceKey);
    int* data = (int*)ptr;
    int* size = (int*)(ptr + 4);
    // cout << "Stream:" <<(int)*data << "   Len:" << *size << "\n";
    Resource_FontFileStream* stream = new Resource_FontFileStream(*data, *size);
    if (!stream) return E_FAIL;
    *fontFileStream = stream;
    return S_OK;
}


Resource_FontFileStream::Resource_FontFileStream(int date, int len)
{
    // m_refCount = 0;
    m_data = date;
    m_size = len;
}

// IUnknown methods
HRESULT STDMETHODCALLTYPE Resource_FontFileStream::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileStream)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

ULONG STDMETHODCALLTYPE Resource_FontFileStream::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE Resource_FontFileStream::Release()
{
    ULONG newCount = InterlockedDecrement(&m_refCount);
    if (newCount == 0) delete this;

    return newCount;
}

// IDWriteFontFileStream methods
HRESULT STDMETHODCALLTYPE Resource_FontFileStream::ReadFileFragment(
    void const** fragmentStart,   // [fragmentSize] in bytes
    UINT64 fileOffset, UINT64 fragmentSize, OUT void** fragmentContext)
{
    if (fileOffset <= m_size && fragmentSize <= m_size - fileOffset) {
        *fragmentStart   = (void*)(m_data + fileOffset);
        *fragmentContext = NULL;
        // cout << "Fragment:" << (int)*fragmentStart << "\n";
        return S_OK;
    }
    *fragmentStart   = NULL;
    *fragmentContext = NULL;
    return E_FAIL;
}

void STDMETHODCALLTYPE Resource_FontFileStream::ReleaseFileFragment(void* fragmentContext) {}

HRESULT STDMETHODCALLTYPE Resource_FontFileStream::GetFileSize(OUT UINT64* fileSize)
{
    *fileSize = m_size;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE Resource_FontFileStream::GetLastWriteTime(OUT UINT64* lastWriteTime)
{
    // The concept of last write time does not apply to this loader.
    *lastWriteTime = 0;
    return E_NOTIMPL;
}


Resource_FontFileEnumerator::Resource_FontFileEnumerator(int              nextIndex,
                                                         IDWriteFontFile* currentFile,
                                                         void const*      collectionKey,
                                                         int              collectionKeySize,
                                                         IDWriteFactory*  factory)
{
    m_refCount          = 0;
    m_nextIndex         = nextIndex;
    m_currentFile       = currentFile;
    m_collectionKey     = (int)collectionKey;
    m_collectionKeySize = collectionKeySize;
    m_factory           = factory;
    if (factory) {
        factory->AddRef();
    }
}

HRESULT STDMETHODCALLTYPE Resource_FontFileEnumerator::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}


ULONG STDMETHODCALLTYPE Resource_FontFileEnumerator::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE Resource_FontFileEnumerator::Release()
{
    ULONG newCount = InterlockedDecrement(&m_refCount);
    if (newCount == 0) delete this;

    return newCount;
}

HRESULT STDMETHODCALLTYPE Resource_FontFileEnumerator::MoveNext(OUT BOOL* hasCurrentFile)
{
    HRESULT hr      = S_OK;
    *hasCurrentFile = false;
    SafeRelease(m_currentFile);
    if (m_nextIndex < m_collectionKeySize) {
        // m_currentFile = (IDWriteFontFile*)(m_collectionKey+(m_nextIndex*4));

        int* fliepath = (int*)(m_collectionKey + (m_nextIndex * 8));
        // cout << "MoveNext:" << (int)*fliepath<< "\n";
        HRESULT hr = m_factory->CreateCustomFontFileReference(
            (void*)fliepath, 8, Resource_FontFileLoader::GetLoader(), &m_currentFile);

        if (SUCCEEDED(hr)) {
            *hasCurrentFile = true;
            ++m_nextIndex;
        }
        else {
            return E_FAIL;
        }
    }
    return hr;
}

HRESULT STDMETHODCALLTYPE
Resource_FontFileEnumerator::GetCurrentFontFile(OUT IDWriteFontFile** fontFile)
{
    *fontFile = m_currentFile;
    if (m_currentFile != nullptr) {
        m_currentFile->AddRef();
        return S_OK;
    }
    return E_FAIL;
}

HRESULT STDMETHODCALLTYPE Resource_FontCollectionLoader::QueryInterface(REFIID iid,
                                                                        void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

ULONG STDMETHODCALLTYPE Resource_FontCollectionLoader::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

ULONG STDMETHODCALLTYPE Resource_FontCollectionLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&m_refCount);
    if (newCount == 0) delete this;

    return newCount;
}

HRESULT STDMETHODCALLTYPE Resource_FontCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* factory, void const* collectionKey, UINT32 collectionKeySize,
    OUT IDWriteFontFileEnumerator** fontFileEnumerator)
{
    Resource_FontFileEnumerator* pFontFileEnumerator =
        new Resource_FontFileEnumerator(0, 0, collectionKey, collectionKeySize / 8, factory);
    *fontFileEnumerator = pFontFileEnumerator;
    return S_OK;
}


///////////////////////////////////////////////////////////////////////////
IDWriteFontCollectionLoader* MFFontCollectionLoader::instance_(new(std::nothrow)
                                                                   MFFontCollectionLoader());

HRESULT STDMETHODCALLTYPE MFFontCollectionLoader::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontCollectionLoader)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

ULONG STDMETHODCALLTYPE MFFontCollectionLoader::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE MFFontCollectionLoader::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0) delete this;

    return newCount;
}

HRESULT STDMETHODCALLTYPE MFFontCollectionLoader::CreateEnumeratorFromKey(
    IDWriteFactory* factory,
    void const*     collectionKey,   // [collectionKeySize] in bytes
    UINT32 collectionKeySize, OUT IDWriteFontFileEnumerator** fontFileEnumerator)
{
    *fontFileEnumerator = NULL;

    HRESULT hr = S_OK;

    if (collectionKeySize % sizeof(UINT) != 0) return E_INVALIDARG;

    MFFontFileEnumerator* enumerator = new (std::nothrow) MFFontFileEnumerator(factory);
    if (enumerator == NULL) return E_OUTOFMEMORY;

    UINT const*  mfCollectionKey = static_cast<UINT const*>(collectionKey);
    UINT32 const mfKeySize       = collectionKeySize;

    hr = enumerator->Initialize(mfCollectionKey, mfKeySize);

    if (FAILED(hr)) {
        delete enumerator;
        return hr;
    }

    *fontFileEnumerator = SafeAcquire(enumerator);

    return hr;
}

// ------------------------------ MFFontFileEnumerator
// ----------------------------------------------------------

MFFontFileEnumerator::MFFontFileEnumerator(IDWriteFactory* factory)
    : refCount_(0)
    , factory_(SafeAcquire(factory))
    , currentFile_()
    , nextIndex_(0)
{}

HRESULT MFFontFileEnumerator::Initialize(UINT const* collectionKey,   // [resourceCount]
                                         UINT32      keySize)
{
    try {
        // dereference collectionKey in order to get index of collection in
        // MFFontGlobals::fontCollections vector
        UINT cPos = *collectionKey;
        for (MFCollection::iterator it = MFFontGlobals::collections()[cPos].begin();
             it != MFFontGlobals::collections()[cPos].end(); ++it) {
            filePaths_.push_back(it->c_str());
        }
    }
    catch (...) {
        return ExceptionToHResult();
    }
    return S_OK;
}

HRESULT STDMETHODCALLTYPE MFFontFileEnumerator::QueryInterface(REFIID iid, void** ppvObject)
{
    if (iid == IID_IUnknown || iid == __uuidof(IDWriteFontFileEnumerator)) {
        *ppvObject = this;
        AddRef();
        return S_OK;
    }
    else {
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }
}

ULONG STDMETHODCALLTYPE MFFontFileEnumerator::AddRef()
{
    return InterlockedIncrement(&refCount_);
}

ULONG STDMETHODCALLTYPE MFFontFileEnumerator::Release()
{
    ULONG newCount = InterlockedDecrement(&refCount_);
    if (newCount == 0) delete this;

    return newCount;
}

HRESULT STDMETHODCALLTYPE MFFontFileEnumerator::MoveNext(OUT BOOL* hasCurrentFile)
{
    HRESULT hr = S_OK;

    *hasCurrentFile = FALSE;
    SafeRelease(currentFile_);

    if (nextIndex_ < filePaths_.size()) {
        hr = factory_->CreateFontFileReference(filePaths_[nextIndex_].c_str(), NULL, &currentFile_);

        if (SUCCEEDED(hr)) {
            *hasCurrentFile = TRUE;

            ++nextIndex_;
        }
    }

    return hr;
}

HRESULT STDMETHODCALLTYPE MFFontFileEnumerator::GetCurrentFontFile(OUT IDWriteFontFile** fontFile)
{
    *fontFile = SafeAcquire(currentFile_);

    return (currentFile_ != NULL) ? S_OK : E_FAIL;
}

// ---------------------------------------- MFFontContext
// ---------------------------------------------------------

MFFontContext::MFFontContext(IDWriteFactory* pFactory)
    : hr_(S_FALSE)
    , hr2_(S_FALSE)
    , g_dwriteFactory(pFactory)
{}

MFFontContext::~MFFontContext()
{
    if (MFFontCollectionLoader::IsLoaderInitialized()) {
        // hr_ =
        // g_dwriteFactory->UnregisterFontCollectionLoader(MFFontCollectionLoader::GetLoader());
    }
    if (Resource_FontFileLoader::IsLoaderInitialized()) {
        // hr_ = g_dwriteFactory->UnregisterFontFileLoader(Resource_FontFileLoader::GetLoader());
    }
}

HRESULT MFFontContext::Initialize()
{
    if (hr_ == S_FALSE) {
        hr_ = InitializeInternal();
    }
    return hr_;
}

HRESULT MFFontContext::Initialize2()
{
    if (hr2_ == S_FALSE) {
        hr2_ = InitializeInternal2();
    }
    return hr2_;
}

HRESULT MFFontContext::InitializeInternal()
{
    if (!MFFontCollectionLoader::IsLoaderInitialized()) {
        return E_FAIL;
    }

    // Register our custom loader with the factory object.
    return g_dwriteFactory->RegisterFontCollectionLoader(MFFontCollectionLoader::GetLoader());
}

HRESULT MFFontContext::InitializeInternal2()
{
    HRESULT hr = S_OK;

    if (!Resource_FontFileLoader::IsLoaderInitialized()) {
        return E_FAIL;
    }

    // Register our custom loader with the factory object.
    hr = g_dwriteFactory->RegisterFontFileLoader(Resource_FontFileLoader::GetLoader());

    if (!Resource_FontCollectionLoader::IsLoaderInitialized()) {
        return E_FAIL;
    }

    // Register our custom loader with the factory object.
    hr = g_dwriteFactory->RegisterFontCollectionLoader(Resource_FontCollectionLoader::GetLoader());

    return hr;
}

HRESULT MFFontContext::CreateFontCollection(MFCollection&               newCollection,
                                            OUT IDWriteFontCollection** result)
{
    *result = NULL;

    HRESULT hr = S_OK;

    // save new collection in MFFontGlobals::fontCollections vector
    UINT m_collectionKey = MFFontGlobals::push(newCollection);
    cKeys.push_back(m_collectionKey);
    const void* fontCollectionKey = &cKeys.back();
    UINT32      keySize           = sizeof(m_collectionKey);


    hr = Initialize();
    if (FAILED(hr)) return hr;

    hr = g_dwriteFactory->CreateCustomFontCollection(MFFontCollectionLoader::GetLoader(),
                                                     fontCollectionKey, keySize, result);

    return hr;
}

HRESULT MFFontContext::CreateFontCollectionFromMem(
    _In_reads_bytes_(collectionKeySize) void const* collectionKey, UINT32 collectionKeySize,
    OUT IDWriteFontCollection** result)
{
    *result = NULL;

    HRESULT hr = S_OK;

    hr = Initialize2();
    if (FAILED(hr)) return hr;

    hr = g_dwriteFactory->CreateCustomFontCollection(Resource_FontCollectionLoader::GetLoader(),
                                                     collectionKey, collectionKeySize, result);

    return hr;
}

std::vector<unsigned int> MFFontContext::cKeys = std::vector<unsigned int>(0);

// ----------------------------------- MFFontGlobals
// ---------------------------------------------------------

std::vector<MFCollection> MFFontGlobals::fontCollections = std::vector<MFCollection>(0);
