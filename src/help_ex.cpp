#include "stdafx.h"

LOCALINFO  g_Li;
RENDERINFO g_Ri;

LPVOID GetProcAddr(LPCWSTR szMod, LPCSTR szApi)
{
    LPVOID  ret  = nullptr;
    HMODULE hLib = GetModuleHandleW(szMod);
    if (hLib == 0) {
        hLib = LoadLibraryW(szMod);
        if (hLib != 0) {
            ret = GetProcAddress(hLib, szApi);
            FreeLibrary(hLib);
        }
    }
    else {
        ret = GetProcAddress(hLib, szApi);
    }
    return ret;
}

void text_split(const wchar_t* pText, const wchar_t* pSplit, std::vector<std::wstring>& list)
{
    if (nullptr == pText || nullptr == pSplit) return;
    if (L'\0' == *pText || L'\0' == *pSplit) return;

    size_t         len    = wcslen(pSplit);
    const wchar_t* p      = pText;
    const wchar_t* pStart = p;
    while (*p) {
        p = wcsstr(p, pSplit);
        if (p) {
            if ((p - pStart) > 0) {
                list.push_back(std::wstring(pStart, p - pStart));
            }
            // No need to add an empty string if (p - pStart) == 0
            p += len;
            pStart = p;
        }
        else {
            if (*pStart) {
                list.push_back(std::wstring(pStart));
            }
            break;
        }
    }
}

INT DtoHimetric(INT d, INT PerInchc)
{
    return MulDiv(d, 2540, PerInchc);
}

LONG_PTR __get(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(LONG_PTR*)a;
}

INT __get_int(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(INT*)a;
}

FLOAT __get_float(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(FLOAT*)a;
}

SHORT __get_short(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(SHORT*)a;
}

CHAR __get_char(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(CHAR*)a;
}

UCHAR __get_unsignedchar(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(UCHAR*)a;
}

WCHAR __get_wchar(LPVOID lpAddr, LONG_PTR offset)
{
    size_t a = (size_t)lpAddr + offset;
    return *(WCHAR*)a;
}

LONG_PTR __set(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    size_t*  a   = (size_t*)((size_t)lpAddr + offset);
    LONG_PTR old = *a;
    *a           = value;
    return old;
}

INT __set_int(LPVOID lpAddr, LONG_PTR offset, INT value)
{
    size_t a   = (size_t)lpAddr + offset;
    INT    old = *(INT*)a;
    *(INT*)a   = value;
    return old;
}

void __set_char(LPVOID lpAddr, LONG_PTR offset, CHAR value)
{
    size_t a  = (size_t)lpAddr + offset;
    *(CHAR*)a = value;
}

SHORT __set_short(LPVOID lpAddr, LONG_PTR offset, SHORT value)
{
    size_t a   = (size_t)lpAddr + offset;
    SHORT  old = *(SHORT*)a;
    *(SHORT*)a = value;
    return old;
}

void __set_unsignedchar(LPVOID lpAddr, LONG_PTR offset, UCHAR value)
{
    size_t a   = (size_t)lpAddr + offset;
    *(UCHAR*)a = value;
}

void __set_wchar(LPVOID lpAddr, LONG_PTR offset, WCHAR value)
{
    size_t a   = (size_t)lpAddr + offset;
    *(WCHAR*)a = value;
}

FLOAT __set_float(LPVOID lpAddr, LONG_PTR offset, FLOAT value)
{
    size_t a   = (size_t)lpAddr + offset;
    FLOAT  old = *(FLOAT*)a;
    *(FLOAT*)a = value;
    return old;
}

BOOL __query(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    return (*(LONG_PTR*)((size_t)lpAddr + offset) & value) == value;
}

BOOL __query_int(LPVOID lpAddr, LONG_PTR offset, INT value)
{
    return (*(INT*)((size_t)lpAddr + offset) & value) == value;
}

void __del(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    size_t a      = (size_t)lpAddr + offset;
    *(LONG_PTR*)a = *(LONG_PTR*)a - (*(LONG_PTR*)a & value);
}

void __add(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    size_t a      = (size_t)lpAddr + offset;
    *(LONG_PTR*)a = *(LONG_PTR*)a | value;
}

void __addn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    size_t a      = (size_t)lpAddr + offset;
    *(LONG_PTR*)a = *(LONG_PTR*)a + value;
}

void __subn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value)
{
    size_t a      = (size_t)lpAddr + offset;
    *(LONG_PTR*)a = *(LONG_PTR*)a - value;
}

void _bit_add(size_t* dwValue, size_t index /*0-31 */)
{
    *dwValue |= (size_t)1 << index;
}

void _bit_del(size_t* dwValue, size_t index /*0-31 */)
{
    *dwValue &= ~((size_t)1 << index);
}

void _bit_not(size_t* dwValue, size_t index /*0-31 */)
{
    *dwValue ^= (size_t)1 << index;
}

BOOL _bit_test(size_t* dwValue, size_t index /*0-31 */)
{
    return *dwValue >> index & (size_t)1;
}

