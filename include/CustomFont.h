#ifndef CUSTOMFONT_H_
#define CUSTOMFONT_H_
///////////////////////////////////////////////////////////////////////////////
template <typename T>
class ExUnknownImpl : public T {
 public:
  ExUnknownImpl() : m_ref(1) {}
  virtual ~ExUnknownImpl() {}

  STDMETHOD_(ULONG, AddRef)(void) override {
    return InterlockedIncrement(&m_ref);
  }
  STDMETHOD_(ULONG, Release)(void) override {
    long ref = InterlockedDecrement(&m_ref);
    if (ref == 0) {
      delete this;
    }
    return ref;
  }

 private:
  long m_ref;
};

template <typename T, typename T_FOR_DESTROY>
class ExUnknownImplEx : public ExUnknownImpl<T> {
 public:
  STDMETHOD_(ULONG, Release)(void) override {
    long ref = InterlockedDecrement(&__super::m_ref);
    if (ref == 0) {
      delete static_cast<T>(this);
    }
    return ref;
  }
};

#define EX_DECLEAR_INTERFACE_BEGIN() \
  STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override {
#define EX_DECLEAR_INTERFACE(INTERFACE)        \
  if (IsEqualIID(riid, __uuidof(INTERFACE))) { \
    *ppvObject = this;                         \
    this->AddRef();                            \
    return S_OK;                               \
  }

#define EX_DECLEAR_INTERFACE_2(_GUID_) \
  if (IsEqualIID(riid, _GUID_)) {      \
    *ppvObject = this;                 \
    this->AddRef();                    \
    return S_OK;                       \
  }

#define EX_DECLEAR_INTERFACE_EX(_GUID_, PTR) \
  if (IsEqualIID(riid, _GUID_)) {            \
    *ppvObject = PTR;                        \
    this->AddRef();                          \
    return S_OK;                             \
  }

#define EX_DECLEAR_INTERFACE_END()                                 \
  return E_NOINTERFACE;                                            \
  }                                                                \
  template <class T>                                               \
  inline HRESULT STDMETHODCALLTYPE QueryInterface(T** ppvObject) { \
    return this->QueryInterface(__uuidof(T), (void**)ppvObject);   \
  }
/////////////////////////////////////////////////////////////////////////////
/// 数据块
struct ExData {
  /// 数据指针
  byte* data;

  /// 数据大小
  size_t size;

  ExData() : data(nullptr), size(0) {}
  ExData(byte* data, size_t size) : data(data), size(size) {}
};
///////////////////////////////////////////////////////////////////////////////
template <class T>
class ExLazySingleton {
 private:
  static T* s_instance;

 public:
  static bool SetInstance(T* instance) {
    bool is_null = s_instance == nullptr;
    if (is_null) {
      s_instance = instance;
    }
    return is_null;
  }

  static T* GetInstance() {
    if (s_instance == nullptr) {
      s_instance = new T();
    }
    return s_instance;
  }

  template <class... Args>
  static T* GetInstance(Args... args) {
    if (s_instance == nullptr) {
      s_instance = new T(args...);
    }
    return s_instance;
  }

  static T* Instance() { return s_instance; }

  static T* DetachInstance() {
    T* temp = s_instance;
    s_instance = nullptr;
    return temp;
  }

  static void ClearInstance(bool delete_instance = true) {
    if (s_instance != nullptr) {
      if (delete_instance) {
        delete s_instance;
      }
      s_instance = nullptr;
    }
  }
};

template <class T>
T* ExLazySingleton<T>::s_instance = nullptr;
///////////////////////////////////////////////////////////////////////////////
#define SafeDelete(ptr) \
  if (ptr != nullptr) { \
    delete ptr;         \
    ptr = NULL;         \
  }
#define SafeDeleteArry(ptr) \
  if (ptr != nullptr) {     \
    delete[] ptr;           \
    ptr = NULL;             \
  }
#define SafeRelease(lpx) \
  if (lpx != NULL) {     \
    lpx->Release();      \
    lpx = NULL;          \
  }  // 释放COM
///////////////////////////////////////////////////////////////
	// 字体文件流对象
class ExFontFileStream : public ExUnknownImpl<IDWriteFontFileStream> {
 public:
  EX_DECLEAR_INTERFACE_BEGIN();
  EX_DECLEAR_INTERFACE(IUnknown);
  EX_DECLEAR_INTERFACE(IDWriteFontFileStream);
  EX_DECLEAR_INTERFACE_END();

  ExFontFileStream(ExData* data) { m_data = *data; }

  virtual HRESULT __stdcall ReadFileFragment(void const** fragmentStart,
                                             UINT64 fileOffset,
                                             UINT64 fragmentSize,
                                             void** fragmentContext) override {
    *fragmentContext = nullptr;

    // 在数据区内
    if (fileOffset < m_data.size && fragmentSize <= m_data.size - fileOffset) {
      *fragmentStart = m_data.data + fileOffset;
      return S_OK;
    } else {
      *fragmentStart = nullptr;
      return E_FAIL;
    }
  }

