#include "Easing_ex.h"

void _easing_curve_free(LPVOID pCurveInfo)
{
    Ex_MemFree(pCurveInfo);
}

void _easing_calc_curve(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, size_t param)
{
    INT dwType = ((easinghead_s *)param)->type_;
    INT nCount = ((easinghead_s *)param)->node_count_;
    std::vector<std::vector<FLOAT>> nodes;
    if (dwType == 2)
    {
        nodes.resize(nCount + 4);
        for (INT i = 0; i < nCount + 4; i++)
        {
            nodes[i].resize(2);
        }
        nodes[nCount + 2][0] = 100;
        nodes[nCount + 3][0] = 100;
        nodes[nCount + 2][1] = ((easinghead_s *)param)->dv_;
        nodes[nCount + 3][1] = ((easinghead_s *)param)->dv_;
        RtlMoveMemory(nodes.data() + 16, (LPVOID)(param + offsetof(easinghead_s, node_start_)), nCount * sizeof(easinghead_s));
    }
    else
    {
        nodes.resize(nCount + 2);
        for (INT i = 0; i < nCount + 2; i++)
        {
            nodes[i].resize(2);
        }
        nodes[nCount + 1][0] = 100;
        nodes[nCount + 1][1] = ((easinghead_s *)param)->dv_;
        RtlMoveMemory(nodes.data() + 8, (LPVOID)(param + offsetof(easinghead_s, node_start_)), nCount * sizeof(easinghead_s));
    }
    DOUBLE nPst = 0;
    if (dwType == 1)
    {
        _easing_calc_bezier(nProgress, &nPst, nodes, nCount + 2, (LPVOID)param);
    }
    else if (dwType == 2)
    {
        _easing_calc_Bspline(nProgress, &nPst, nodes, nCount + 4, (LPVOID)param);
    }
    else
    {
        _easing_calc_line(nProgress, &nPst, nodes, nCount + 2, (LPVOID)param);
    }
    *nCurrent = (nStop - nStart) * nPst / 100 + nStart;
}

BOOL _easing_setstate(HEXEASING pEasing, DWORD nState)
{
    BOOL ret = FALSE;
    if (pEasing != 0 && !IsBadWritePtr(pEasing, sizeof(EX_EASING)))
    {
        ((EX_EASING *)pEasing)->nState = nState;
        SetEvent(((EX_EASING *)pEasing)->hEventPause);
        ret = TRUE;
    }
    return ret;
}

DWORD _easing_getstate(HEXEASING pEasing)
{
    DWORD ret = 0;
    if (pEasing != 0 && !IsBadWritePtr(pEasing, sizeof(EX_EASING)))
    {
        ret = ((EX_EASING *)pEasing)->nState;
    }
    return ret;
}