LPVOID __ptr_ins(LPVOID* ptr, INT nCount, INT* nIndexInsert, INT cbBlock, LPVOID pNewItem)
{
    if (cbBlock <= 0) cbBlock = sizeof(size_t);
    if (*nIndexInsert <= 0 || *nIndexInsert > nCount + 1) *nIndexInsert = nCount + 1;
    LPVOID pNew = Ex_MemAlloc((nCount + 1) * cbBlock);
    if (pNew) {
        if (*ptr) {
            RtlMoveMemory(pNew, *ptr, (*nIndexInsert - 1) * cbBlock);
            RtlMoveMemory((LPVOID)((size_t)pNew + *nIndexInsert * cbBlock),
                          (LPVOID)((size_t)*ptr + (*nIndexInsert - 1) * cbBlock),
                          (nCount - *nIndexInsert + 1) * cbBlock);
            Ex_MemFree(*ptr);
        }
        if (pNewItem != 0) {
            RtlMoveMemory((LPVOID)((size_t)pNew + (*nIndexInsert - 1) * cbBlock), pNewItem,
                          cbBlock);
        }
    }
    *ptr = pNew;
    return pNew;
}

LPVOID __ptr_index(LPVOID ptr, INT nCount, INT nIndex, INT cbBlock)
{
    if (nIndex <= 0 || nIndex > nCount) return 0;
    if (cbBlock <= 0) cbBlock = sizeof(size_t);
    return (LPVOID)((size_t)ptr + (nIndex - 1) * cbBlock);
}

LPVOID __ptr_del(LPVOID* ptr, INT nCount, INT nIndex, INT cbBlock)
{
    if (cbBlock <= 0) cbBlock = sizeof(size_t);
    if (nIndex <= 0 || nIndex > nCount) return *ptr;
    LPVOID pNew = Ex_MemAlloc(nCount * cbBlock);
    if (pNew) {
        if (*ptr) {
            RtlMoveMemory(pNew, *ptr, (nIndex - 1) * cbBlock);
            EX_REPORTLIST_COLUMNINFO* ptc =
                (EX_REPORTLIST_COLUMNINFO*)((size_t)*ptr +
                                            (nIndex - 1) * sizeof(EX_REPORTLIST_COLUMNINFO));
            RtlMoveMemory((LPVOID)((size_t)pNew + (nIndex - 1) * cbBlock),
                          (LPVOID)((size_t)*ptr + nIndex * cbBlock), (nCount - nIndex) * cbBlock);
            Ex_MemFree(*ptr);
        }
    }
    *ptr = pNew;
    return pNew;
}

void _wstr_deletechar(LPVOID lpstr, INT* dwsize, WCHAR wchar)
{
    LPVOID lpstart = lpstr;
    WCHAR* lpend   = (WCHAR*)((size_t)lpstart + *dwsize);
    BOOL   fMoved  = FALSE;
    while (lpstart < lpend) {
        lpstart = wcschr((WCHAR*)lpstart, wchar);
        if (lpstart == 0) {
            break;
        }
        else {
            lpend = (WCHAR*)((size_t)lpend - 2);
            RtlMoveMemory(lpstart, (WCHAR*)((size_t)lpstart + 2), (size_t)lpend - (size_t)lpstart);
            fMoved = TRUE;
        }
    }
    if (fMoved) {
        __set_wchar(lpend, 0, 0);
        *dwsize = (size_t)lpend - (size_t)lpstr;
    }
}

INT GetNearestPrime(INT value)
{
    INT prime = 0;

    // 如果输入的值小于41，则返回41
    if (value < 41) {
        return 41;
    }

    // 计算最小的质数，该质数大于或等于输入值
    int i = 0;
    while (value >= prime) {
        i++;

        // 通过以下公式计算最接近的质数 n^2 + n + 41
        prime = i * i + i + 41;
    }

    return prime;
}


void _struct_destroyfromaddr(LPVOID lpAddr, size_t Offset)
{
    LPVOID tmp = (LPVOID)__get(lpAddr, Offset);
    if (tmp) {
        Ex_MemFree(tmp);
        __set(lpAddr, Offset, 0);
    }
}

LPVOID _struct_createfromaddr(LPVOID lpAddr, size_t Offset, INT sizeofstruct, INT* nError)
{

    LPVOID tmp = (LPVOID)__get(lpAddr, Offset);
    if (tmp == 0) {
        tmp = Ex_MemAlloc(sizeofstruct);
        if (tmp == 0) {
            *nError = ERROR_EX_MEMORY_ALLOC;
        }
        else {
            __set(lpAddr, Offset, (size_t)tmp);
        }
    }
    return tmp;
}

void RC4(LPVOID dst, size_t dstlen, const LPVOID pwd, size_t pwdlen)
{
    size_t i, j, x;
    UCHAR  m[256], k[256], temp;

    if (pwdlen == 0) return;

    for (i = 0, j = 0; i < 256; ++i) {
        m[i] = (UCHAR)i;
        k[i] = ((UCHAR*)pwd)[j];
        if (++j == pwdlen) j = 0;
    }

    for (i = 0, j = 0; i < 256; ++i) {
        j    = (j + m[i] + k[i]) & 255;
        temp = m[i];
        m[i] = m[j];
        m[j] = temp;
    }
    for (x = 0, i = 0, j = 0; x < dstlen; ++x) {
        i    = (i + 1) & 255;
        j    = (j + m[i]) & 255;
        temp = m[i];
        m[i] = m[j];
        m[j] = temp;
        ((UCHAR*)dst)[x] ^= m[(m[i] + m[j]) & 255];
    }
}

const UINT32 table[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d};

