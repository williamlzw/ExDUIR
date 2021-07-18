#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

typedef size_t(*EasingPROC)(DOUBLE, INT, INT, LPVOID, LPVOID);
typedef size_t(*EasingPROC2)(LPVOID, DOUBLE, DOUBLE,LPVOID, INT, size_t, size_t, size_t, size_t);

struct EX_EASING
{
	DWORD dwType;
	DWORD dwMode;
	LPVOID lpfnEsaing;
	LPVOID lpEasingContext;
	LONG_PTR pContext;
	HANDLE hEventPause;
	INT nCurFrame;
	INT nTotal;
	INT nInterval;
	INT nFrameCount;
	INT nStart;
	INT nStop;
	DWORD nState;
	LONG_PTR param1;
	LONG_PTR param2;
	LONG_PTR param3;
	LONG_PTR param4;
};

struct easinghead_s
{
	INT type_;
	FLOAT dv_;
	INT node_count_;
	INT node_start_;
};

void _easing_curve_free(LPVOID pCurveInfo);

void _easing_progress(HEXEASING pEasing);
INT _easing_calc(LPVOID lpEasingProc, INT nType, LPVOID pEasingContext, INT nStart, INT nStop, DOUBLE nProgress, DOUBLE* nCurrent);
void _easing_calc_line(DOUBLE nProgress, DOUBLE* nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param);
void _easing_calc_curve(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, size_t param);
void _easing_calc_bezier(DOUBLE nProgress, DOUBLE* nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param);
void _easing_calc_Bspline(DOUBLE nProgress, DOUBLE* nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param);
void _easing_calc_Linear(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
DOUBLE _easing_calc_getOutBounce(DOUBLE nStart, DOUBLE nStop, DOUBLE nProgress);
DOUBLE _easing_calc_getInBounce(DOUBLE nStart, DOUBLE nStop, DOUBLE nProgress);
void _easing_calc_InBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_OutElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_InOutElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
DOUBLE _easing_calc_asin(DOUBLE v);
void _easing_calc_Clerp(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_Spring(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);
void _easing_calc_Punch(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE* nCurrent, LPVOID param);