void _easing_progress(HEXEASING pEasing)
{
    INT uType = ((EX_EASING *)pEasing)->dwType;
    LPVOID lpCalcProc = ((EX_EASING *)pEasing)->lpfnEsaing;
    LPVOID pEasingContext = ((EX_EASING *)pEasing)->lpEasingContext;
    LONG_PTR pContext = ((EX_EASING *)pEasing)->pContext;
    INT nFrameCount = ((EX_EASING *)pEasing)->nFrameCount;
    INT nInterval = ((EX_EASING *)pEasing)->nInterval;
    DOUBLE nFrameStep = (DOUBLE)nInterval / ((EX_EASING *)pEasing)->nTotal;
    INT nStart = ((EX_EASING *)pEasing)->nStart;
    INT nStop = ((EX_EASING *)pEasing)->nStop;
    LONG_PTR param1 = ((EX_EASING *)pEasing)->param1;
    LONG_PTR param2 = ((EX_EASING *)pEasing)->param2;
    LONG_PTR param3 = ((EX_EASING *)pEasing)->param3;
    LONG_PTR param4 = ((EX_EASING *)pEasing)->param4;
    INT nMode = LOWORD(((EX_EASING *)pEasing)->dwMode);
    INT nTimes = HIWORD(((EX_EASING *)pEasing)->dwMode);
    if (nTimes <= 0)
        nTimes = 1;
    EX_EASINGINFO EasingInfo;
    DOUBLE nProcess = 0;
    DOUBLE nCurrent = 0;
    LPVOID pProgress;
    LPVOID pCurrent;
    if ((nMode & ES_DISPATCHNOTIFY) == ES_DISPATCHNOTIFY)
    {
        EasingInfo.param1 = param1;
        EasingInfo.param2 = param2;
        EasingInfo.param3 = param3;
        EasingInfo.param4 = param4;
        EasingInfo.pEasing = pEasing;
        EasingInfo.pEasingContext = pEasingContext;
    }
    else
    {
        pProgress = &nProcess;
        pCurrent = &nCurrent;
    }
    BOOL fDesc = TRUE;
    INT nProcessTime = 0;
    while (nTimes > 0 || (nMode & ES_CYCLE) == ES_CYCLE)
    {
        if ((nMode & ES_BACKANDFORTH) == ES_BACKANDFORTH)
        {
            fDesc = (fDesc == TRUE ? FALSE : TRUE);
        }
        else if ((nMode & ES_REVERSE) == ES_REVERSE)
        {
            fDesc = TRUE;
        }
        else
        {
            fDesc = FALSE;
        }
        nProcessTime = GetTickCount64();
        INT i = 1;
        BOOL fStop = FALSE;
        while (i <= nFrameCount)
        {
            Ex_Sleep((nInterval - (GetTickCount64() - nProcessTime)) * 1000);
            if (((EX_EASING *)pEasing)->nState == EES_PAUSE)
            {
                WaitForSingleObject(((EX_EASING *)pEasing)->hEventPause, INFINITE); //停住最大程度节省CPU
                nProcessTime = GetTickCount64();                                    //如果没停住,则延时一段时间节省CPU
                continue;
            }
            nProcess = nFrameStep * i;
            if (!_easing_calc(lpCalcProc, uType, pEasingContext, fDesc ? nStop : nStart, fDesc ? nStart : nStop, nProcess, &nCurrent))
            {
                nProcess = 1;
                ((EX_EASING *)pEasing)->nState = 1;
            }
            if ((nMode & ES_DISPATCHNOTIFY) == ES_DISPATCHNOTIFY)
            {
                EasingInfo.nCurrent = nCurrent;
                EasingInfo.nProgress = nProcess;
                EasingInfo.nTimesSurplus = nTimes - 1;
                LPVOID ptr = 0;
                INT nError = 0;
                if (_handle_validate((EXHANDLE)pContext, HT_DUI, (LPVOID *)&ptr, &nError))
                {
                    fStop = SendMessageW(((wnd_s *)ptr)->hWnd_, g_Li.dwMessage, (WPARAM)&EasingInfo, MAKELONG(EMT_EASING, 0)) != 0;
                }
                if (_handle_validate((EXHANDLE)pContext, HT_OBJECT, (LPVOID *)&ptr, &nError))
                {
                    fStop = Ex_ObjSendMessage((EXHANDLE)pContext, WM_EX_EASING, (WPARAM)pEasing, (size_t)&EasingInfo) != 0;
                }
            }
            else
            {
                fStop = ((EasingPROC2)pContext)(pEasing, nProcess, nCurrent, pEasingContext, nTimes - 1, param1, param2, param3, param4) != 0;
            }
            if (fStop || ((EX_EASING *)pEasing)->nState == EES_STOP)
            {
                nTimes = 0;
                nMode = (nMode & ~ES_CYCLE);
                break;
            }
            i = i + 1;
            nProcessTime = GetTickCount64();
        }
        nTimes = nTimes - 1;
    }
    if ((nMode & ES_RELEASECURVE) == ES_RELEASECURVE && uType == ET_CURVE)
    {
        _easing_curve_free(pEasingContext);
    }
    CloseHandle(((EX_EASING *)pEasing)->hEventPause);
    Ex_MemFree(pEasing);
}