UINT Crc32_Addr(LPVOID buf, UINT nLength)
{
    if (nLength < 1) {
        return 0xffffffff;
    }

    UINT CRC32 = 0xFFFFFFFF;   // 设置初始值

    for (DWORD i = 0; i < nLength; ++i) {
        CRC32 = table[(CRC32 ^ ((UCHAR*)(buf))[i]) & 0xff] ^ (CRC32 >> 8);
    }

    return CRC32 ^ 0xFFFFFFFF;
}

LPVOID prefixstring(LPCWSTR lpString, INT dwFmt, INT* nOffset)
{
    LPVOID ret = nullptr;
    if ((dwFmt & DT_NOPREFIX) == 0) {
        const wchar_t* lpOffset = wcschr(lpString, 38);
        if (lpOffset != 0) {
            INT len  = lstrlenW(lpString) * 2;
            *nOffset = (size_t)lpOffset - (size_t)lpString;
            ret      = Ex_MemAlloc(len);
            if (ret != 0) {
                RtlMoveMemory(ret, lpString, *nOffset);
                RtlMoveMemory((LPVOID)((size_t)ret + *nOffset),
                              (LPVOID)((size_t)lpString + *nOffset + 2), len - *nOffset - 2);
            }
        }
    }
    return ret;
}

std::wstring Ex_A2W(const std::string& str)
{
    if (str.empty()) {
        return L"";
    }
    int wlen = ::MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), NULL, 0);
    if (wlen <= 0) {
        return L"";
    }
    std::vector<wchar_t> result(wlen);
    ::MultiByteToWideChar(CP_ACP, 0, str.data(), (int)str.size(), result.data(), wlen);
    return std::wstring(result.data(), result.size());
}

std::string Ex_W2A(const std::wstring& wstr)
{
    if (wstr.empty()) {
        return "";
    }
    int len = ::WideCharToMultiByte(CP_ACP, 0, wstr.data(), (int)wstr.size(), NULL, 0, 0, 0);
    if (len <= 0) {
        return "";
    }
    std::vector<char> result(len);
    ::WideCharToMultiByte(CP_ACP, 0, wstr.data(), (int)wstr.size(), result.data(), len, 0, 0);
    return std::string(result.data(), result.size());
}

std::wstring Ex_A2W2(std::vector<UCHAR> str)
{
    if (str.empty()) {
        return L"";
    }
    int wlen = ::MultiByteToWideChar(CP_ACP, 0, (LPCCH)str.data(), (int)str.size(), NULL, 0);
    if (wlen <= 0) {
        return L"";
    }
    std::vector<wchar_t> result(wlen);
    ::MultiByteToWideChar(CP_ACP, 0, (LPCCH)str.data(), (int)str.size(), result.data(), wlen);
    return std::wstring(result.data(), result.size());
}

std::string Ex_A2U(const std::string& str)
{
    return Ex_W2U(Ex_A2W(str));
}

std::string Ex_U2A(const std::string& str)
{
    return Ex_W2A(Ex_U2W(str));
}

std::string Ex_U2A2(std::vector<CHAR> str)
{
    return Ex_W2A(Ex_U2W2(str));
}

std::string Ex_W2U(const std::wstring& wstr)
{
    if (wstr.empty()) {
        return "";
    }
    int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, 0, 0);
    if (len <= 0) {
        return "";
    }
    std::vector<char> result(len);
    ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), result.data(), len, 0, 0);
    return std::string(result.data(), result.size());
}

std::vector<UCHAR> Ex_W2U2(const std::wstring& wstr)
{
    std::vector<UCHAR> ret;
    if (wstr.empty()) {
        return ret;
    }
    int len = ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), NULL, 0, 0, 0);
    if (len <= 0) {
        return ret;
    }
    ret.resize(len);
    ::WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), (LPSTR)ret.data(), len, 0, 0);
    return ret;
}

std::wstring Ex_U2W(const std::string& str)
{
    if (str.empty()) {
        return L"";
    }
    int wlen = ::MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), NULL, 0);
    if (wlen <= 0) {
        return L"";
    }
    std::vector<wchar_t> result(wlen);
    ::MultiByteToWideChar(CP_UTF8, 0, str.data(), (int)str.size(), result.data(), wlen);
    return std::wstring(result.data(), result.size());
}

std::wstring Ex_U2W3(std::vector<UCHAR> str)
{
    if (str.empty()) {
        return L"";
    }
    int wlen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.data(), (int)str.size(), NULL, 0);
    if (wlen <= 0) {
        return L"";
    }
    std::vector<wchar_t> result(wlen);
    ::MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.data(), (int)str.size(), result.data(), wlen);
    return std::wstring(result.data(), result.size());
}

std::wstring Ex_U2W2(std::vector<CHAR> str)
{
    if (str.empty()) {
        return L"";
    }
    int wlen = ::MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.data(), (int)str.size(), NULL, 0);
    if (wlen <= 0) {
        return L"";
    }
    std::vector<wchar_t> result(wlen);
    ::MultiByteToWideChar(CP_UTF8, 0, (LPCCH)str.data(), (int)str.size(), result.data(), wlen);
    return std::wstring(result.data(), result.size());
}

std::wstring WStringFormat(const wchar_t* format, ...)
{
    std::wstring s;
    if (format == 0) {
        return s;
    }
    va_list argList;
    va_start(argList, format);
    size_t nLength = _vscwprintf(format, argList);
    s.resize(nLength);
    vswprintf_s(&s[0], nLength + 1, format, argList);
    va_end(argList);
    return s;
}

