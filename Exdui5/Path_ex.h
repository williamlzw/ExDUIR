#pragma once
#include "help_ex.h"
#include "DirectX_ex.h"

#define epf_bOpened 2147483648
#define EPF_DISABLESCALE 1

struct path_s
{
	int dwFlags_;
	void* pObj_;
	void* pGeometry_;
};

bool _path_destroy(EXHANDLE hPath);
bool _path_reset(EXHANDLE hPath);
bool _path_create(int dwFlags, EXHANDLE* hPath);
bool _path_getbounds(EXHANDLE hPath, void* lpBounds);
bool _path_open(EXHANDLE hPath);
bool _path_close(EXHANDLE hPath);
bool _path_beginfigure(EXHANDLE hPath);
bool _path_endfigure(EXHANDLE hPath, bool fCloseFigure);
bool _path_hittest(EXHANDLE hPath, float x, float y);
bool _path_addline(EXHANDLE hPath, float x1, float y1, float x2, float y2);
bool _path_addarc(EXHANDLE hPath, float x1, float y1, float x2, float y2, float radiusX, float radiusY, bool fClockwise);
bool _path_addrect(EXHANDLE hPath, float left, float top, float right, float bottom);
bool _path_addroundedrect(EXHANDLE hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight);
