#include "stdafx.h"

void _bin_uncompress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen, LPVOID* retPtr,
                     size_t* retLen)
{
    INT    i      = 1263556677;
    INT    nError = 0;
    LPVOID hImg   = nullptr;
    if (__get_int(lpData, 0) == i) {
        LPVOID pData = Ex_MemAlloc(dwSize);
        if (pData == 0) {
            nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            RtlMoveMemory(pData, lpData, dwSize);
            if (lpKey == 0) {
                lpKey = &i;
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
                        // UCHAR prop = __get_unsignedchar(retPtr, 4);
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