void CALLBACK pfnDefaultFreeData(LPVOID dwData)
{
    Ex_MemFree(dwData);
}

unsigned int LunarCalendarTable[199] = {
    0x04AE53, 0x0A5748, 0x5526BD, 0x0D2650, 0x0D9544,
    0x46AAB9, 0x056A4D, 0x09AD42, 0x24AEB6, 0x04AE4A, /*1901-1910*/
    0x6A4DBE, 0x0A4D52, 0x0D2546, 0x5D52BA, 0x0B544E,
    0x0D6A43, 0x296D37, 0x095B4B, 0x749BC1, 0x049754, /*1911-1920*/
    0x0A4B48, 0x5B25BC, 0x06A550, 0x06D445, 0x4ADAB8,
    0x02B64D, 0x095742, 0x2497B7, 0x04974A, 0x664B3E, /*1921-1930*/
    0x0D4A51, 0x0EA546, 0x56D4BA, 0x05AD4E, 0x02B644,
    0x393738, 0x092E4B, 0x7C96BF, 0x0C9553, 0x0D4A48, /*1931-1940*/
    0x6DA53B, 0x0B554F, 0x056A45, 0x4AADB9, 0x025D4D,
    0x092D42, 0x2C95B6, 0x0A954A, 0x7B4ABD, 0x06CA51, /*1941-1950*/
    0x0B5546, 0x555ABB, 0x04DA4E, 0x0A5B43, 0x352BB8,
    0x052B4C, 0x8A953F, 0x0E9552, 0x06AA48, 0x6AD53C, /*1951-1960*/
    0x0AB54F, 0x04B645, 0x4A5739, 0x0A574D, 0x052642,
    0x3E9335, 0x0D9549, 0x75AABE, 0x056A51, 0x096D46, /*1961-1970*/
    0x54AEBB, 0x04AD4F, 0x0A4D43, 0x4D26B7, 0x0D254B,
    0x8D52BF, 0x0B5452, 0x0B6A47, 0x696D3C, 0x095B50, /*1971-1980*/
    0x049B45, 0x4A4BB9, 0x0A4B4D, 0xAB25C2, 0x06A554,
    0x06D449, 0x6ADA3D, 0x0AB651, 0x093746, 0x5497BB, /*1981-1990*/
    0x04974F, 0x064B44, 0x36A537, 0x0EA54A, 0x86B2BF,
    0x05AC53, 0x0AB647, 0x5936BC, 0x092E50, 0x0C9645, /*1991-2000*/
    0x4D4AB8, 0x0D4A4C, 0x0DA541, 0x25AAB6, 0x056A49,
    0x7AADBD, 0x025D52, 0x092D47, 0x5C95BA, 0x0A954E, /*2001-2010*/
    0x0B4A43, 0x4B5537, 0x0AD54A, 0x955ABF, 0x04BA53,
    0x0A5B48, 0x652BBC, 0x052B50, 0x0A9345, 0x474AB9, /*2011-2020*/
    0x06AA4C, 0x0AD541, 0x24DAB6, 0x04B64A, 0x69573D,
    0x0A4E51, 0x0D2646, 0x5E933A, 0x0D534D, 0x05AA43, /*2021-2030*/
    0x36B537, 0x096D4B, 0xB4AEBF, 0x04AD53, 0x0A4D48,
    0x6D25BC, 0x0D254F, 0x0D5244, 0x5DAA38, 0x0B5A4C, /*2031-2040*/
    0x056D41, 0x24ADB6, 0x049B4A, 0x7A4BBE, 0x0A4B51,
    0x0AA546, 0x5B52BA, 0x06D24E, 0x0ADA42, 0x355B37, /*2041-2050*/
    0x09374B, 0x8497C1, 0x049753, 0x064B48, 0x66A53C,
    0x0EA54F, 0x06B244, 0x4AB638, 0x0AAE4C, 0x092E42, /*2051-2060*/
    0x3C9735, 0x0C9649, 0x7D4ABD, 0x0D4A51, 0x0DA545,
    0x55AABA, 0x056A4E, 0x0A6D43, 0x452EB7, 0x052D4B, /*2061-2070*/
    0x8A95BF, 0x0A9553, 0x0B4A47, 0x6B553B, 0x0AD54F,
    0x055A45, 0x4A5D38, 0x0A5B4C, 0x052B42, 0x3A93B6, /*2071-2080*/
    0x069349, 0x7729BD, 0x06AA51, 0x0AD546, 0x54DABA,
    0x04B64E, 0x0A5743, 0x452738, 0x0D264A, 0x8E933E, /*2081-2090*/
    0x0D5252, 0x0DAA47, 0x66B53B, 0x056D4F, 0x04AE45,
    0x4A4EB9, 0x0A4D4C, 0x0D1541, 0x2D92B5 /*2091-2099*/
};
int  MonthAdd[12]   = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
BYTE gLunarHolDay[] = {
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1901
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1902
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   // 1903
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   // 1904
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1905
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1906
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   // 1907
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1908
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1909
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1910
    0X96, 0XA5, 0X87, 0X96, 0X87, 0X87, 0X79, 0X69, 0X69, 0X69, 0X78, 0X78,   // 1911
    0X86, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1912
    0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1913
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1914
    0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   // 1915
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1916
    0X95, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   // 1917
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1918
    0X96, 0XA5, 0X97, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   // 1919
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1920
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   // 1921
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1922
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X69, 0X69, 0X78, 0X78,   // 1923
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1924
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X87,   // 1925
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1926
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1927
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1928
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1929
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1930
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X87, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1931
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1932
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1933
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1934
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1935
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1936
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1937
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1938
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1939
    0X96, 0XA5, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1940
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1941
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1942
    0X96, 0XA4, 0X96, 0X96, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1943
    0X96, 0XA5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1944
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1945
    0X95, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   // 1946
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1947
    0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 1948
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X79, 0X77, 0X87,   // 1949
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   // 1950
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X79, 0X79, 0X79, 0X69, 0X78, 0X78,   // 1951
    0X96, 0XA5, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 1952
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1953
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X68, 0X78, 0X87,   // 1954
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1955
    0X96, 0XA5, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 1956
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1957
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1958
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1959
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1960
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1961
    0X96, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1962
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1963
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1964
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1965
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1966
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1967
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1968
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1969
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1970
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X79, 0X69, 0X78, 0X77,   // 1971
    0X96, 0XA4, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1972
    0XA5, 0XB5, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1973
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1974
    0X96, 0XB4, 0X96, 0XA6, 0X97, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   // 1975
    0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X89, 0X88, 0X78, 0X87, 0X87,   // 1976
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 1977
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   // 1978
    0X96, 0XB4, 0X96, 0XA6, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   // 1979
    0X96, 0XA4, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1980
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X77, 0X87,   // 1981
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1982
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X78, 0X79, 0X78, 0X69, 0X78, 0X77,   // 1983
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1984
    0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 1985
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1986
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X79, 0X78, 0X69, 0X78, 0X87,   // 1987
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 1988
    0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1989
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 1990
    0X95, 0XB4, 0X96, 0XA5, 0X86, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1991
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 1992
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1993
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1994
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X76, 0X78, 0X69, 0X78, 0X87,   // 1995
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 1996
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 1997
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 1998
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 1999
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2000
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2001
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 2002
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 2003
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2004
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2005
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2006
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X69, 0X78, 0X87,   // 2007
    0X96, 0XB4, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   // 2008
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2009
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2010
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X78, 0X87,   // 2011
    0X96, 0XB4, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   // 2012
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2013
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2014
    0X95, 0XB4, 0X96, 0XA5, 0X96, 0X97, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 2015
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   // 2016
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2017
    0XA5, 0XB4, 0XA6, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2018
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 2019
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   // 2020
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2021
    0XA5, 0XB4, 0XA5, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2022
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X79, 0X77, 0X87,   // 2023
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   // 2024
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2025
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2026
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 2027
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   // 2028
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2029
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2030
    0XA5, 0XB4, 0X96, 0XA5, 0X96, 0X96, 0X88, 0X78, 0X78, 0X78, 0X87, 0X87,   // 2031
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   // 2032
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2033
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X78, 0X88, 0X78, 0X87, 0X87,   // 2034
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2035
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   // 2036
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X86,   // 2037
    0XA5, 0XB3, 0XA5, 0XA5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2038
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2039
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X96,   // 2040
    0XA5, 0XC3, 0XA5, 0XB5, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   // 2041
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X88, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2042
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2043
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X88, 0X87, 0X96,   // 2044
    0XA5, 0XC3, 0XA5, 0XB4, 0XA5, 0XA6, 0X87, 0X88, 0X87, 0X78, 0X87, 0X86,   // 2045
    0XA5, 0XB3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X88, 0X78, 0X87, 0X87,   // 2046
    0XA5, 0XB4, 0X96, 0XA5, 0XA6, 0X96, 0X88, 0X88, 0X78, 0X78, 0X87, 0X87,   // 2047
    0X95, 0XB4, 0XA5, 0XB4, 0XA5, 0XA5, 0X97, 0X87, 0X87, 0X88, 0X86, 0X96,   // 2048
    0XA4, 0XC3, 0XA5, 0XA5, 0XA5, 0XA6, 0X97, 0X87, 0X87, 0X78, 0X87, 0X86,   // 2049
    0XA5, 0XC3, 0XA5, 0XB5, 0XA6, 0XA6, 0X87, 0X88, 0X78, 0X78, 0X87, 0X87    // 2050
};


