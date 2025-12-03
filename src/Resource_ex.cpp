#include "stdafx.h"

std::vector<BYTE> _bin_compress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen,
                                INT* nError)
{
    std::vector<BYTE> data;
    if (dwSize > 0) {
        INT padding = dwSize % 4;
        padding     = padding == 0 ? 0 : 4 - padding;
        UINT dstLen = dwSize + padding + 8;
        INT  width  = static_cast<INT>(sqrt(dstLen / 4.0));
        INT  height = width;
        while ((INT)(width * height * 4) < dstLen) {
            height += 1;
        }
        if (width > 0 && width < 65536 && height > 0 && height < 65536) {
            IWICBitmap* pHic = nullptr;
            HRESULT     hr   = g_Ri.pWICFactory->CreateBitmap(
                width, height,
                GUID_WICPixelFormat32bppBGRA,   // 注意这里使用的像素格式
                WICBitmapCacheOnLoad, &pHic);
            if (SUCCEEDED(hr)) {
                IWICBitmapLock* pBitmapLock = nullptr;
                WICRect         lockRect    = {0, 0, width, height};
                hr = pHic->Lock(&lockRect, WICBitmapLockWrite, &pBitmapLock);
                if (SUCCEEDED(hr) && pBitmapLock != nullptr) {
                    BYTE* pLockedData = nullptr;
                    hr                = pBitmapLock->GetDataPointer(&dstLen, &pLockedData);
                    if (SUCCEEDED(hr) && pLockedData != nullptr) {
                        UINT stride = 0;
                        pBitmapLock->GetStride(&stride);
                        // 计算实际数据起始位置
                        BYTE* pStart = pLockedData /*+ (height - 1) * stride*/;
                        // 设置数据头部信息
                        *(INT*)pStart       = dwSize;
                        UINT C32ADD         = Crc32_Addr(lpData, dwSize);
                        *(INT*)(pStart + 4) = C32ADD;
                        memcpy(pStart + 8, lpData, dwSize);
                        // 填充 padding
                        memset(pStart + 8 + dwSize, 0, padding);
                        // 解锁位图
                        pBitmapLock->Release();
                    }
                    LPVOID buffer = Ex_MemAlloc(4);
                    size_t ret    = 0;

                    _wic_savetobin(pHic, &buffer, &ret, nError);

                    data.resize(ret - 4);
                    RtlMoveMemory(data.data(), (LPVOID)((size_t)buffer + 4), ret - 4);
                    pHic->Release();
                    Ex_MemFree(buffer);


                    if (*nError == 0) {
                        INT magic = 1263556677;
                        if (lpKey == 0) {
                            lpKey = &magic;
                            dwLen = 4;
                        }
                        RC4(data.data(), data.size(), lpKey, dwLen);
                        data.insert(data.begin(), {0x45, 0x58, 0x50, 0x4B});

                        *nError = 0;
                    }
                }
            }
        }
    }

    return data;
}

