#pragma once
#include "help_ex.h"


void* _brush_create(int argb);
int _brush_destroy(void* hBrush);
void _brush_setcolor(void* hBrush, int argb);
void* _brush_createfromimg(EXHANDLE hImg);
void* _brush_createfromcanvas(EXHANDLE hCanvas);
void _brush_settransform(void* hBrush, matrix_s* matrix);
