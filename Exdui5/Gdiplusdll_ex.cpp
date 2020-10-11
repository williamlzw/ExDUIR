#include "Gdiplusdll_ex.h"
EX_DECLEAR_API(GdiplusStartup);
EX_DECLEAR_API(GdipLoadImageFromStream);
EX_DECLEAR_API(GdipBitmapLockBits);
EX_DECLEAR_API(GdipBitmapUnlockBits);
EX_DECLEAR_API(GdipDisposeImage);

void º”‘ÿGdiplusDLL()
{
	HMODULE hModule = LoadLibraryW(L"gdiplus.dll");
	if (hModule)
	{
		EX_GET_API(GdiplusStartup);
		EX_GET_API(GdipLoadImageFromStream);
		EX_GET_API(GdipBitmapLockBits);
		EX_GET_API(GdipBitmapUnlockBits);
		EX_GET_API(GdipDisposeImage);
	}
}