INT GetLunarCalendar(INT nYear, INT nMonth, INT nDay, INT* jr, INT* jq)
{
    int year                      = nYear;
    int month                     = nMonth;
    int day                       = nDay;
    *jr                           = 0;
    *jq                           = 0;
    unsigned int LunarCalendarDay = 0;
    int          Spring_NY, Sun_NY, StaticDayCount;
    int          index, flag;
    // Spring_NY 记录春节离当年元旦的天数。
    // Sun_NY 记录阳历日离当年元旦的天数。
    if (((LunarCalendarTable[year - 1901] & 0x0060) >> 5) == 1)
        Spring_NY = (LunarCalendarTable[year - 1901] & 0x001F) - 1;
    else
        Spring_NY = (LunarCalendarTable[year - 1901] & 0x001F) - 1 + 31;
    Sun_NY = MonthAdd[month - 1] + day - 1;
    if ((!(year % 4)) && (month > 2)) Sun_NY++;
    // StaticDayCount记录大小月的天数 29 或30
    // index 记录从哪个月开始来计算。
    // flag 是用来对闰月的特殊处理。
    // 判断阳历日在春节前还是春节后
    if (Sun_NY >= Spring_NY)   // 阳历日在春节后（含春节那天）
    {
        Sun_NY -= Spring_NY;
        month = 1;
        index = 1;
        flag  = 0;
        if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
            StaticDayCount = 29;
        else
            StaticDayCount = 30;
        while (Sun_NY >= StaticDayCount) {
            Sun_NY -= StaticDayCount;
            index++;
            if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20)) {
                flag = ~flag;
                if (flag == 0) month++;
            }
            else
                month++;
            if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
                StaticDayCount = 29;
            else
                StaticDayCount = 30;
        }
        day = Sun_NY + 1;
    }
    else   // 阳历日在春节前
    {
        Spring_NY -= Sun_NY;
        year--;
        month = 12;
        if (((LunarCalendarTable[year - 1901] & 0xF00000) >> 20) == 0)
            index = 12;
        else
            index = 13;
        flag = 0;
        if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
            StaticDayCount = 29;
        else
            StaticDayCount = 30;
        while (Spring_NY > StaticDayCount) {
            Spring_NY -= StaticDayCount;
            index--;
            if (flag == 0) month--;
            if (month == ((LunarCalendarTable[year - 1901] & 0xF00000) >> 20)) flag = ~flag;
            if ((LunarCalendarTable[year - 1901] & (0x80000 >> (index - 1))) == 0)
                StaticDayCount = 29;
            else
                StaticDayCount = 30;
        }
        day = StaticDayCount - Spring_NY + 1;
    }
    LunarCalendarDay |= day;
    LunarCalendarDay |= (month << 6);

    int jjr = 0;
    // 取阳历节假日
    if (nMonth == 1 && nDay == 1) {
        jjr = 1;   // 元旦
    }
    else if (nMonth == 2 && nDay == 14) {
        jjr = 2;   // 情人
    }
    else if (nMonth == 3 && nDay == 8) {
        jjr = 3;   // 妇女
    }
    else if (nMonth == 3 && nDay == 12) {
        jjr = 4;   // 植树
    }
    else if (nMonth == 4 && nDay == 1) {
        jjr = 5;   // 愚人
    }
    else if (nMonth == 5 && nDay == 1) {
        jjr = 6;   // 劳动
    }
    else if (nMonth == 5 && nDay == 4) {
        jjr = 7;   // 青年
    }
    else if (nMonth == 5 && nDay == 12) {
        jjr = 8;   // 护士
    }
    else if (nMonth == 6 && nDay == 1) {
        jjr = 9;   // 儿童
    }
    else if (nMonth == 7 && nDay == 1) {
        jjr = 10;   // 建党
    }
    else if (nMonth == 8 && nDay == 1) {
        jjr = 11;   // 建军
    }
    else if (nMonth == 9 && nDay == 10) {
        jjr = 12;   // 教师
    }
    else if (nMonth == 10 && nDay == 1) {
        jjr = 13;   // 国庆
    }
    else if (nMonth == 12 && nDay == 24) {
        jjr = 14;   // 平安
    }
    else if (nMonth == 12 && nDay == 25) {
        jjr = 15;   // 圣诞
    }
    // 阴历节假日
    if (jjr == 0) {
        if (month == 1 && day == 1) {
            jjr = 16;   // 春节
        }
        else if (month == 1 && day == 15) {
            jjr = 17;   // 元宵
        }
        else if (month == 4 && day == 444) {
            jjr = 18;   // 清明
        }
        else if (month == 5 && day == 5) {
            jjr = 19;   // 端午
        }
        else if (month == 7 && day == 7) {
            jjr = 20;   // 七夕
        }
        else if (month == 8 && day == 15) {
            jjr = 21;   // 中秋
        }
        else if (month == 9 && day == 9) {
            jjr = 22;   // 重阳
        }
        else if (month == 10 && day == 1) {
            jjr = 23;   // 祭祖
        }
        else if (month == 12 && day == 8) {
            jjr = 24;   // 腊八
        }
        else if (month == 12 && day == 23) {
            jjr = 25;   // 小年
        }
        else if (month == 12 && day == 30) {
            jjr = 26;   // 除夕
        }
    }
    *jr = jjr;

    // 取节气
    if (jjr == 0) {
        BYTE& flag2 = gLunarHolDay[(nYear - 1901) * 12 + nMonth - 1];
        int   day2;
        if (nDay < 15) {
            day2 = 15 - ((flag2 >> 4) & 0x0f);
        }
        else {
            day2 = ((flag2) & 0x0f) + 15;
        }
        if (nDay == day2) {
            *jq = (nMonth - 1) * 2 + (nDay > 15 ? 1 : 0) + 2;
        }
    }

    return LunarCalendarDay;
}

