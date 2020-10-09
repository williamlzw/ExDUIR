#include <iostream>
#include <vector>
#include <algorithm>
#include <Windows.h>

#pragma comment ( lib,"Bcrypt.lib" ) 

#include "help_ex.h"
#include "Global_ex.h"
#include "Array_ex.h"




void 测试哈希表()
{
	auto aptr = LocalAlloc(64, sizeof(void*));
	auto table = HashTable_Create(17, &pfnDefaultFreeData);
	auto aptr2 = LocalAlloc(64, sizeof(void*));
	HashTable_Set(table, 1, (size_t)aptr);
	HashTable_Set(table, 8, (size_t)aptr2);
	size_t ret = 0;
	std::cout << (size_t)aptr2 << std::endl;
	HashTable_Get(table, 8, &ret);
	std::cout << ret << std::endl;
	size_t* arry_key = (size_t*)LocalAlloc(64, sizeof(void*));
	size_t* arry_value = (size_t*)LocalAlloc(64, sizeof(void*));
	HashTable_GetAllKeysAndValues(table, &arry_key, &arry_value);
	std::cout << arry_key[1] << std::endl;
	std::cout << arry_value[1] << std::endl;
	//std::cout << "11"<< std::endl;

	HashTable_Destroy(table);

	LocalFree(arry_key);
	LocalFree(arry_value);
}


void 数组遍历()
{
	std::vector<int> aryColorsAtom = { ATOM_BACKGROUND_COLOR, ATOM_COLOR_BACKGROUND, ATOM_BORDER_COLOR, ATOM_COLOR_BORDER, ATOM_COLOR, ATOM_COLOR_NORMAL, ATOM_COLOR_HOVER, ATOM_COLOR_DOWN, ATOM_COLOR_FOCUS, ATOM_COLOR_CHECKED, ATOM_COLOR_SELECTED, ATOM_COLOR_HOT, ATOM_COLOR_VISTED, ATOM_COLOR_SHADOW };
	
	std::for_each(aryColorsAtom.cbegin(), aryColorsAtom.cend(), [](const int& val)->void {std::cout << val << std::endl; });
	for (auto iter = aryColorsAtom.cbegin(); iter != aryColorsAtom.cend(); iter++)
	{
		//std::cout << (*iter) << std::endl;
	}
	auto result = std::find(aryColorsAtom.begin(), aryColorsAtom.end(), ATOM_COLOR_BORDER); //查找3

	if (result != aryColorsAtom.end()) //找到
	{
		auto index = std::distance(aryColorsAtom.begin(), result);
		std::cout << "Yes" << (int)index << std::endl;

	}
	else //没找到
	{
		std::cout << "No" << std::endl;
	}


	std::cout << aryColorsAtom.size() << std::endl;
}

void 测试子程序()
{
	//std::wstring a = L"RGBA";
	//std::cout << Ex_Atom((LPWSTR)a.c_str()) << std::endl;
   // BYTE aa[5] = { 1,2,3,4,5 };
	//std::cout << 数据_Crc32_Addr((UCHAR*)aa,5) << std::endl;
	//float a = round(1.6 * 2);
	//std::cout <<a<< std::endl;

	DWORD dwFlags = 1;
	__add(&dwFlags, 0, 4);
	__del(&dwFlags, 0, 4);
	std::cout<< __query(&dwFlags, 0, 4) << std::endl;
	
}

bool 枚举数组(void* pArray,int nIndex,void* pvItem,int nType,size_t pvParam)
{
	std::cout << "句柄:"<<pArray << std::endl;
	std::cout << "索引:" << nIndex << std::endl;
	std::cout << "项目句柄:" << pvItem << std::endl;
	std::cout << "类型:" << nType << std::endl;
	std::cout << "回调参数:" << pvParam << std::endl;
	return (size_t)pvItem ==pvParam;
}

void 测试数组()
{
	auto aa = Array_Create(5);
	Array_AddMember(aa, 6);
	Array_SetMember(aa, 2, 3);
	std::cout << Array_GetMember(aa, 2) << std::endl;//3
	Array_DelMember(aa, 1);
	std::cout << Array_GetMember(aa, 1) << std::endl;//3删除成员后索引往前进1,原来索引是2
	Array_SetMember(aa, 1, 4);
	std::cout << Array_GetMember(aa, 1) << std::endl;//4修改成员
	std::cout << Array_GetCount(aa) << std::endl;//5
	Array_Redefine(aa, 6, false);
	std::cout << Array_GetCount(aa) << std::endl;//6
	Array_SetExtra(aa, 66);
	std::cout << Array_GetExtra(aa) << std::endl;//66
	Array_Emum(aa, &枚举数组,4);
	Array_Destroy(aa);
}

void 测试句柄池()
{
	g_Li.hHandles = _handle_init();
	auto bb = 申请内存(sizeof(img_s));
	int nError=0;
	std::cout << "bb:" << bb << std::endl;
	auto cc = _handle_create(HT_IMAGE, bb, &nError);
	std::cout<<"cc:" << cc << std::endl;
	std::cout << "nError:" << nError << std::endl;
	void* dd;

	std::cout << _handle_validate(cc, HT_IMAGE, &dd, &nError) << std::endl;
	std::cout << "nError:"<< nError << std::endl;
	std::cout << "dd:"<<dd << std::endl;
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	_handle_destroy(cc, &nError);
	_handle_uninit(g_Li.hHandles);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
	释放内存(bb);
	std::cout << IsBadReadPtr(bb, sizeof(img_s)) << std::endl;
}

