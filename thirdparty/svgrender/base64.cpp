#include "stdafx.h"
#include "base64.h"

/* 0－63的数转化为可见字符查表 */
char g_b64EncTable[65] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char g_b64DecTable[256] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0x3e, 0xff, 0xff, 0xff, 0x3f,

        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b,
        0x3c, 0x3d, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,

        0xff, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
        0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
        0x17, 0x18, 0x19, 0xff, 0xff, 0xff, 0xff, 0xff,

        0xff, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
        0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
        0x31, 0x32, 0x33, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

int WINAPI Base64Encode(unsigned char* pInput, int nLength, char* pOutput, int nOutBufSize)
{
    int nModulus = nLength % 3;
    int nOutLen = (nLength + 2) / 3 * 4; /* 编码后数据是4的整数倍 */
    int nDatalen = nLength - nModulus;
    int i, j;
    int x, y, z;

    /* 缓冲区大小校验 */
    if (nOutBufSize < nOutLen)
    {
        return 0;
    }

    for (i = 0, j = 0; i < nDatalen; i += 3, j += 4)
    {
        x = pInput[i];
        y = pInput[i + 1];
        z = pInput[i + 2];
        pOutput[j] = g_b64EncTable[x >> 2];
        pOutput[j + 1] = g_b64EncTable[((x & 3) << 4) | (y >> 4)];
        pOutput[j + 2] = g_b64EncTable[((y & 15) << 2) | (z >> 6)];
        pOutput[j + 3] = g_b64EncTable[z & 63];
    }

    i = nDatalen;
    j = (nDatalen / 3) * 4;
    if (nModulus == 1)
    {
        x = pInput[i];
        pOutput[j] = g_b64EncTable[x >> 2];
        pOutput[j + 1] = g_b64EncTable[(x & 3) << 4];
        pOutput[j + 2] = '=';
        pOutput[j + 3] = '=';
    }
    else if (nModulus == 2)
    {
        x = pInput[i];
        y = pInput[i + 1];
        pOutput[j] = g_b64EncTable[x >> 2];
        pOutput[j + 1] = g_b64EncTable[((x & 3) << 4) | (y >> 4)];
        pOutput[j + 2] = g_b64EncTable[(y & 15) << 2];
        pOutput[j + 3] = '=';
    }
    return nOutLen;
}

int WINAPI Base64Decode(const char* pInput, int nLength, unsigned char* pOutput)
{
    int i, j;
    int x, y, z, k;
    int nOutLen;
    int nPadBytes;
    int nDataLen;

    if (nLength % 4 != 0)
    {
        return 0;
    }

    /* 根据'='的个数判断编码前数据的长度 */
    if (pInput[nLength - 2] == '=')
    {
        nPadBytes = 2;
    }
    else if (pInput[nLength - 1] == '=')
    {
        nPadBytes = 1;
    }
    else
    {
        nPadBytes = 0;
    }

    nOutLen = nLength / 4 * 3 - nPadBytes;
    nDataLen = (nLength - nPadBytes) / 4 * 3;

    /* 开始解码 */
    for (i = 0, j = 0; i < nDataLen; i += 3, j += 4)
    {
        x = g_b64DecTable[(int)pInput[j]];
        y = g_b64DecTable[(int)pInput[j + 1]];
        z = g_b64DecTable[(int)pInput[j + 2]];
        k = g_b64DecTable[(int)pInput[j + 3]];
        if (x == 0xff || y == 0xff || z == 0xff || k == 0xff)
        {
            return 0;
        }
        pOutput[i] = (x << 2) | (y >> 4);
        pOutput[i + 1] = ((y & 15) << 4) | (z >> 2);
        pOutput[i + 2] = ((z & 3) << 6) | k;
    }

    i = nDataLen;
    if (nPadBytes == 1)
    {
        x = g_b64DecTable[(int)pInput[j]];
        y = g_b64DecTable[(int)pInput[j + 1]];
        z = g_b64DecTable[(int)pInput[j + 2]];

        pOutput[i] = (x << 2) | (y >> 4);
        pOutput[i + 1] = ((y & 15) << 4) | (z >> 2);
    }
    else if (nPadBytes == 2)
    {
        x = g_b64DecTable[(int)pInput[j]];
        y = g_b64DecTable[(int)pInput[j + 1]];

        pOutput[i] = (x << 2) | (y >> 4);
    }
    return nOutLen;
}

