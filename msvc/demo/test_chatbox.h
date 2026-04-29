#pragma once
#include "ExDUIR_Func.h"

void test_chatbox(HWND hWnd);
LRESULT CALLBACK OnChatBoxEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnChatButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);


std::wstring GetEditBoxText(HEXOBJ hEditObj);
void AddChatTextItem(HEXOBJ hChatBox, int nRole, LPCWSTR szText);
void AddChatCardItem(HEXOBJ hChatBox, int nRole, LPCWSTR szTitle, LPCWSTR szContent, LPCWSTR szReason, LPCWSTR szReasonTitle, LPCWSTR szButtonText, LPCWSTR szImgPath);
void AddChatBoostModeItem(HEXOBJ hChatBox, int nRole, LPCWSTR szTitle, LPCWSTR szContent, LPCWSTR szImgPath);
void AddChatErrorListItem(HEXOBJ hChatBox, int nRole, LPCWSTR szTitle, LPCWSTR szImgPath, const EX_CHATBOX_ITEMINFO_ERRORLIST_UNIT* pUnits, int nCount);
void AddChatInfoListItem(HEXOBJ hChatBox, int nRole, LPCWSTR szContent, const EX_CHATBOX_ITEMINFO_INFOLIST_UNIT* pUnits, int nCount);
void AddChatMarkdownItem(HEXOBJ hChatBox, int nRole, LPCWSTR szMarkdown);
void AddChatLinkItem(HEXOBJ hChatBox, int nRole, LPCWSTR szContent, LPCWSTR szTitle,
    const EX_CHATBOX_ITEMINFO_LINK_UNIT* pUnits, int nCount);
void AddChatTableListItem(HEXOBJ hChatBox, int nRole, LPCWSTR szContent,
    int nRows, int nCols, const LPCWSTR* pCellTexts);