void _res_pack_processing(const std::wstring path, const std::wstring root, bool bPntBits,
    std::vector<BYTE>& buffser, int& nCount)
{
    std::vector<CHAR> bin;
    std::wstring      t_path = path;
    std::wstring      t_root = root;
    if (!t_path.empty() && t_path.back() != L'\\') {
        t_path.push_back(L'\\');
    }
    HANDLE       hFileFind = (HANDLE)0;
    std::wstring szFile = FindFile(hFileFind, t_path + L"*.*", -1, 6);
    char         prop = 0;
    INT          nError = 0;
    EXATOM       atomName = 0;
    INT          len = 0;
    std::wstring wzName;
    LPSTREAM     lpStream = 0;
    UINT         w, h;
    while (szFile.size()) {
        prop = 0;
        wzName = t_path.substr(t_root.length()) + szFile;
        if (wzName.front() == L'\\') {
            wzName = wzName.substr(1);
        }
        Ex_ReadFile((path + L"\\" + szFile).c_str(), &bin);
        atomName = Ex_Atom(wzName.c_str());
        len = bin.size();

        if (len > 4 && bPntBits) {
            if (*((INT*)bin.data()) == PNG_HEADER) {
                LPSTREAM lpStream = _img_createfromstream_init(bin.data(), len, &nError);
                if (nError == 0) {
                    IWICStream* pIWICStream = nullptr;
                    g_Ri.pWICFactory->CreateStream(&pIWICStream);
                    pIWICStream->InitializeFromIStream(lpStream);
                    IWICBitmapDecoder* pDecoder = nullptr;
                    g_Ri.pWICFactory->CreateDecoderFromStream(
                        pIWICStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
                    IWICBitmapFrameDecode* pFrame = nullptr;
                    pDecoder->GetFrame(0, &pFrame);
                    WICPixelFormatGUID pixelFormat;
                    pFrame->GetPixelFormat(&pixelFormat);
                    IWICFormatConverter* pConverter = nullptr;
                    if (pixelFormat != GUID_WICPixelFormat32bppPBGRA) {

                        g_Ri.pWICFactory->CreateFormatConverter(&pConverter);
                        if (SUCCEEDED(pConverter->Initialize(pFrame, GUID_WICPixelFormat32bppPBGRA,
                            WICBitmapDitherTypeNone, NULL, 0.f,
                            WICBitmapPaletteTypeCustom))) {
                            pFrame->Release();
                        }
                        else {
                            pConverter->Release();
                        }
                    }
                    UINT width = 0, height = 0;
                    pFrame->GetSize(&w, &h);
                    UINT  stride = w * 4;
                    UINT  bufferSize = stride * h;
                    BYTE* pixels = new BYTE[bufferSize];
                    pConverter->CopyPixels(NULL, stride, bufferSize, pixels);
                    prop = PACKAGEHEADER_PNGBITS;
                    len = stride * h + 8;
                    bin.resize(len);

                    RtlMoveMemory(bin.data(), &w, sizeof(w));
                    RtlMoveMemory((void*)((size_t)bin.data() + sizeof(w)), &h, sizeof(h));
                    RtlMoveMemory((BYTE*)((size_t)bin.data() + sizeof(w) + sizeof(h)), pixels,
                        w * h * 4);
                    delete[] pixels;
                    pFrame->Release();
                    pDecoder->Release();
                    pIWICStream->Release();
                }
                lpStream->Release();
            }
        }

        // 存储原始路径信息
        // 格式: atomName(4字节) + prop(1字节) + 原始路径长度(4字节) + 数据长度(4字节) + 原始路径数据 + 文件数据
        std::vector<UCHAR> binatomName(4);
        RtlMoveMemory(binatomName.data(), &atomName, sizeof(atomName));
        std::vector<UCHAR> binprop(1);
        RtlMoveMemory(binprop.data(), &prop, sizeof(prop));

        // 原始路径长度(字节数)
        INT pathLen = (wzName.length() + 1) * sizeof(wchar_t); // 包含终止null字符
        std::vector<UCHAR> binpathLen(4);
        RtlMoveMemory(binpathLen.data(), &pathLen, sizeof(pathLen));

        // 数据长度
        std::vector<UCHAR> binlen(4);
        RtlMoveMemory(binlen.data(), &len, sizeof(len));

        // 原始路径数据
        std::vector<UCHAR> binpath(pathLen);
        RtlMoveMemory(binpath.data(), wzName.c_str(), pathLen);

        // 添加到缓冲区
        buffser.insert(buffser.end(), binatomName.begin(), binatomName.end());
        buffser.insert(buffser.end(), binprop.begin(), binprop.end());
        buffser.insert(buffser.end(), binpathLen.begin(), binpathLen.end());
        buffser.insert(buffser.end(), binlen.begin(), binlen.end());
        buffser.insert(buffser.end(), binpath.begin(), binpath.end());
        buffser.insert(buffser.end(), bin.begin(), bin.end());

        nCount = nCount + 1;
        szFile = FindFile(hFileFind, L"", -1, 6);
    }
    szFile = FindFile(hFileFind, t_path + L"*.*", 16, 6);
    while (szFile.length()) {
        if (szFile != L"." && szFile != L"..")
            _res_pack_processing((std::wstring(path) + szFile).c_str(), root, bPntBits, buffser,
                nCount);
        szFile = FindFile(hFileFind, L"", 16, 6);
    }
    if (hFileFind) ::FindClose(hFileFind);
}

void _res_pack(LPCWSTR root, LPCWSTR file, UCHAR byteHeader)
{
    std::vector<BYTE> buffers;
    int               nCount   = 0;
    bool              bPntBits = false;
    if (byteHeader == PACKAGEHEADER_THEME) {
        bPntBits = true;
    }
    _res_pack_processing(root, root, bPntBits, buffers, nCount);

    std::vector<unsigned char> binbyteHeader(1);
    RtlMoveMemory(binbyteHeader.data(), &byteHeader, sizeof(byteHeader));

    std::vector<unsigned char> binnCount(4);
    RtlMoveMemory(binnCount.data(), &nCount, sizeof(nCount));

    buffers.insert(buffers.begin(), binnCount.begin(), binnCount.end());
    buffers.insert(buffers.begin(), binbyteHeader.begin(), binbyteHeader.end());


    int nError = 0;
    buffers    = _bin_compress(buffers.data(), buffers.size(), 0, 0, &nError);

    if (nError == 0) {
        Ex_WriteFile(file, buffers.data(), buffers.size());
    }
}

void _bin_uncompress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen, LPVOID* retPtr,
                     size_t* retLen)
{
    INT    magic  = 1263556677;
    INT    nError = 0;
    LPVOID hImg   = nullptr;

    if (__get_int(lpData, 0) == magic) {
        LPVOID pData = Ex_MemAlloc(dwSize);
        if (pData == 0) {
            nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            RtlMoveMemory(pData, lpData, dwSize);
            if (lpKey == 0) {
                lpKey = &magic;
                dwLen = 4;
            }
            RC4((LPVOID)((size_t)pData + 4), dwSize - 4, lpKey, dwLen);
            __set_int(pData, 0, PNG_HEADER);
            LPSTREAM lpStream = _img_createfromstream_init(pData, dwSize, &nError);

            if (nError == 0) {

                IWICStream* pIWICStream = nullptr;
                g_Ri.pWICFactory->CreateStream(&pIWICStream);
                pIWICStream->InitializeFromIStream(lpStream);
                IWICBitmapDecoder* pDecoder = nullptr;
                g_Ri.pWICFactory->CreateDecoderFromStream(pIWICStream, NULL,
                                                          WICDecodeMetadataCacheOnLoad, &pDecoder);
                IWICBitmapFrameDecode* pFrame = nullptr;
                pDecoder->GetFrame(0, &pFrame);
                UINT w, h;
                pFrame->GetSize(&w, &h);
                BYTE* pixels = new BYTE[w * h * 4];
                pFrame->CopyPixels(0, w * 4, w * h * 4, pixels);

                INT srcLen = __get_int(pixels, 0);

                if (Crc32_Addr((LPVOID)((size_t)pixels + 8), srcLen) == __get_int(pixels, 4)) {
                    if (IsBadWritePtr(*retPtr, srcLen)) {
                        *retPtr = Ex_MemAlloc(srcLen);
                    }
                    RtlMoveMemory(*retPtr, (LPVOID)((size_t)pixels + 8), srcLen);
                    *retLen = srcLen;
                }
                else {
                    nError = ERROR_EX_CHECKSUM;
                }
                delete[] pixels;
                pFrame->Release();
                pDecoder->Release();
                pIWICStream->Release();
            }
            lpStream->Release();
            Ex_MemFree(pData);
        }
    }
    else {
        nError = ERROR_EX_UNSUPPORTED_TYPE;
    }
    Ex_SetLastError(nError);
}

