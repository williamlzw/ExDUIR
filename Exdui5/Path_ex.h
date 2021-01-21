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

bool _path_destroy(ExHandle hPath);
bool _path_reset(ExHandle hPath);
bool _path_create(int dwFlags, ExHandle* hPath);
bool _path_getbounds(ExHandle hPath, void* lpBounds);
bool _path_open(ExHandle hPath);
bool _path_close(ExHandle hPath);
bool _path_beginfigure(ExHandle hPath);
bool _path_endfigure(ExHandle hPath, bool fCloseFigure);
bool _path_hittest(ExHandle hPath, float x, float y);
bool _path_addline(ExHandle hPath, float x1, float y1, float x2, float y2);
bool _path_addarc(ExHandle hPath, float x1, float y1, float x2, float y2, float radiusX, float radiusY, bool fClockwise);
bool _path_addrect(ExHandle hPath, float left, float top, float right, float bottom);
bool _path_addroundedrect(ExHandle hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight);
