#pragma once

std::vector<BYTE> _bin_compress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen,
                                INT* nError);
void   _bin_uncompress(LPVOID lpData, size_t dwSize, LPVOID lpKey, size_t dwLen, LPVOID* retPtr,
                       size_t* retLen);
HEXRES _res_unpack(LPVOID lpData, size_t dwDataLen, UCHAR byteHeader);
void _res_pack_processing(const std::wstring path, const std::wstring root, bool bPntBits,
                           std::vector<BYTE>& buffser, int& nCount);
bool directoryExists(const std::wstring& dirPath);
bool createDirectoryRecursive(const std::wstring& dirPath);
