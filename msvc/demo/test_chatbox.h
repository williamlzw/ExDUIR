#pragma once
#include "ExDUIR_Func.h"

void test_chatbox(HWND hWnd);
LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnChatButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);


std::wstring GetEditBoxText(HEXOBJ hEditObj);
void AddChatTextItem(HEXOBJ hChatBox, int nRole, LPCWSTR szText);
void AddChatMarkdownItem(HEXOBJ hChatBox, int nRole, LPCWSTR szMarkdown);