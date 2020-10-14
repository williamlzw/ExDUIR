#pragma once
#include "help_ex.h"

EX_DEFINE_API(GdiplusStartup, int, (void*, void*,void*));
EX_DEFINE_API(GdipLoadImageFromStream, int, (void*, void**));
EX_DEFINE_API(GdipBitmapLockBits, int, (void*, void*,int,int,void*));
EX_DEFINE_API(GdipBitmapUnlockBits, int, (void*,void*));
EX_DEFINE_API(GdipDisposeImage, int, (void*));
EX_DEFINE_API(GdipSetMatrixElements, int, (void*,float,float,float,float,float,float));
EX_DEFINE_API(GdipGetMatrixElements, int, (void*, void*));
EX_DEFINE_API(GdipTranslateMatrix, int, (void*, float,float,int));
EX_DEFINE_API(GdipRotateMatrix, int, (void*, float,  int));
EX_DEFINE_API(GdipScaleMatrix, int, (void*, float, float, int));
EX_DEFINE_API(GdipCreateMatrix, int, (void**));

void º”‘ÿGdiplusDLL();

struct BitmapData_s
{
    int Width;
    int Height;
    int Stride;
    int PixelFormat;
    void* Scan0;
    void* Reserved;
};