INT CALLBACK _easing_calc(LPVOID lpEasingProc, INT nType, LPVOID pEasingContext, INT nStart, INT nStop, DOUBLE nProgress, DOUBLE *nCurrent)
{
    if (lpEasingProc != 0)
    {
        ((EasingPROC)lpEasingProc)(nProgress, nStart, nStop, nCurrent, pEasingContext);
    }
    else
    {
        _easing_calc_Linear(nProgress, nStart, nStop, nCurrent, pEasingContext);
    }
    return 1;
}

void _easing_calc_line(DOUBLE nProgress, DOUBLE *nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param)
{
    DOUBLE nPst = nProgress * 100;
    DOUBLE n = 0;
    for (INT i = 0; i < 2; i++)
    {
        n = nPst - aNodes[i][0];
        n = (DOUBLE)(n / (aNodes[i + 1][0] - aNodes[i][0]));
        if (nPst <= aNodes[i + 1][0])
        {
            *nCurrent = aNodes[i][1] + (aNodes[i + 1][1] - aNodes[i][1]) * n;
            break;
        }
    }
}

void _easing_calc_bezier(DOUBLE nProgress, DOUBLE *nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param)
{
    if (nCount <= 0)
        return;
    std::vector<std::vector<FLOAT>> tmpTVs;
    tmpTVs.resize(nCount);
    for (INT index = 0; index < nCount; index++)
    {
        tmpTVs[index].resize(2);
    }

    for (INT i = 1; i < nCount; i++)
    {
        for (INT j = 0; j < nCount - i; j++)
        {
            if (i == 1)
            {
                tmpTVs[j][0] = aNodes[j][0] * (1 - nProgress) + aNodes[j + 1][0] * nProgress;
                tmpTVs[j][2] = aNodes[j][2] * (1 - nProgress) + aNodes[j + 1][2] * nProgress;
            }
            else
            {
                tmpTVs[j][0] = tmpTVs[j][0] * (1 - nProgress) + tmpTVs[j + 1][0] * nProgress;
                tmpTVs[j][2] = tmpTVs[j][2] * (1 - nProgress) + tmpTVs[j + 1][2] * nProgress;
            }
        }
    }
    *nCurrent = tmpTVs[0][1];
}

void _easing_calc_Bspline(DOUBLE nProgress, DOUBLE *nCurrent, std::vector<std::vector<FLOAT>> aNodes, INT nCount, LPVOID param)
{
    DOUBLE nPst = nProgress * 100;
    DOUBLE n = 0;
    DOUBLE t = 0;
    DOUBLE a1, a2, a3;
    for (INT i = 0; i < 2; i++)
    {
        n = nPst - aNodes[i][0];
        n = (DOUBLE)(n / (aNodes[i + 1][0] - aNodes[i][0]));
        if (nPst <= aNodes[i + 1][0])
        {
            t = n * n;
            a1 = (DOUBLE)((t - 2 * n + 1) / 2);
            a2 = (DOUBLE)((2 * n - 2 * t + 1) / 2);
            a3 = (DOUBLE)(t / 2);
            *nCurrent = abs(a1 * aNodes[i][1] + a2 * aNodes[i + 1][1] + a3 * aNodes[i + 2][1]);
            break;
        }
    }
}

void _easing_calc_Linear(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * nProgress + nStart;
}

void _easing_calc_InQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * nProgress * nProgress + nStart;
}

void _easing_calc_OutQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = -_ed1 * nProgress * (nProgress - 2) + nStart;
}

void _easing_calc_InOutQuad(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ts = nProgress * 2;
    if (_ts < 1)
    {
        *nCurrent = ((DOUBLE)(nStop - nStart) / 2 * _ts * _ts) + nStart;
        return;
    }
    _ts = _ts - 1;
    *nCurrent = -((DOUBLE)(nStop - nStart) / 2 * (_ts * (_ts - 2) - 1)) + nStart;
}

void _easing_calc_InCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * nProgress * nProgress * nProgress + nStart;
}

void _easing_calc_OutCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress - 1;
    *nCurrent = (DOUBLE)(nStop - nStart) * (_st * _st * _st + 1) + nStart;
}

