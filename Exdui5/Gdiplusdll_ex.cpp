#include "Gdiplusdll_ex.h"
EX_DECLEAR_API(GdiplusStartup);
EX_DECLEAR_API(GdipLoadImageFromStream);
EX_DECLEAR_API(GdipBitmapLockBits);
EX_DECLEAR_API(GdipBitmapUnlockBits);
EX_DECLEAR_API(GdipDisposeImage);
EX_DECLEAR_API(GdipSetMatrixElements);
EX_DECLEAR_API(GdipGetMatrixElements);
EX_DECLEAR_API(GdipTranslateMatrix);
EX_DECLEAR_API(GdipRotateMatrix);
EX_DECLEAR_API(GdipScaleMatrix);
EX_DECLEAR_API(GdipCreateMatrix);

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
		EX_GET_API(GdipSetMatrixElements);
		EX_GET_API(GdipGetMatrixElements);
		EX_GET_API(GdipTranslateMatrix);
		EX_GET_API(GdipRotateMatrix);
		EX_GET_API(GdipScaleMatrix);
		EX_GET_API(GdipCreateMatrix);
	}
}