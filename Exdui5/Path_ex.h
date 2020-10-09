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

bool _path_destroy(size_t hPath);
bool _path_reset(size_t hPath);
bool _path_create(int dwFlags, size_t* hPath);
bool _path_getbounds(size_t hPath, void* lpBounds);
bool _path_open(size_t hPath);
bool _path_close(size_t hPath);
bool _path_beginfigure(size_t hPath);
bool _path_endfigure(size_t hPath, bool fCloseFigure);
bool _path_hittest(size_t hPath, float x, float y);
bool _path_addline(size_t hPath, float x1, float y1, float x2, float y2);
bool _path_addarc(size_t hPath, float x1, float y1, float x2, float y2, float radiusX, float radiusY, bool fClockwise);
bool _path_addrect(size_t hPath, float left, float top, float right, float bottom);
bool _path_addroundedrect(size_t hPath, float left, float top, float right, float bottom, float radiusTopLeft, float radiusTopRight, float radiusBottomLeft, float radiusBottomRight);
