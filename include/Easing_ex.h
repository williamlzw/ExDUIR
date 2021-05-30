#pragma once
#include "help_ex.h"
#include "ExDUIR_Func.h"

typedef size_t(*EasingPROC)(double, int, int, void*, void*);
typedef size_t(*EasingPROC2)(void*, double, double,void*, int, size_t, size_t, size_t, size_t);

struct EX_EASING
{
	DWORD dwType;
	DWORD dwMode;
	LPVOID lpfnEsaing;
	LPVOID lpEasingContext;
	LPVOID pContext;
	HANDLE hEventPause;
	INT nCurFrame;
	INT nTotal;
	INT nInterval;
	INT nFrameCount;
	INT nStart;
	INT nStop;
	DWORD nState;
	size_t param1;
	size_t param2;
	size_t param3;
	size_t param4;
};

struct easinghead_s
{
	int type_;
	float dv_;
	int node_count_;
	int node_start_;
};

void _easing_curve_free(void* pCurveInfo);

void _easing_progress(HEXEASING pEasing);
int _easing_calc(void* lpEasingProc, int nType, void* pEasingContext, int nStart, int nStop, double nProgress, double* nCurrent);
void _easing_calc_line(double nProgress, double* nCurrent, std::vector<std::vector<float>> aNodes, int nCount, void* param);
void _easing_calc_curve(double nProgress, int nStart, int nStop, double* nCurrent, size_t param);
void _easing_calc_bezier(double nProgress, double* nCurrent, std::vector<std::vector<float>> aNodes, int nCount, void* param);
void _easing_calc_Bspline(double nProgress, double* nCurrent, std::vector<std::vector<float>> aNodes, int nCount, void* param);
void _easing_calc_Linear(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutQuad(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutCubic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutQuart(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutQuint(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutSine(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutExpo(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutCirc(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutBounce(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
double _easing_calc_getOutBounce(double nStart, double nStop, double nProgress);
double _easing_calc_getInBounce(double nStart, double nStop, double nProgress);
void _easing_calc_InBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutBack(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_OutElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_InOutElastic(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
double _easing_calc_asin(double v);
void _easing_calc_Clerp(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_Spring(double nProgress, int nStart, int nStop, double* nCurrent, void* param);
void _easing_calc_Punch(double nProgress, int nStart, int nStop, double* nCurrent, void* param);