  virtual void __stdcall ReleaseFileFragment(void* fragmentContext) override {}
  virtual HRESULT __stdcall GetFileSize(UINT64* fileSize) override {
    *fileSize = m_data.size;
    return S_OK;
  }
  virtual HRESULT __stdcall GetLastWriteTime(UINT64* lastWriteTime) override {
    *lastWriteTime = 0;
    return E_NOTIMPL;
  }

 private:
  ExData m_data;
};

// 字体文件加载器对象
class ExFontFileLoader : public ExUnknownImpl<IDWriteFontFileLoader> {
  friend ExLazySingleton<ExFontFileLoader>;

 public:
  EX_DECLEAR_INTERFACE_BEGIN();
  EX_DECLEAR_INTERFACE(IUnknown);
  EX_DECLEAR_INTERFACE(IDWriteFontFileLoader);
  EX_DECLEAR_INTERFACE_END();

  virtual HRESULT __stdcall CreateStreamFromKey(
      void const* fontFileReferenceKey, UINT32 fontFileReferenceKeySize,
      IDWriteFontFileStream** fontFileStream) override {
    if (fontFileReferenceKey != nullptr &&
        fontFileReferenceKeySize == sizeof(ExData)) {
      ExData* data = (ExData*)fontFileReferenceKey;
      if (data->data != nullptr && data->size > 0) {
        *fontFileStream = new ExFontFileStream(data);
        return (*fontFileStream) ? S_OK : E_FAIL;
      }
    }
    return E_FAIL;
  }
};

// 字体文件枚举器对象
class ExFontFileEnumerator : public ExUnknownImpl<IDWriteFontFileEnumerator> {
 public:
  EX_DECLEAR_INTERFACE_BEGIN();
  EX_DECLEAR_INTERFACE(IUnknown);
  EX_DECLEAR_INTERFACE(IDWriteFontFileEnumerator);
  EX_DECLEAR_INTERFACE_END();

  ExFontFileEnumerator(IDWriteFactory* pFactory, ExData* aDatas, UINT cDatas)
      : m_factory(pFactory),
        m_data_list(aDatas),
        m_data_count(cDatas),
        m_current_file(nullptr),
        m_next_index(0) {}

  virtual HRESULT __stdcall MoveNext(BOOL* hasCurrentFile) override {
    HRESULT hr = S_OK;

    // 释放当前字体文件
    SafeRelease(m_current_file);
    *hasCurrentFile = FALSE;

    // 有下一个字体文件
    if (m_next_index < m_data_count) {
      ExData* data = m_data_list + m_next_index;
      if (data->data != nullptr && data->size > 0) {
        // 创建字体文件流
        hr = m_factory->CreateCustomFontFileReference(
            data, sizeof(ExData), ExLazySingleton<ExFontFileLoader>::Instance(),
            &m_current_file);
        if (SUCCEEDED(hr)) {
          *hasCurrentFile = TRUE;
          m_next_index++;
        }
      } else
        hr = E_FAIL;
    }

    return hr;
  }

  virtual HRESULT __stdcall GetCurrentFontFile(
      IDWriteFontFile** fontFile) override {
    *fontFile = m_current_file;
    if (m_current_file == nullptr) return E_FAIL;
    m_current_file->AddRef();
    return S_OK;
  }

 private:
  IDWriteFactory* m_factory;
  ExData* m_data_list;
  UINT m_data_count;
  IDWriteFontFile* m_current_file;
  UINT m_next_index;
};

// 字体集加载器对象
class ExFontCollectionLoader
    : public ExUnknownImpl<IDWriteFontCollectionLoader> {
  friend ExLazySingleton<ExFontCollectionLoader>;

 public:
  EX_DECLEAR_INTERFACE_BEGIN();
  EX_DECLEAR_INTERFACE(IUnknown);
  EX_DECLEAR_INTERFACE(IDWriteFontCollectionLoader);
  EX_DECLEAR_INTERFACE_END();

  virtual HRESULT __stdcall CreateEnumeratorFromKey(
      IDWriteFactory* factory, void const* collectionKey,
      UINT32 collectionKeySize,
      IDWriteFontFileEnumerator** fontFileEnumerator) override {
    if (!collectionKey) return E_FAIL;
    if (collectionKeySize % sizeof(ExData) != 0) return E_FAIL;

    *fontFileEnumerator = new ExFontFileEnumerator(
        factory, (ExData*)collectionKey, collectionKeySize / sizeof(ExData));

    return (*fontFileEnumerator) ? S_OK : E_FAIL;
  }
};


#endif  // 