void _easing_calc_InOutCubic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress * 2;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    if (_st < 1)
    {
        *nCurrent = (DOUBLE)(_ed1 / 2 * _st * _st * _st) + nStart;
        return;
    }
    _st = _st - 2;
    *nCurrent = (DOUBLE)(_ed1 / 2 * (_st * _st * _st + 2)) + nStart;
}

void _easing_calc_InQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * nProgress * nProgress * nProgress * nProgress + nStart;
}

void _easing_calc_OutQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress - 1;
    *nCurrent = -((DOUBLE)(nStop - nStart) * (_st * _st * _st * _st - 1)) + nStart;
}

void _easing_calc_InOutQuart(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress * 2;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    if (_st < 1)
    {
        *nCurrent = (DOUBLE)(_ed1 / 2 * _st * _st * _st * _st) + nStart;
        return;
    }
    _st = _st - 2;
    *nCurrent = -(DOUBLE)(_ed1 / 2 * (_st * _st * _st * _st - 2)) + nStart;
}

void _easing_calc_InQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * nProgress * nProgress * nProgress * nProgress * nProgress + nStart;
}

void _easing_calc_OutQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress - 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * (_st * _st * _st * _st * _st + 1) + nStart;
}

void _easing_calc_InOutQuint(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress * 2;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    if (_st < 1)
    {
        *nCurrent = (DOUBLE)(_ed1 / 2 * _st * _st * _st * _st * _st) + nStart;
        return;
    }
    _st = _st - 2;
    *nCurrent = (DOUBLE)(_ed1 / 2 * (_st * _st * _st * _st * _st + 2)) + nStart;
}

void _easing_calc_InSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = -(DOUBLE)(_ed1 * cos(nProgress / 1 * 3.1415926 / 2)) + _ed1 + nStart;
}

void _easing_calc_OutSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = -(DOUBLE)(_ed1 * sin(nProgress / 1 * 3.1415926 / 2)) + nStart;
}

void _easing_calc_InOutSine(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    INT _ed1 = nStop - nStart;
    *nCurrent = -(DOUBLE)(_ed1 / 2 * (cos(3.1415926 * nProgress / 1) - 1)) + nStart;
}

void _easing_calc_InExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * pow(2, 10 * (nProgress / 1 - 1)) + nStart;
}

void _easing_calc_OutExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = _ed1 * (-pow(2, -10 * nProgress / 1) + 1) + nStart;
}

void _easing_calc_InOutExpo(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress * 2;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    if (_st < 1)
    {
        *nCurrent = (DOUBLE)(_ed1 / 2 * pow(2, 10 * (_st - 1))) + nStart;
        return;
    }
    _st = _st - 1;
    *nCurrent = (DOUBLE)(_ed1 / 2 * (-pow(2, -10 * _st) + 2)) + nStart;
}

void _easing_calc_InCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed1 = (DOUBLE)(nStop - nStart);
    *nCurrent = -_ed1 * (sqrt(1 - nProgress * nProgress) - 1) + nStart;
}

void _easing_calc_OutCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress - 1;
    DOUBLE _ed1 = nStop - nStart;
    *nCurrent = _ed1 * sqrt(1 - _st * _st) + nStart;
}

void _easing_calc_InOutCirc(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = nProgress * 2;
    DOUBLE _ed1 = nStop - nStart;
    if (_st < 1)
    {
        *nCurrent = -(DOUBLE)(_ed1 / 2 * (sqrt(1 - _st * _st) - 1)) + nStart;
        return;
    }
    _st = _st - 2;
    *nCurrent = (DOUBLE)(_ed1 / 2 * (sqrt(1 - _st * _st) + 1)) + nStart;
}

void _easing_calc_InBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    *nCurrent = _easing_calc_getInBounce(nStart, nStop, nProgress);
}

void _easing_calc_OutBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    *nCurrent = _easing_calc_getOutBounce(nStart, nStop, nProgress);
}

