#pragma once
#include <dwrite.h>
#include <vector>
#include <string>


#define SafeRelease(lpx) if(lpx!=NULL){lpx->Release();lpx=NULL;}//释放COM

class Resource_FontFileLoader : public IDWriteFontFileLoader
{
public:
    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontFileLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateStreamFromKey(
        void const* fontFileReferenceKey,
        UINT32 fontFileReferenceKeySize,
        OUT IDWriteFontFileStream** fontFileStream
    );

    // Gets the singleton loader instance.
    static IDWriteFontFileLoader* GetLoader()
    {
        return instance_;
    }

    static bool IsLoaderInitialized()
    {
        return instance_ != NULL;
    }

private:
    ULONG m_refCount;
    static IDWriteFontFileLoader* instance_;
};


class Resource_FontFileStream : public IDWriteFontFileStream
{
public:
    Resource_FontFileStream(int date, int len);

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontFileStream methods
    virtual HRESULT STDMETHODCALLTYPE ReadFileFragment(
        void const** fragmentStart, // [fragmentSize] in bytes
        UINT64 fileOffset,
        UINT64 fragmentSize,
        OUT void** fragmentContext
    );

    virtual void STDMETHODCALLTYPE ReleaseFileFragment(
        void* fragmentContext
    );

    virtual HRESULT STDMETHODCALLTYPE GetFileSize(
        OUT UINT64* fileSize
    );

    virtual HRESULT STDMETHODCALLTYPE GetLastWriteTime(
        OUT UINT64* lastWriteTime
    );


private:
    ULONG m_refCount;
    int m_data;      // [resourceSize_] in bytes
    int m_size;
};

class Resource_FontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
    // IUnknown methods
    Resource_FontFileEnumerator(int nextIndex, IDWriteFontFile* currentFile, void const* collectionKey, int collectionKeySize, IDWriteFactory* factory);
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontFileEnumerator methods
    virtual HRESULT STDMETHODCALLTYPE MoveNext(OUT BOOL* hasCurrentFile);
    virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(OUT IDWriteFontFile** fontFile);

private:
    ULONG m_refCount;
    IDWriteFactory* m_factory;
    IDWriteFontFile* m_currentFile;
    int m_collectionKeySize;
    int m_nextIndex;
    int m_collectionKey;
};

class Resource_FontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontCollectionLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
        IDWriteFactory* factory,
        void const* collectionKey,                      // [collectionKeySize] in bytes
        UINT32 collectionKeySize,
        OUT IDWriteFontFileEnumerator** fontFileEnumerator
    );
    // Gets the singleton loader instance.
    static IDWriteFontCollectionLoader* GetLoader()
    {
        return instance_;
    }

    static bool IsLoaderInitialized()
    {
        return instance_ != NULL;
    }

private:
    ULONG m_refCount;
    static IDWriteFontCollectionLoader* instance_;

};


///////////////////////////////////////////////////////////////////
// Acquires an additional reference, if non-null.
template <typename InterfaceType>
inline InterfaceType* SafeAcquire(InterfaceType* newObject)
{
    if (newObject != NULL)
        newObject->AddRef();

    return newObject;
}


// Sets a new COM object, releasing the old one.
template <typename InterfaceType>
inline void SafeSet(InterfaceType** currentObject, InterfaceType* newObject)
{
    SafeAcquire(newObject);
    SafeRelease(currentObject);
    currentObject = newObject;
}

// Maps exceptions to equivalent HRESULTs,
inline HRESULT ExceptionToHResult() throw()
{
    try
    {
        throw;  // Rethrow previous exception.
    }
    catch (std::bad_alloc&)
    {
        return E_OUTOFMEMORY;
    }
    catch (...)
    {
        return E_FAIL;
    }
}

typedef std::vector<std::wstring> MFCollection;

class MFFontCollectionLoader : public IDWriteFontCollectionLoader
{
public:
    MFFontCollectionLoader() : refCount_(0)
    {
    }

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontCollectionLoader methods
    virtual HRESULT STDMETHODCALLTYPE CreateEnumeratorFromKey(
        IDWriteFactory* factory,
        void const* collectionKey,                      // [collectionKeySize] in bytes
        UINT32 collectionKeySize,
        OUT IDWriteFontFileEnumerator** fontFileEnumerator
    );

    // Gets the singleton loader instance.
    static IDWriteFontCollectionLoader* GetLoader()
    {
        return instance_;
    }

    static bool IsLoaderInitialized()
    {
        return instance_ != NULL;
    }

private:
    ULONG refCount_;

    static IDWriteFontCollectionLoader* instance_;
};

class MFFontFileEnumerator : public IDWriteFontFileEnumerator
{
public:
    MFFontFileEnumerator(
        IDWriteFactory* factory
    );

    HRESULT Initialize(
        UINT const* collectionKey,    // [resourceCount]
        UINT32 keySize
    );

    ~MFFontFileEnumerator()
    {
        SafeRelease(currentFile_);
        SafeRelease(factory_);
    }

    // IUnknown methods
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef();
    virtual ULONG STDMETHODCALLTYPE Release();

    // IDWriteFontFileEnumerator methods
    virtual HRESULT STDMETHODCALLTYPE MoveNext(OUT BOOL* hasCurrentFile);
    virtual HRESULT STDMETHODCALLTYPE GetCurrentFontFile(OUT IDWriteFontFile** fontFile);

private:
    ULONG refCount_;

    IDWriteFactory* factory_;
    IDWriteFontFile* currentFile_;
    std::vector<std::wstring> filePaths_;
    size_t nextIndex_;
};

class MFFontContext
{
public:
    MFFontContext(IDWriteFactory* pFactory);
    ~MFFontContext();

    HRESULT Initialize();
    HRESULT Initialize2();

    HRESULT CreateFontCollection(
        MFCollection& newCollection,
        OUT IDWriteFontCollection** result
    );

    HRESULT CreateFontCollectionFromMem(
        _In_reads_bytes_(collectionKeySize) void const* collectionKey,
        UINT32 collectionKeySize,
        OUT IDWriteFontCollection** result
    );
private:
    // Not copyable or assignable.
    MFFontContext(MFFontContext const&);
    void operator=(MFFontContext const&);

    HRESULT InitializeInternal();
    HRESULT InitializeInternal2();
    IDWriteFactory* g_dwriteFactory;
    static std::vector<unsigned int> cKeys;

    // Error code from Initialize().
    HRESULT hr_;
    HRESULT hr2_;
};

class MFFontGlobals
{
public:
    MFFontGlobals() {}
    static unsigned int push(MFCollection& addCollection)
    {
        unsigned int ret = fontCollections.size();
        fontCollections.push_back(addCollection);
        return ret;
    }
    static std::vector<MFCollection>& collections()
    {
        return fontCollections;
    }
private:
    static std::vector<MFCollection> fontCollections;
};
