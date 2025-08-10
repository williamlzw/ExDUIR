#pragma once
#if defined(_WIN32) || defined(_WIN64)
#ifndef WINAPI
#define WINAPI __stdcall
#endif
#else
#define WINAPI 
#endif

/*********************************************************************************
        功  能: 对数据块进行Base64编码
        参        数: pInput      - [in]编码前数据块
                        nLength     - [in]输入数据块（pInput）长度
                        pOutput     - [out]Base64编码后数据块，大小为输入数据的4/3倍，
                                                   输出数据块pInput 和输入数据块pOutput 起始地址可以相同
                        nOutBufSize - [in]存放编码后数据（pOutput）的缓冲区大小
        返  回:          0   - 用于存放编码后数据的缓冲区不够，编码失败。
                        大于0 - 编码后数据长度，值为(inputLen+2)/3*4
*********************************************************************************/
int WINAPI Base64Encode(unsigned char* pInput, int nLength, char* pOutput, int nOutBufSize);

/*********************************************************************************
        功  能：对输入的Base64编码数据块进行Base64解码
        参        数：pInput  - [in]Base64编码数据块
                        nLength - [in]Base64编码数据块长度
                        pOutput - [out]Base64解码后的数据块
                                                输出数据块pInput和输入数据块pOutput起始地址可以相同
        返  回：   0  - 无效数据，解码失败
                        大于0 - Base64解码后数据长度
*********************************************************************************/
int WINAPI Base64Decode(const char* pInput, int nLength, unsigned char* pOutput);

/*********************************************************************************
        功  能: 对数据块进行Base64URL编码
        参        数: pInput      - [in]编码前数据块
                        nLength     - [in]输入数据块（pInput）长度
                        pOutput     - [out]Base64URL编码后数据块，大小为输入数据的4/3倍，
                                                           输出数据块pInput 和输入数据块pOutput 起始地址可以相同
                        nOutBufSize - [in]存放编码后数据（pOutput）的缓冲区大小
        返  回:          0   - 用于存放编码后数据的缓冲区不够，编码失败。
                        大于0 - 编码后数据长度，值为(inputLen+2)/3*4
*********************************************************************************/
int WINAPI Base64URLEncode(unsigned char* pInput, int nLength, char* pOutput, int nOutBufSize);

/*********************************************************************************
        功  能：对输入的Base64URL编码数据块进行base64URL解码
        参        数：pInput  - [in]Base64URL编码数据块
                        nLength - [in]Base64URL编码数据块长度
                        pOutput - [out]Base64URL解码后的数据块
                                                输出数据块pInput和输入数据块pOutput起始地址可以相同
        返  回：   0  - 无效数据，解码失败
                        大于0 - Base64URL解码后数据长度
*********************************************************************************/
int WINAPI Base64URLDecode(const char* pInput, int nLength, unsigned char* pOutput);