// 取当月天数
INT GetMdayCount(INT year, INT mon)
{
    // 取当月天数
    INT daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // 如果是 2 月份，判断是否是闰年
    if (mon == 2) {
        if ((year % 400) == 0 || (year % 4) == 0 && (year % 100) != 0) {
            return 29;
        }
    }

    return daysInMonth[mon - 1];
}

////取指定时间的星期
INT GetWeekOfDate(INT year, INT month, INT day)
{
    if (month == 1 || month == 2) {
        month += 12;
        --year;
    }
    return (day + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7 +
           1;
}


BOOL Flag_Query(INT dwFlag)
{
    return (g_Li.dwFlags & dwFlag) == dwFlag;
}

void Flag_Add(INT dwFlag)
{
    g_Li.dwFlags = g_Li.dwFlags | dwFlag;
}

void Flag_Del(INT dwFlag)
{
    g_Li.dwFlags = g_Li.dwFlags - (g_Li.dwFlags & dwFlag);
}

void IME_Control(HWND hWnd, wnd_s* pWnd, BOOL bEnable)
{
    ImmAssociateContext(hWnd, (bEnable ? pWnd->hImc_ : 0));
}

LPCWSTR GetErrorMessage(DWORD error)
{
    WCHAR szBuf[1024];
    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL, error, 0, (LPWSTR)&szBuf, 1024, NULL);
    wcscpy_s(szBuf, szBuf);
    return (LPCWSTR)szBuf;
}

