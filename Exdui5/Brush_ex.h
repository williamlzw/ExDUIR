#pragma once
#include "help_ex.h"
#include "Canvas_ex.h"
#include "Global_ex.h"

void* _brush_create(int argb);
int _brush_destroy(void* hBrush);
void _brush_setcolor(void* hBrush, int argb);
void* _brush_createfromimg(ExHandle hImg);
void* _brush_createfromcanvas(ExHandle hCanvas);
void _brush_settransform(void* hBrush, void* matrix);