void _easing_calc_InOutBounce(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed = nStop - nStart;
    if (nProgress < 0.5)
    {
        *nCurrent = _easing_calc_getInBounce(0, _ed, nProgress * 2) * 0.5 + nStart;
    }
    else
    {
        *nCurrent = _easing_calc_getOutBounce(0, _ed, nProgress * 2 - 1) * 0.5 + _ed * 0.5 + nStart;
    }
}

DOUBLE _easing_calc_getOutBounce(DOUBLE nStart, DOUBLE nStop, DOUBLE nProgress)
{
    DOUBLE _ed = nStop - nStart;
    DOUBLE _st = nProgress;
    if (_st < 1 / 2.75)
    {
        return _ed * 7.5625 * _st * _st + nStart;
    }
    if (_st < 2 / 2.75)
    {
        _st = _st - 1.5 / 2.75;
        return _ed * (7.5625 * _st * _st + 0.75) + nStart;
    }
    if (_st < 2.5 / 2.75)
    {
        _st = _st - 2.25 / 2.75;
        return _ed * (7.5625 * _st * _st + 0.9375) + nStart;
    }
    _st = _st - 2.625 / 2.75;
    return _ed * (7.5625 * _st * _st + 0.984375) + nStart;
}

DOUBLE _easing_calc_getInBounce(DOUBLE nStart, DOUBLE nStop, DOUBLE nProgress)
{
    DOUBLE _ed = nStop - nStart;
    return _ed - _easing_calc_getOutBounce(0, _ed, 1 - nProgress) + nStart;
}

void _easing_calc_InBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed = nStop - nStart;
    DOUBLE _s = 1.70158;
    *nCurrent = _ed * nProgress * nProgress * ((_s + 1) * nProgress - _s) + nStart;
}

void _easing_calc_OutBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed = nStop - nStart;
    DOUBLE _s = 1.70158;
    DOUBLE _st = nProgress - 1;
    *nCurrent = _ed * (_st * _st * ((_s + 1) * _st + _s) + 1) + nStart;
}

void _easing_calc_InOutBack(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed = nStop - nStart;
    DOUBLE _s = 1.70158;
    DOUBLE _st = nProgress * 2;
    if (_st < 1)
    {
        _s = _s * 1.525;
        *nCurrent = (DOUBLE)(_ed / 2 * _st * _st * ((_s + 1) * _st - _s)) + nStart;
        return;
    }
    _st = _st - 2;
    _s = _s * 1.525;
    *nCurrent = (DOUBLE)(_ed / 2 * (_st * _st * ((_s + 1) * _st + _s) + 2)) + nStart;
}

void _easing_calc_InElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    if (nProgress == 0)
    {
        *nCurrent = nStart;
        return;
    }
    if (nProgress == 1)
    {
        *nCurrent = nStop;
        return;
    }
    DOUBLE _ed = nStop - nStart;
    DOUBLE _d = 1;
    DOUBLE _p = _d * 0.3;
    DOUBLE _s = 0;
    DOUBLE _a = 0;
    if (_a = 0 || _a < abs(_ed))
    {
        _a = _ed;
        _s = (DOUBLE)(_p / 4);
    }
    else
    {
        _s = (DOUBLE)(_p / (2 * 3.1415826) * _easing_calc_asin(_ed / _a));
    }
    DOUBLE _st = nProgress - 1;
    *nCurrent = -(_a * pow(2, 10 * _st) * sin((DOUBLE)(_st * _d - _s) * 2 * 3.1415926 / _p)) + nStart;
}

void _easing_calc_OutElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    if (nProgress == 0)
    {
        *nCurrent = nStart;
        return;
    }
    if (nProgress == 1)
    {
        *nCurrent = nStop;
        return;
    }
    DOUBLE _ed = nStop - nStart;
    DOUBLE _d = 1;
    DOUBLE _p = _d * 0.3;
    DOUBLE _s = 0;
    DOUBLE _a = 0;
    if (_a = 0 || _a < abs(_ed))
    {
        _a = _ed;
        _s = (DOUBLE)(_p / 4);
    }
    else
    {
        _s = (DOUBLE)(_p / (3.1415826 * 2) * asin(_ed / _a));
    }
    *nCurrent = _a * pow(2, nProgress * -10) * sin((DOUBLE)(nProgress * _d - _s) * 2 * 3.1415926 / _p) + nStop;
}