HEXRES _res_unpack(LPVOID lpData, size_t dwDataLen, UCHAR byteHeader)
{
    LPVOID retPtr = nullptr;
    size_t retLen = 0;
    HEXRES tableFiles = nullptr;
    _bin_uncompress(lpData, dwDataLen, 0, 0, &retPtr, &retLen);
    if (retLen > 0) {
        if (__get_unsignedchar(retPtr, 0) == byteHeader) {
            INT count = __get_int(retPtr, 1);
            if (count > 0) {
                tableFiles = HashTable_Create(GetNearestPrime(count), pfnDefaultFreeData);
                if (tableFiles != 0) {
                    retPtr = (LPVOID)((size_t)retPtr + 5);
                    for (INT i = 0; i < count; i++) {
                        EXATOM atom = __get_int(retPtr, 0);
                        UCHAR prop = __get_unsignedchar(retPtr, 4);
                        INT pathLen = __get_int(retPtr, 5);
                        INT len = __get_int(retPtr, 9);

                        // 分配内存存储资源项（包含原始路径信息）
                        // 存储格式：atom(4) + prop(1) + pathLen(4) + len(4) + pathData + fileData
                        INT totalLen = 13 + pathLen + len;
                        LPVOID tmp = Ex_MemAlloc(totalLen);
                        if (tmp != 0) {
                            HashTable_Set((EX_HASHTABLE*)tableFiles, atom, (size_t)tmp);
                            RtlMoveMemory(tmp, retPtr, totalLen);
                        }

                        retPtr = (LPVOID)((size_t)retPtr + totalLen);
                    }
                }
            }
        }
    }
    return tableFiles;
}