int HexToInt(char c)
{
    if (c >= '0' && c <= '9') return c - '0';

    if (c >= 'a' && c <= 'f') return (c - 'a') + 10;

    if (c >= 'A' && c <= 'F') return (c - 'A') + 10;

    return 0;   // 如果不是有效的十六进制字符，就返回 0
}

std::wstring UrlDecode(const std::wstring& url, BOOL utf8)
{
    std::vector<UCHAR> data    = Ex_W2U2(url);
    auto               dataLen = data.size();
    DWORD              pos     = 0;
    DWORD              retLen  = 0;
    std::vector<UCHAR> retData;
    retData.resize(dataLen);

    while (pos < dataLen)                           // 遍历字符串中每一个字符
    {                                               // 一行代码就可以实现所有逻辑：
        if (data[pos] == 37 && pos + 2 < dataLen)   // 如果遇到'%'字符，并且当前位置在字符串末尾之前
        {                                           // 就将该字符解码成一个字节：
            retData[retLen++] = (UCHAR)(HexToInt(data[pos + 1]) * 16 + HexToInt(data[pos + 2]));
            pos += 3;   // 向前跳过三个字节
        }
        else {   // 否则直接将当前字节复制到目标数组中：
            retData[retLen++] = data[pos++];
        }
    }

    retData.resize(retLen);   // 确保目标数组的大小正好是里面有效内容的大小

    if (utf8) {
        return Ex_U2W3(retData);
    }
    return Ex_A2W2(retData);
}


const int HEXINT[16] = {48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 65, 66, 67, 68, 69, 70};

std::string UrlEncode(std::wstring url, BOOL notEncodeAlphanumeric, BOOL utf8, INT mode)
{
    // encodeURIComponent('!@#$ % ^&*()_ + -= [] {}; :\'\"<>,./?\\? ') = !% 40 % 23 % 24 % 25 % 5E %
    // 26 * ()_ % 2B - % 3D % 5B % 5D % 7B % 7D % 3B % 3A'%22%3C%3E%2C.%2F%3F%5C%3F%20
    // encodeURI('!@#$ % ^&*()_ + -= [] {}; :\'\"<>,./?\\? ') = !@#$ % 25 % 5E & *()_ + -= % 5B % 5D
    // % 7B % 7D; :'%22%3C%3E,./?%5C?%20 escape('!@#$ % ^&*()_ + -= [] {}; :\'\"<>,./?\\? ') = % 21@
    // % 23 % 24 % 25 % 5E % 26 * %28 % 29_ + -% 3D % 5B % 5D % 7B % 7D % 3B % 3A % 27 % 22 % 3C %
    // 3E % 2C. / % 3F % 5C % 3F % 20
    //  set default mode if mode is invalid
    if (mode < 0 || mode > 2) mode = 0;

    std::wstring text(url);
    std::string  utext;
    utext               = Ex_W2U(text);
    size_t      len     = utext.length();
    std::string decoded = "";

    // iterate through characters in string
    for (size_t i = 0; i < len; i++) {
        // check if character should be encoded
        bool shouldEncode = false;
        if (notEncodeAlphanumeric) {
            unsigned char wch   = utext.at(i);
            bool isAlphanumeric = (('A' <= wch && wch <= 'Z') || ('a' <= wch && wch <= 'z') ||
                                   ('0' <= wch && wch <= '9'));

            shouldEncode =
                !isAlphanumeric && (wch != '*' && wch != '-' && wch != '.' && wch != '_');

            if (mode < 2)
                shouldEncode =
                    shouldEncode && (wch != '!' && wch != '\'' && wch != '(' && wch != ')');
            if (mode > 0) shouldEncode = shouldEncode && (wch != '+' && wch != '/' && wch != '@');
            if (mode == 1)
                shouldEncode =
                    shouldEncode && (wch != '#' && wch != '$' && wch != '&' && wch != ',' &&
                                     wch != ':' && wch != ';' && wch != '=' && wch != '?');
        }

        // append encoded/not encoded character to output
        if (shouldEncode) {
            decoded += '%';
            decoded += (unsigned char)HEXINT[((unsigned char)utext.at(i) >> 4)];
            decoded += (unsigned char)HEXINT[((unsigned char)utext.at(i) & 15)];
        }
        else {
            decoded += (unsigned char)utext.at(i);
        }
    }

    return decoded;
}

std::wstring UrlEncodeW(std::wstring url, BOOL notEncodeAlphanumeric, BOOL utf8, INT mode)
{
    return Ex_U2W(UrlEncode(url, notEncodeAlphanumeric, utf8, mode));
}