void _easing_calc_InOutElastic(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    if (nProgress == 0)
    {
        *nCurrent = nStart;
        return;
    }
    DOUBLE _st = nProgress * 2;
    if (_st == 2)
    {
        *nCurrent = nStop;
        return;
    }
    DOUBLE _ed = nStop - nStart;
    DOUBLE _d = 1;
    DOUBLE _p = _d * 0.3;
    DOUBLE _s = 0;
    DOUBLE _a = 0;
    if (_a == 0 || _a < abs(_ed))
    {
        _a = _ed;
        _s = (DOUBLE)(_p / 4);
    }
    else
    {
        _s = (DOUBLE)(_p / (2 * 3.1415826) * _easing_calc_asin(_ed / _a));
    }
    if (_st < 1)
    {
        _st = _st - 1;
        *nCurrent = -0.5 * _a * pow(2, 10 * _st) * sin((DOUBLE)(_st * _d - _s) * 2 * 3.1415926 / _p) + nStart;
        return;
    }
    _st = _st - 1;
    *nCurrent = _a * pow(2, -10 * _st) * sin((DOUBLE)(_st * _d - _s) * 2 * 3.1415926 / _p) * 0.5 + nStop;
}

DOUBLE _easing_calc_asin(DOUBLE v)
{
    if (v < -1 || v > 1)
    {
        return 0;
    }
    return atan((DOUBLE)v / sqrt(1 - v * v));
}

void _easing_calc_Clerp(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _ed = nStop - nStart;
    DOUBLE _min = 0;
    DOUBLE _max = 360;
    DOUBLE _half = 180;
    DOUBLE _retval = 0;
    DOUBLE _diff = 0;
    if (_ed < -_half)
    {
        _diff = (_max - nStart + nStop) * nProgress;
        _retval = nStart + _diff;
    }
    else if (_ed > _half)
    {
        _diff = -(_max - nStop + nStart) * nProgress;
        _retval = nStart + _diff;
    }
    else
    {
        _retval = nStart + _ed * nProgress;
    }
    *nCurrent = _retval;
}

void _easing_calc_Spring(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    DOUBLE _st = (sin(nProgress * 3.1415826 * (0.2 + 2.5 * nProgress * nProgress * nProgress)) * pow(1 - nProgress, 2.2) + nProgress) * (1 + 1.2 * (1 - nProgress));
    *nCurrent = nStart + (DOUBLE)(nStop - nStart) * _st;
}

void _easing_calc_Punch(DOUBLE nProgress, INT nStart, INT nStop, DOUBLE *nCurrent, LPVOID param)
{
    if (nProgress < 0)
        nProgress = 0;
    if (nProgress > 1)
        nProgress = 1;
    if (nProgress == 0)
    {
        *nCurrent = 0;
        return;
    }
    if (nProgress == 1)
    {
        *nCurrent = 0;
        return;
    }
    DOUBLE _p = 0.3;
    DOUBLE _s = (DOUBLE)(_p / (2 * 3.1415826) * _easing_calc_asin(0));
    *nCurrent = nStop * pow(2, -10 * nProgress) * sin((DOUBLE)(nProgress - _s) * 2 * 3.1415926 / _p);
}

