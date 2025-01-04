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
    std::wstring szFile    = FindFile(hFileFind, t_path + L"*.*", -1, 6);
    char         prop      = 0;
    INT          nError    = 0;
    EXATOM       atomName  = 0;
    INT          len       = 0;
    std::wstring wzName;
    LPSTREAM     lpStream = 0;
    UINT         w, h;
    while (szFile.size()) {
        prop   = 0;
        wzName = t_path.substr(t_root.length()) + szFile;
        if (wzName.front() == L'\\') {
            wzName = wzName.substr(1);
        }
        Ex_ReadFile((path + L"\\" + szFile).c_str(), &bin);
        atomName = Ex_Atom(wzName.c_str());
        len      = bin.size();

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
                    UINT  stride     = w * 4;
                    UINT  bufferSize = stride * h;
                    BYTE* pixels     = new BYTE[bufferSize];
                    pConverter->CopyPixels(NULL, stride, bufferSize, pixels);
                    prop = PACKAGEHEADER_PNGBITS;
                    len  = stride * h + 8;
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
        std::vector<UCHAR> binatomName(4);
        RtlMoveMemory(binatomName.data(), &atomName, sizeof(atomName));
        std::vector<UCHAR> binprop(1);
        RtlMoveMemory(binprop.data(), &prop, sizeof(prop));
        std::vector<UCHAR> binlen(4);
        RtlMoveMemory(binlen.data(), &len, sizeof(len));
        buffser.insert(buffser.end(), binatomName.begin(), binatomName.end());
        buffser.insert(buffser.end(), binprop.begin(), binprop.end());
        buffser.insert(buffser.end(), binlen.begin(), binlen.end());
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
                unsigned char* pixels = new unsigned char[w * h * 4];
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
    LPVOID retPtr     = nullptr;
    size_t retLen     = 0;
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
                        
                        INT len = __get_int(retPtr, 5) + 5;   // byteProp + len + data
                        if (len > 5) {
                            LPVOID tmp = Ex_MemAlloc(len);
                            if (tmp != 0) {
                                HashTable_Set((EX_HASHTABLE*)tableFiles, atom, (size_t)tmp);
                                RtlMoveMemory(tmp, (LPVOID)((size_t)retPtr + 4), len);
                            }
                        }
                        retPtr = (LPVOID)((size_t)retPtr + 4 + len);
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

void Ex_ResFree(HEXRES hRes)
{
    if (hRes != 0) {
        HashTable_Destroy((EX_HASHTABLE*)hRes);
    }
}

BOOL Ex_ResGetFileFromAtom(HEXRES hRes, EXATOM atomPath, LPVOID* lpFile, size_t* dwFileLen)
{
    LPVOID pData = nullptr;

    if (HashTable_Get((EX_HASHTABLE*)hRes, atomPath, (size_t*)&pData)) {

        if (pData != 0) {
            *lpFile    = (LPVOID)((size_t)pData + 5);
            *dwFileLen = __get_int(pData, 1);
        }
    }
    return *dwFileLen > 0;
}

BOOL Ex_ResGetFile(HEXRES hRes, LPCWSTR lpwzPath, LPVOID* lpFile, size_t* dwFileLen)
{
    return Ex_ResGetFileFromAtom(hRes, Ex_Atom(lpwzPath), lpFile, dwFileLen);
}