HEXRES Ex_ResLoadFromMemory(LPVOID lpData, size_t dwDataLen)
{
    INT    nError = 0;
    HEXRES ret    = nullptr;
    if (dwDataLen > 0) {
        if (IsBadReadPtr(lpData, dwDataLen)) {
            nError = ERROR_EX_MEMORY_BADPTR;
        }
        else {
            ret = _res_unpack(lpData, dwDataLen, PACKAGEHEADER_FILES);
        }
    }
    else {
        nError = ERROR_EX_BAD_LENGTH;
    }
    Ex_SetLastError(nError);
    return ret;
}

HEXRES Ex_ResLoadFromFile(LPCWSTR lptszFile)
{
    INT    dwLen = lstrlenW(lptszFile);
    HEXRES ret   = nullptr;
    if (dwLen > 0) {
        std::vector<CHAR> data;
        Ex_ReadFile(lptszFile, &data);
        ret = Ex_ResLoadFromMemory(data.data(), data.size());
    }
    return ret;
}

BOOL Ex_ResFree(HEXRES hRes)
{
    if (hRes != 0) {
        return HashTable_Destroy((EX_HASHTABLE*)hRes);
    }
    return FALSE;
}

BOOL Ex_ResGetFileFromAtom(HEXRES hRes, EXATOM atomPath, LPVOID* lpFile, size_t* dwFileLen)
{
    LPVOID pData = nullptr;

    if (HashTable_Get((EX_HASHTABLE*)hRes, atomPath, (size_t*)&pData)) {
        if (pData != 0) {
            // 新格式：跳过atom(4) + prop(1) + pathLen(4) + len(4) + pathData
            INT pathLen = __get_int(pData, 5);
            *dwFileLen = __get_int(pData, 9);
            *lpFile = (LPVOID)((size_t)pData + 13 + pathLen);
        }
    }
    return *dwFileLen > 0;
}

BOOL Ex_ResGetOriginalPathFromAtom(HEXRES hRes, EXATOM atomPath, LPWSTR* lpwzPath, size_t* dwPathLen)
{
    LPVOID pData = nullptr;

    if (HashTable_Get((EX_HASHTABLE*)hRes, atomPath, (size_t*)&pData)) {
        if (pData != 0) {
            // 获取原始路径长度
            *dwPathLen = __get_int(pData, 5);

            // 分配内存并复制路径
            *lpwzPath = (LPWSTR)Ex_MemAlloc(*dwPathLen);
            if (*lpwzPath != nullptr) {
                LPVOID pPathData = (LPVOID)((size_t)pData + 13);
                RtlMoveMemory(*lpwzPath, pPathData, *dwPathLen);
                return TRUE;
            }
        }
    }
    return FALSE;
}

BOOL Ex_ResGetFile(HEXRES hRes, LPCWSTR lpwzPath, LPVOID* lpFile, size_t* dwFileLen)
{
    return Ex_ResGetFileFromAtom(hRes, Ex_Atom(lpwzPath), lpFile, dwFileLen);
}