HEXEASING _easing_create(DWORD dwType, LPVOID pEasingContext, DWORD dwMode, LONG_PTR pContext, INT nTotalTime, INT nInterval, DWORD nState, INT nStart, INT nStop, LONG_PTR param1, LONG_PTR param2, LONG_PTR param3, LONG_PTR param4)
{
    HEXEASING pEasing = nullptr;
    if (pContext != 0)
    {
        pEasing = (HEXEASING)Ex_MemAlloc(sizeof(EX_EASING));
        INT nError = 0;
        LPVOID ptr;
        if ((dwMode & ES_DISPATCHNOTIFY) == ES_DISPATCHNOTIFY)
        {
            if (_handle_validate((EXHANDLE)pContext, HT_DUI, (LPVOID *)&ptr, &nError))
            {
            }
            else if (_handle_validate((EXHANDLE)pContext, HT_OBJECT, (LPVOID *)&ptr, &nError))
            {
            }
            else
            {
                Ex_MemFree(pEasing);
                Ex_SetLastError(ERROR_EX_INVALID_OBJECT);
                return 0;
            }
        }
        ((EX_EASING *)pEasing)->pContext = pContext;
        LPVOID lpProc = nullptr;
        std::vector<LPVOID> lpCalcProcs = {&_easing_calc_Linear, &_easing_calc_Clerp, &_easing_calc_Spring, &_easing_calc_Punch, &_easing_calc_InQuad, &_easing_calc_OutQuad, &_easing_calc_InOutQuad, &_easing_calc_InCubic, &_easing_calc_OutCubic, &_easing_calc_InOutCubic, &_easing_calc_InQuart, &_easing_calc_OutQuart, &_easing_calc_InOutQuart, &_easing_calc_InQuint, &_easing_calc_OutQuint, &_easing_calc_InOutQuint, &_easing_calc_InSine, &_easing_calc_OutSine, &_easing_calc_InOutSine, &_easing_calc_InExpo, &_easing_calc_OutExpo, &_easing_calc_InOutExpo, &_easing_calc_InCirc, &_easing_calc_OutCirc, &_easing_calc_InOutCirc, &_easing_calc_InBounce, &_easing_calc_OutBounce, &_easing_calc_InOutBounce, &_easing_calc_InBack, &_easing_calc_OutBack, &_easing_calc_InOutBack, &_easing_calc_InElastic, &_easing_calc_OutElastic, &_easing_calc_InOutElastic};
        if (dwType >= 1 && dwType <= lpCalcProcs.size())
        {
            lpProc = lpCalcProcs[dwType - 1];
        }
        else if (dwType == ET_CUSTOM)
        {
            lpProc = pEasingContext;
        }
        else if (dwType == ET_CURVE)
        {
            lpProc = &_easing_calc_curve;
            if (pEasingContext == 0)
            {
                Ex_MemFree(pEasing);
                Ex_SetLastError(ERROR_EX_MEMORY_BADPTR);
                return 0;
            }
        }
        else
        {
            lpProc = &_easing_calc_Linear;
        }
        if (lpProc == nullptr)
        {
            Ex_MemFree(pEasing);
            Ex_SetLastError(ERROR_EX_MEMORY_BADPTR);
            return 0;
        }
        ((EX_EASING *)pEasing)->dwType = dwType;
        ((EX_EASING *)pEasing)->lpfnEsaing = lpProc;
        ((EX_EASING *)pEasing)->lpEasingContext = pEasingContext;
        ((EX_EASING *)pEasing)->dwMode = dwMode;

        ((EX_EASING *)pEasing)->nCurFrame = 0;
        ((EX_EASING *)pEasing)->nTotal = nTotalTime;
        ((EX_EASING *)pEasing)->nInterval = nInterval;
        ((EX_EASING *)pEasing)->nFrameCount = nTotalTime / nInterval;
        ((EX_EASING *)pEasing)->nState = nState;
        ((EX_EASING *)pEasing)->nStart = nStart;
        ((EX_EASING *)pEasing)->nStop = nStop;
        ((EX_EASING *)pEasing)->param1 = param1;
        ((EX_EASING *)pEasing)->param2 = param2;
        ((EX_EASING *)pEasing)->param3 = param3;
        ((EX_EASING *)pEasing)->param4 = param4;
        ((EX_EASING *)pEasing)->hEventPause = CreateEventW(0, FALSE, TRUE, 0);
        if ((dwMode & ES_THREAD) != 0)
        {
            Thread_Create((LPTHREAD_START_ROUTINE)&_easing_progress, pEasing);
        }
        else
        {
            _easing_progress(pEasing);
        }
    }
    return pEasing;
}