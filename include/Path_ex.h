#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

#define epf_bOpened 2147483648
#define EPF_DISABLESCALE 1

struct path_s
{
	UINT dwFlags_;
	ID2D1GeometrySink *pObj_;
	ID2D1PathGeometry *pGeometry_;
};