/*
Base64是网络上最常见的用于传输8Bit字节码的编码方式之一，Base64就是一种基于64个可打印字符来表示二进制数据的方法
BASE64URL编码的流程：
        1、明文使用BASE64进行加密
        2、在BASE64的基础上进行一下的编码：
                2.1)去除尾部的"="
                2.2)把"+"替换成"-"
                2.3)把"/"替换成"_"
BASE64URL解码的流程：
        1、把BASE64URL的编码做如下解码：
                1)把"-"替换成"+"
                2)把"_"替换成"/"
                3)(计算BASE64URL编码长度)%4
                        3.1)结果为0，不做处理
                        3.2)结果为2，字符串添加"=="
                        3.3)结果为3，字符串添加"="
        2、使用BASE64解码密文，得到原始的明文
*/
int WINAPI Base64URLEncode(unsigned char* pInput, int nLength, char* pOutput, int nOutBufSize)
{
    //1.明文使用BASE64进行加密 
    int nOutLen = Base64Encode(pInput, nLength, pOutput, nOutBufSize);
    if (nOutLen <= 0)
    {
        return nOutLen;
    }
    //2.在BASE64的基础上进行一下的编码
    //2.1 去除尾部的"=" 
    if ('=' == pOutput[nOutLen - 2])
    {
        pOutput[nOutLen - 2] = '\0';
        nOutLen = nOutLen - 2;
    }
    else if ('=' == pOutput[nOutLen - 1])
    {
        pOutput[nOutLen - 1] = '\0';
        nOutLen = nOutLen - 1;
    }
    //        2.2)把"+"替换成"-" 
    //  2.3)把"/"替换成"_"
    for (int i = 0; i < nOutLen; i++)
    {
        if ('+' == pOutput[i])
            pOutput[i] = '-';
        else if ('/' == pOutput[i])
            pOutput[i] = '_';
    }
    return nOutLen;
}

int WINAPI Base64URLDecode(const char* pInput, int nLength, unsigned char* pOutput)
{
    int iTmpLen = nLength;
    char* pTmpBuffer = (char*)Ex_MemAlloc((nLength + 10) * sizeof(char));
    memcpy(pTmpBuffer, pInput, nLength);
    //1、把BASE64URL的编码做如下解码
    // 1)把"-"替换成"+". 
    // 2)把"_"替换成"/" .
    for (int i = 0; i < nLength; i++)
    {
        if ('-' == pTmpBuffer[i])
            pTmpBuffer[i] = '+';
        else if ('_' == pTmpBuffer[i])
            pTmpBuffer[i] = '/';
    }
    //3)(计算BASE64URL编码长度)%4
    //   3.1)结果为0，不做处理
    //   3.2)结果为2，字符串添加"=="
    //   3.3)结果为3，字符串添加"="
    int nModulus = nLength % 4;
    if (2 == nModulus)
    {
        pTmpBuffer[nLength] = '=';
        pTmpBuffer[nLength + 1] = '=';
        iTmpLen = nLength + 2;
    }
    else if (3 == nModulus)
    {
        pTmpBuffer[nLength] = '=';
        iTmpLen = nLength + 1;
    }
    // 使用BASE64解码密文，得到原始的明文
    int nOutLen = Base64Decode(pTmpBuffer, iTmpLen, pOutput);

    Ex_MemFree(pTmpBuffer);
    return nOutLen;
}

