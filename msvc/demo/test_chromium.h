#pragma once
#include "ExDUIR_Func.h"


LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam,
                                      LPARAM lParam, LRESULT* lpResult);
void CALLBACK    OnFunction(LPCWSTR name, HV8VALUE object, std::vector<uintptr_t*> arguments,
                            uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam);
void CALLBACK    OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1,
                                     LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4,
                                     bool* pbHWEBVIEWd, void* lParam);
LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void             test_chromium(HWND hParent);