BOOL Ex_ResWriteToDirectory(HEXRES hRes, LPCWSTR lpwzOutputDir)
{
    if (hRes == nullptr || lpwzOutputDir == nullptr) {
        return FALSE;
    }

    // 确保输出目录存在
    if (!std::filesystem::exists(lpwzOutputDir)) {
        if (!std::filesystem::create_directories(lpwzOutputDir)) {
            return FALSE;
        }
    }

    // 获取哈希表中的所有键值对
    std::vector<size_t> aryKey;
    std::vector<size_t> aryValue;
    size_t dwCount = HashTable_GetAllKeysAndValues((EX_HASHTABLE*)hRes, aryKey, aryValue);

    if (dwCount == 0) {
        return TRUE; // 没有资源可导出
    }

    INT nError = 0;

    for (size_t i = 0; i < dwCount; i++) {
        EXATOM atom = aryKey[i];
        LPVOID pData = (LPVOID)aryValue[i];

        if (pData == nullptr) {
            continue;
        }

        // 解析资源项（新格式）
        UCHAR prop = __get_unsignedchar(pData, 4); // 属性
        INT pathLen = __get_int(pData, 5);         // 原始路径长度
        INT len = __get_int(pData, 9);             // 数据长度
        LPVOID pathData = (LPVOID)((size_t)pData + 13); // 原始路径数据开始位置
        LPVOID fileData = (LPVOID)((size_t)pathData + pathLen); // 文件数据开始位置

        // 获取原始路径
        std::wstring wzOriginalPath((wchar_t*)pathData);

        // 构建完整的输出路径
        std::wstring outputPath = lpwzOutputDir;
        if (!outputPath.empty() && outputPath.back() != L'\\' && outputPath.back() != L'/') {
            outputPath += L'\\';
        }
        outputPath += wzOriginalPath;

        // 创建目录（如果需要）
        size_t pos = outputPath.find_last_of(L"\\/");
        if (pos != std::wstring::npos) {
            std::wstring dirPath = outputPath.substr(0, pos);
            if (!std::filesystem::exists(dirPath)) {
                std::filesystem::create_directories(dirPath);
            }
        }

        // 处理不同类型的资源
        if (prop == PACKAGEHEADER_PNGBITS) {
            // PNG位图资源 - 需要解码
            if (len >= 8) {
                UINT width = __get_unsignedint(fileData, 0);
                UINT height = __get_unsignedint(fileData, 4);
                BYTE* pixels = (BYTE*)((size_t)fileData + 8);

                // 创建位图并保存为PNG
                IWICBitmap* pBitmap = nullptr;
                HRESULT hr = g_Ri.pWICFactory->CreateBitmap(
                    width, height,
                    GUID_WICPixelFormat32bppPBGRA,
                    WICBitmapCacheOnLoad, &pBitmap);

                if (SUCCEEDED(hr)) {
                    WICRect rect = { 0, 0, (INT)width, (INT)height };
                    IWICBitmapLock* pLock = nullptr;
                    hr = pBitmap->Lock(&rect, WICBitmapLockWrite, &pLock);

                    if (SUCCEEDED(hr)) {
                        BYTE* pBuffer = nullptr;
                        UINT bufferSize = 0;
                        UINT stride = 0;
                        pLock->GetDataPointer(&bufferSize, &pBuffer);
                        pLock->GetStride(&stride);

                        // 复制像素数据
                        for (UINT y = 0; y < height; y++) {
                            BYTE* pDest = pBuffer + y * stride;
                            BYTE* pSrc = pixels + y * width * 4;
                            memcpy(pDest, pSrc, width * 4);
                        }

                        pLock->Release();

                        // 保存为PNG文件
                        IWICStream* pStream = nullptr;
                        hr = g_Ri.pWICFactory->CreateStream(&pStream);

                        if (SUCCEEDED(hr)) {
                            hr = pStream->InitializeFromFilename(outputPath.c_str(), GENERIC_WRITE);

                            if (SUCCEEDED(hr)) {
                                IWICBitmapEncoder* pEncoder = nullptr;
                                hr = g_Ri.pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);

                                if (SUCCEEDED(hr)) {
                                    hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);

                                    if (SUCCEEDED(hr)) {
                                        IWICBitmapFrameEncode* pFrameEncode = nullptr;
                                        hr = pEncoder->CreateNewFrame(&pFrameEncode, NULL);

                                        if (SUCCEEDED(hr)) {
                                            hr = pFrameEncode->Initialize(NULL);

                                            if (SUCCEEDED(hr)) {
                                                hr = pFrameEncode->SetSize(width, height);

                                                if (SUCCEEDED(hr)) {
                                                    WICPixelFormatGUID format = GUID_WICPixelFormat32bppPBGRA;
                                                    hr = pFrameEncode->SetPixelFormat(&format);

                                                    if (SUCCEEDED(hr)) {
                                                        hr = pFrameEncode->WriteSource(pBitmap, NULL);
                                                    }
                                                }
                                                pFrameEncode->Commit();
                                            }
                                            pFrameEncode->Release();
                                        }
                                        pEncoder->Commit();
                                    }
                                    pEncoder->Release();
                                }
                            }
                            pStream->Release();
                        }
                    }
                    pBitmap->Release();
                }
            }
        }
        else {
            // 普通文件资源 - 直接写入
            std::ofstream file(outputPath, std::ios::binary);
            if (file.is_open()) {
                file.write((const char*)fileData, len);
                file.close();
            }
            else {
                nError = ERROR_EX_INVALID_OBJECT;
            }
        }
    }
    Ex_SetLastError(nError);
    return nError == 0;
}