void 测试引用(int *a)
{

	*a = 5;
}

//char -> 16
void char_2_Hex(char* Char, char* Hex)
{
	int length = strlen(Char);
	for (size_t i = 0; i < length; i++)
	{
		int tmp = int(Char[i]);
		if (Char[i] < 0)
			tmp = (-1) * Char[i] + 128;
		int high = tmp / 16;
		int low = tmp % 16;
		char HIHG;
		char LOW;

		if (high >= 10)
			HIHG = char(high - 10 + 65);
		else
			HIHG = char(high + 48);

		if (low >= 10)
			LOW = char(low - 10 + 65);
		else
			LOW = char(low + 48);

		Hex[2 * i] = HIHG;
		Hex[2 * i + 1] = LOW;
	}
}

//16 -> char 
void Hex_2_char(char* Hex, char* Char)
{
	int length = strlen(Hex) / 2;
	for (size_t i = 0; i < length; i++)
	{
		int high;
		int low;
		if (int(Hex[2 * i]) >= 65)
			high = int(Hex[2 * i] - 65 + 10);
		else
			high = int(Hex[2 * i] - 48);

		if (int(Hex[2 * i + 1]) >= 65)
			low = int(Hex[2 * i + 1] - 65 + 10);
		else
			low = int(Hex[2 * i + 1] - 48);

		Char[i] = char(high * 16 + low);
	}
}

char* RC4(char* C, char* key)
{
	int S[256];
	int T[256];

	int  count = 0;
	count = strlen(key);

	for (int i = 0; i < 256; i++)
	{
		S[i] = i;
		int tmp = i % count;
		T[i] = key[tmp];
	}

	int j = 0;
	int i;
	for ( i = 0; i < 256; i++)
	{
		j = (j + S[i] + T[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;
	}

	int length = 0;
	length = strlen(C);

	i = 0, j = 0;

	for (int p = 0; p < length; p++)
	{

		i = (i + 1) % 256;
		j = (j + S[i]) % 256;
		int tmp;
		tmp = S[j];
		S[j] = S[i];
		S[i] = tmp;

		int k = S[(S[i] + S[j]) % 256];
		C[p] = C[p] ^ k;
	}

	return C;
}

const std::string toHexString(const unsigned char* input, const int datasize)

{
	std::string output;
	char ch[3];

	for (int i = 0; i < datasize; ++i)
	{
		sprintf_s(ch, 3, "%02x", input[i]);
		output += ch;
	}
	return output;

}

const std::string ToHexString2(const unsigned char* input, const int datasize)
{
	char  output[33];
	for (int j = 0; j < datasize; j++)
	{
		unsigned char b = *(input + j);
		sprintf_s(output + j * 2, 3, "%02x", b);
	}
	return std::string(output);
}

int main()
{
	
	//测试句柄池();
	//数组遍历();
	//测试哈希表();
	//测试子程序();
	//测试数组();
	//int b;
	//测试引用(&b);
	//bool a = true;
	BCRYPT_ALG_HANDLE hAlgHandle;
	BCRYPT_KEY_HANDLE hKey;
	////char A[1000] = { 97,98,99,100,101,102 };
	////char key[256] = { 97,98,99,100,101,102 };
	char A[1000] = "abcdef";
	char key[256] = "abcdef";
	
	BCryptOpenAlgorithmProvider(&hAlgHandle, L"RC4", NULL, 0);
	
	BCryptGenerateSymmetricKey(hAlgHandle, &hKey, NULL, NULL,(PUCHAR)&key[0],strlen(key),0);
	
	ULONG cbCipherText;
	 BCryptEncrypt(hKey, (PUCHAR)&A[0],strlen(A),NULL,0,0,NULL,0,&cbCipherText,0);
	
	//char* B = new char[cbCipherText];
	void* BB = malloc(cbCipherText);
	//std::cout << cbCipherText<<","<< strlen(B) << std::endl;
	ULONG cbData;
	auto ret = BCryptEncrypt(hKey, (PUCHAR)&A[0], strlen(A), NULL, 0, 0, (PUCHAR)BB, cbCipherText,&cbData, 0);
	std::cout << ret << "," << cbCipherText<< cbData << std::endl;
	BCryptDestroyKey(hKey);
	BCryptCloseAlgorithmProvider(hAlgHandle, 0);
	//std::cout << "22" << std::endl;
	//std::cout<< !a<<std::endl;
	/*char A[1000] = "abcdef";
	for (int index = 0; index < strlen(A); index++)
	{
		std::cout << "A= " << toascii(A[index]) << std::endl;
	}
	std::cout << "C =" << A << std::endl;
	char key[256] = "abcdef";
	std::cout << "key = " << key << std::endl;
	size_t  count = 0;
	count = strlen(A);

	std::cout <<count<< std::endl;
	char* B = RC4(A, key);

	char* M = new char[ 2*count];
	for (int index = 0; index < strlen(B); index++)
	{
		std::cout << "B= " << toascii( B[index]) << std::endl;
	}
	*/
	char M[1024]={0};
	memset(M, 0, cbCipherText);
	//std::cout << strlen(M) << std::endl;
	//ToHexString2((unsigned char*)BB,6);
	
	std::cout << "M = " << ToHexString2((unsigned char*)BB, 6) << std::endl;
	free(BB);
}

