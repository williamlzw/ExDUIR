#include "StrokeStyle_ex.h"

ID2D1StrokeStyle* _strokestyle_create(D2D1_CAP_STYLE startCap, D2D1_CAP_STYLE endCap, D2D1_CAP_STYLE dashCap, D2D1_LINE_JOIN lineJoin, FLOAT miterLimit, D2D1_DASH_STYLE dashStyle, FLOAT dashOffset)
{
	INT nError = 0;
	D2D1_STROKE_STYLE_PROPERTIES pro = {};
	pro.startCap = startCap;
	pro.endCap = endCap;
	pro.dashCap = dashCap;
	pro.lineJoin = lineJoin;
	pro.miterLimit = miterLimit;
	pro.dashStyle = dashStyle;
	pro.dashOffset = dashOffset;
	ID2D1StrokeStyle* strokeStyle = nullptr;
	nError = g_Ri.pD2Dfactory->CreateStrokeStyle(pro, NULL, 0, &strokeStyle);
	Ex_SetLastError(nError);
	return strokeStyle;
}

BOOL _strokestyle_destroy(ID2D1StrokeStyle* hstrokestyle)
{
	INT nError = 0;
	if (hstrokestyle != 0)
	{
		nError = hstrokestyle->Release();
	}
	return nError == 0;
}