HRESULT IsEffectRegistered(ID2D1Factory1* d2dFactory, const CLSID& effectID, bool& result)
{
    UINT32  n;
    HRESULT hr = d2dFactory->GetRegisteredEffects(nullptr, 0, nullptr, &n);
    if (FAILED(hr)) {
        return hr;
    }

    std::vector<CLSID> effects(n);
    hr = d2dFactory->GetRegisteredEffects(effects.data(), n, &n, nullptr);
    if (FAILED(hr)) {
        return hr;
    }

    auto it = std::find(effects.begin(), effects.end(), effectID);
    result  = it != effects.end();

    return S_OK;
}

void printVector(const std::vector<BYTE>& vec)
{
    // 使用 wstringstream 来构建宽字符字符串
    std::wostringstream wss;
    wss << L"{";

    if (!vec.empty()) {
        // 输出第一个元素
        wss << static_cast<int>(vec[0]);

        // 输出剩余的元素
        for (size_t i = 1; i < vec.size(); ++i) {
            wss << L"," << static_cast<int>(vec[i]);
        }
    }

    wss << L"}";

    // 将 wstringstream 的内容转换为宽字符 C 字符串
    std::wstring ws = wss.str();
    OutputDebugStringW(ws.c_str());
}

bool IsDotSubDirName(const TCHAR* szFileName)
{
    if (szFileName == NULL) return FALSE;

    return ((szFileName[0] == '.' && szFileName[1] == '\0') ||
            (szFileName[0] == '.' && szFileName[1] == '.' && szFileName[2] == '\0'));
}

std::wstring FindFile(HANDLE& hFileFind, std::wstring prefindfile, INT prefindfileattribute,
                      INT preskipfileattribute)
{
    bool            blpIsFindNext = prefindfile.empty();
    WIN32_FIND_DATA infFindData;
    bool            blpSucceeded = false;
    while (TRUE) {
        if (blpIsFindNext)
            blpSucceeded = (hFileFind == NULL ? FALSE : ::FindNextFile(hFileFind, &infFindData));
        else {
            if (hFileFind != NULL) ::FindClose(hFileFind);
            hFileFind    = ::FindFirstFile(prefindfile.data(), &infFindData);
            blpSucceeded = (hFileFind != INVALID_HANDLE_VALUE);
            if (blpSucceeded == FALSE) hFileFind = NULL;
            blpIsFindNext = TRUE;
        }
        if (blpSucceeded == FALSE) {
            if (hFileFind != NULL) {
                ::FindClose(hFileFind);
                hFileFind = NULL;
            }
            break;
        }
        if (((infFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&
             IsDotSubDirName(infFindData.cFileName)) ||                   // 为"."或".."目录?
            (infFindData.dwFileAttributes & preskipfileattribute) != 0)   // 需要被跳过?
            continue;
        if ((prefindfileattribute == 0 && infFindData.dwFileAttributes == 0) ||
            (prefindfileattribute == -1
                 ? (infFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0
                 : (infFindData.dwFileAttributes & prefindfileattribute) != 0)) {
            break;
        }
    }
    return std::wstring(blpSucceeded ? infFindData.cFileName : _T(""));
}


// 辅助函数：将数字0-15转换为对应的十六进制字符
wchar_t LTOUCHAR(BYTE value)
{
    if (value < 10)
        return L'0' + value;
    else
        return L'A' + (value - 10);
}

std::wstring Md5Encrypt(const std::vector<BYTE>& data)
{
    // MD5 哈希长度为 16 字节
    const DWORD MD5_HASH_LENGTH = 16;
    // 十六进制表示每个字节需要 2 个字符
    const size_t HEX_LENGTH = MD5_HASH_LENGTH * 2;

    std::wstring hashString;
    hashString.resize(HEX_LENGTH, L'0');

    // 打开 MD5 算法提供者
    BCRYPT_ALG_HANDLE hAlgorithm = NULL;
    NTSTATUS          status = BCryptOpenAlgorithmProvider(&hAlgorithm, BCRYPT_MD5_ALGORITHM, NULL,
                                                           BCRYPT_HASH_REUSABLE_FLAG);
    if (status < 0) {
        // 处理错误
        return L"";
    }

    // 创建哈希对象
    BCRYPT_HASH_HANDLE hHash = NULL;
    status                   = BCryptCreateHash(hAlgorithm, &hHash, NULL, 0, NULL, 0, 0);
    if (status < 0) {
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return L"";
    }

    // 更新哈希数据
    status = BCryptHashData(hHash, (PUCHAR)data.data(), data.size(), 0);
    if (status < 0) {
        BCryptDestroyHash(hHash);
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return L"";
    }

    // 完成哈希计算
    BYTE hash[MD5_HASH_LENGTH];
    status = BCryptFinishHash(hHash, hash, MD5_HASH_LENGTH, 0);
    if (status < 0) {
        BCryptDestroyHash(hHash);
        BCryptCloseAlgorithmProvider(hAlgorithm, 0);
        return L"";
    }

    for (DWORD i = 0; i < MD5_HASH_LENGTH; ++i) {
        hashString[i * 2]     = LTOUCHAR((hash[i] >> 4) & 0xF);
        hashString[i * 2 + 1] = LTOUCHAR(hash[i] & 0xF);
    }
    // 释放资源
    BCryptDestroyHash(hHash);
    BCryptCloseAlgorithmProvider(hAlgorithm, 0);

    return hashString;
}
