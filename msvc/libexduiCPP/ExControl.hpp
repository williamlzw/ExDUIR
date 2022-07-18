#pragma once
#include "ExUIbase.hpp"

using namespace ExDUIR::FrameWorks;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Control
		{
			class ExControl : virtual public ExUIbase
			{
			public:
				ExControl() = default;
				~ExControl() {};
				ExControl(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExControl(ExUIbase pOwner, INT x, INT y, INT width, INT height, LPCWSTR lptszClassName, LPCWSTR lptszObjTitle = NULL, INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = nullptr, MsgPROC lpfnMsgProc = NULL)
				{
					m_handle = Ex_ObjCreateEx(dwStyleEx, lptszClassName, lptszObjTitle, dwStyle, x, y, width, height, pOwner.m_handle, nID, dwTextFormat, lParam, hTheme, lpfnMsgProc);
				}

				inline BOOL IsValidate() { return Ex_ObjIsValidate(m_handle); }
				inline BOOL IsVisible() { return Ex_ObjIsVisible(m_handle); }
				inline BOOL Visible(BOOL bVisible) { return Ex_ObjShow(m_handle, bVisible); }
				inline BOOL IsEnable() { return Ex_ObjIsEnable(m_handle); }
				inline BOOL Enable(BOOL bEnable) { return Ex_ObjEnable(m_handle, bEnable); }
				inline BOOL Move(INT left, INT top, INT width, INT height, BOOL bRepaint) { Ex_ObjMove(m_handle, left, top, width, height, bRepaint); }

				RECT GetRect()
				{
					RECT rc{};
					Ex_ObjGetRect(m_handle, &rc);
					return rc;
				}

				RECT GetClientRect()
				{
					RECT rc{};
					Ex_ObjGetClientRect(m_handle, &rc);
					return rc;
				}

				std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle);
					std::wstring str;
					str.resize(len * 2 + 2 );
					Ex_ObjGetText(m_handle, str.c_str(), len *2+ 2 );
					return str;
				}

				BOOL SetText(LPCWSTR text, BOOL bRepaint = FALSE)
				{
					return Ex_ObjSetText(m_handle, text, bRepaint);
				}

				inline BOOL GetBackgroundImage(EX_BACKGROUNDIMAGEINFO& BkgInfo) { return Ex_ObjGetBackgroundImage(m_handle, &BkgInfo); }

				inline BOOL SetBackgroundImage(LPVOID lpImage, size_t dwImageLen, INT x, INT y, DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha, BOOL fUpdate) { return Ex_ObjSetBackgroundImage(m_handle, lpImage, dwImageLen, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, fUpdate); }

				inline size_t GetLong(INT nIndex)
				{
					return Ex_ObjGetLong(m_handle, nIndex);
				}

				inline size_t SetLong(INT nIndex, size_t newLong)
				{
					return Ex_ObjSetLong(m_handle, nIndex, newLong);
				}

				inline BOOL SetPadding(INT nPaddingType, INT left, INT top, INT right, INT bottom, BOOL fRepaint)
				{
					return Ex_ObjSetPadding(m_handle, nPaddingType, left, top, right, bottom, fRepaint);
				}

				inline BOOL PostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjPostMessage(m_handle, uMsg, wParam, lParam);
				}

				inline size_t SendMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjSendMessage(m_handle, uMsg, wParam, lParam);
				}

				inline BOOL SetBackgroundPlayState(BOOL fPlayFrames, BOOL fResetFrame, BOOL fRepaint)
				{
					return Ex_ObjSetBackgroundPlayState(m_handle, fPlayFrames, fResetFrame, fRepaint);
				}

				inline BOOL SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL fRepaint)
				{
					return Ex_ObjSetRadius(m_handle, topleft, topright, bottomright, bottomleft, fRepaint);
				}

				inline BOOL SetPos(ExControl InsertAfter, INT x, INT y, INT width, INT height, INT flags)
				{
					return Ex_ObjSetPos(m_handle, InsertAfter.m_handle, x, y, width, height, flags);
				}

				inline BOOL HandleEvent(INT nEvent, EventHandlerPROC pfnCallback)
				{
					return Ex_ObjHandleEvent(m_handle, nEvent, pfnCallback);
				}

				inline EXARGB GetColor(INT nIndex)
				{
					return Ex_ObjGetColor(m_handle, nIndex);
				}

				inline BOOL SetColor(INT nIndex, EXARGB nColor, BOOL fRepaint)
				{
					return Ex_ObjSetColor(m_handle, nIndex, nColor, fRepaint);
				}

				inline BOOL BeginPaint(EX_PAINTSTRUCT& ps)
				{
					return Ex_ObjBeginPaint(m_handle, &ps);
				}

				inline BOOL EndPaint(EX_PAINTSTRUCT& ps)
				{
					return Ex_ObjEndPaint(m_handle, &ps);
				}

				ExControl GetFocus()
				{
					auto hObj = Ex_ObjGetFocus(m_handle);
					auto ret = ExControl(hObj);
					return ret;
				}

				inline BOOL KillFocus()
				{
					return Ex_ObjKillFocus(m_handle);
				}

				inline BOOL TrackPopupMenu(HMENU hMenu, DWORD uFlags, INT x, INT y, size_t nReserved, RECT* lpRC, MsgPROC pfnCallback, DWORD dwFlags)
				{
					return Ex_TrackPopupMenu(hMenu, uFlags, x, y, nReserved, m_handle, lpRC, pfnCallback, dwFlags);
				}

				inline static size_t CallProc(ClsPROC lpPrevObjProc, HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjCallProc(lpPrevObjProc, hWnd, hObj, uMsg, wParam, lParam);
				}

				inline static size_t DefProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
				}

				inline size_t DispatchMsg(INT uMsg, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjDispatchMessage(m_handle, uMsg, wParam, lParam);
				}

				inline size_t DispatchNotify(INT nCode, WPARAM wParam, LPARAM lParam)
				{
					return Ex_ObjDispatchNotify(m_handle, nCode, wParam, lParam);
				}

				inline BOOL ClientToScreen(INT* x, INT* y)
				{
					return Ex_ObjClientToScreen(m_handle, x, y);
				}

				inline BOOL ClientToWindow(INT* x, INT* y)
				{
					return Ex_ObjClientToWindow(m_handle, x, y);
				}

				inline BOOL Invalidate()
				{
					return Ex_ObjInvalidateRect(m_handle, NULL);
				}

				inline BOOL Invalidate(RECT rc)
				{
					return Ex_ObjInvalidateRect(m_handle, &rc);
				}

				inline HANDLE SetTimer(INT uElapse)
				{
					return Ex_ObjSetTimer(m_handle, uElapse);
				}

				inline BOOL KillTimer()
				{
					return Ex_ObjKillTimer(m_handle);
				}

				inline BOOL SetFont(LPCWSTR lpszFontfamily, INT dwFontsize, INT dwFontstyle = 0, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetFontFromFamily(m_handle, lpszFontfamily, dwFontsize, dwFontstyle, fRepaint);
				}

				inline BOOL EnablePaintingMsg(BOOL bEnable)
				{
					return Ex_ObjEnablePaintingMsg(m_handle, bEnable);
				}

				inline INT GetDropString(LPVOID pDataObject, LPWSTR lpwzBuffer, INT cchMaxLength)
				{
					return Ex_ObjGetDropString(m_handle, pDataObject, lpwzBuffer, cchMaxLength);
				}

				inline BOOL CheckDropFormat(LPVOID pDataObject, DWORD dwFormat)
				{
					return Ex_ObjCheckDropFormat(m_handle, pDataObject, dwFormat);
				}

				inline ExControl GetFromID(INT nID)
				{
					auto hObj = Ex_ObjGetFromID(m_handle, nID);

					return ExControl(hObj);
				}
			};
		}
	}
}