#include "StrokeStyle_ex.h"

void* _strokestyle_create(int startCap, int endCap, int dashCap, int lineJoin, float miterLimit, int dashStyle, float dashOffset)
{
	int nError = 0;
	D2D1_STROKE_STYLE_PROPERTIES pro = {};
	pro.startCap = (D2D1_CAP_STYLE)startCap;
	pro.endCap = (D2D1_CAP_STYLE)endCap;
	pro.dashCap = (D2D1_CAP_STYLE)dashCap;
	pro.lineJoin = (D2D1_LINE_JOIN)lineJoin;
	pro.miterLimit = miterLimit;
	pro.dashStyle = (D2D1_DASH_STYLE)dashStyle;
	pro.dashOffset = dashOffset;
	void* strokeStyle = nullptr;
	nError = ((ID2D1Factory*)g_Ri.pD2Dfactory)->CreateStrokeStyle(pro, NULL, 0, (ID2D1StrokeStyle**)&strokeStyle);
	Ex_SetLastError(nError);
	return strokeStyle;
}

bool _strokestyle_destroy(void* hstrokestyle)
{
	int nError = 0;
	if (hstrokestyle != 0)
	{
		nError = ((ID2D1StrokeStyle*)hstrokestyle)->Release();
	}
	return nError == 0;
}