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
				~ExControl() = default;
				ExControl(EXHANDLE hObj)
				{
					m_handle = hObj;
				}
				ExControl(HWND hWnd, POINT point)
				{
					m_handle = Ex_DUIGetObjFromPoint((EXHANDLE)hWnd, point.x, point.y);
				}
				ExControl(ExUIbase pOwner, INT x, INT y, INT width, INT height, std::wstring lptszClassName, std::wstring lptszObjTitle = NULL, INT dwStyle = -1, INT dwStyleEx = -1, INT dwTextFormat = -1, INT nID = NULL, LPARAM lParam = NULL, HEXTHEME hTheme = nullptr, MsgPROC lpfnMsgProc = NULL)
				{
					m_handle = Ex_ObjCreateEx(dwStyleEx, lptszClassName.c_str(), lptszObjTitle.c_str(), dwStyle, x, y, width, height, pOwner.m_handle, nID, dwTextFormat, lParam, hTheme, lpfnMsgProc);
				}

				inline BOOL IsValidate() { return Ex_ObjIsValidate(m_handle); }
				inline BOOL IsVisible() { return Ex_ObjIsVisible(m_handle); }
				inline BOOL Visible(BOOL bVisible) { return Ex_ObjShow(m_handle, bVisible); }
				inline BOOL IsEnable() { return Ex_ObjIsEnable(m_handle); }
				inline BOOL Enable(BOOL bEnable) { return Ex_ObjEnable(m_handle, bEnable); }
				inline BOOL Move(INT left, INT top, INT width, INT height, BOOL bRepaint = FALSE) { return Ex_ObjMove(m_handle, left, top, width, height, bRepaint); }

				inline BOOL GetRect(RECT& rc)
				{
					return Ex_ObjGetRect(m_handle, &rc);
				}

				inline BOOL GetClientRect(RECT& rc)
				{
					return Ex_ObjGetClientRect(m_handle, &rc);
				}

				inline std::wstring GetText()
				{
					size_t len = Ex_ObjGetTextLength(m_handle);
					std::wstring str;
					str.resize(len * 2);
					Ex_ObjGetText(m_handle, str.c_str(), len * 2);
					return str;
				}

				inline BOOL SetText(std::wstring text, BOOL bRepaint = FALSE)
				{
					return Ex_ObjSetText(m_handle, text.c_str(), bRepaint);
				}

				inline BOOL GetBackgroundImage(EX_BACKGROUNDIMAGEINFO& BkgInfo) { return Ex_ObjGetBackgroundImage(m_handle, &BkgInfo); }

				inline size_t GetLong(INT nIndex)
				{
					return Ex_ObjGetLong(m_handle, nIndex);
				}

				inline size_t SetLong(INT nIndex, size_t newLong)
				{
					return Ex_ObjSetLong(m_handle, nIndex, newLong);
				}

				inline BOOL SetPadding(INT nPaddingType, INT left, INT top, INT right, INT bottom, BOOL fRepaint = FALSE)
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

				inline BOOL SetBackgroundPlayState(BOOL fPlayFrames, BOOL fResetFrame, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetBackgroundPlayState(m_handle, fPlayFrames, fResetFrame, fRepaint);
				}

				inline BOOL SetRadius(FLOAT topLeft, FLOAT topRight, FLOAT bottomRight, FLOAT bottomLeft, BOOL fUpdate = FALSE)
				{
					return Ex_ObjSetRadius(m_handle, topLeft, topRight, bottomRight, bottomLeft, fUpdate);
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

				inline BOOL BeginPaint(EX_PAINTSTRUCT& ps)
				{
					return Ex_ObjBeginPaint(m_handle, &ps);
				}

				inline BOOL EndPaint(EX_PAINTSTRUCT& ps)
				{
					return Ex_ObjEndPaint(m_handle, &ps);
				}

				inline ExControl GetFocus()
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

				inline INT SetTimer(INT uElapse)
				{
					return Ex_ObjSetTimer(m_handle, uElapse);
				}

				inline BOOL KillTimer()
				{
					return Ex_ObjKillTimer(m_handle);
				}

				inline BOOL SetFont(std::wstring lpszFontfamily, INT dwFontsize, INT dwFontstyle = 0, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetFontFromFamily(m_handle, lpszFontfamily.c_str(), dwFontsize, dwFontstyle, fRepaint);
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

				inline ExControl GetFromNodeID(INT nID)
				{
					auto hObj = Ex_ObjGetFromNodeID(m_handle, nID);
					return ExControl(hObj);
				}

				inline ExControl GetParent()
				{
					auto hObj = Ex_ObjGetParent(m_handle);
					return ExControl(hObj);
				}

				inline BOOL SetBackgroundImage(std::vector<CHAR> imageData, INT x = 0, INT y = 0, DWORD dwRepeat = BACKGROUND_REPEAT_ZOOM, RECT* lpGrid = NULL, INT dwFlags = BACKGROUND_FLAG_DEFAULT, DWORD dwAlpha = 255, BOOL fUpdate = FALSE)
				{
					return Ex_ObjSetBackgroundImage(m_handle, imageData.data(), imageData.size(), x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, fUpdate);
				}
				inline BOOL SetBackgroundImage(std::wstring imageFilePath, INT x = 0, INT y = 0, DWORD dwRepeat = BACKGROUND_REPEAT_ZOOM, RECT* lpGrid = NULL, INT dwFlags = BACKGROUND_FLAG_DEFAULT, DWORD dwAlpha = 255, BOOL fUpdate = FALSE)
				{
					std::vector<CHAR> imgdata;
					Ex_ReadFile(imageFilePath.c_str(), &imgdata);
					return SetBackgroundImage(imgdata, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, fUpdate);
				}

				inline EXARGB GetColorBackground()
				{
					return Ex_ObjGetColor(m_handle, COLOR_EX_BACKGROUND);
				}

				inline BOOL SetColorBackground(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_BACKGROUND, nColor, fRepaint);
				}

				inline BOOL SetColorBorder(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_BORDER, nColor, fRepaint);
				}

				inline BOOL SetColorTextNormal(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_NORMAL, nColor, fRepaint);
				}

				inline BOOL SetColorTextFocus(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_FOCUS, nColor, fRepaint);
				}

				inline BOOL SetColorTextHover(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_HOVER, nColor, fRepaint);
				}

				inline BOOL SetColorTextDown(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_DOWN, nColor, fRepaint);
				}

				inline BOOL SetColorTextChecked(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_CHECKED, nColor, fRepaint);
				}

				inline BOOL SetColorTextSelect(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_SELECT, nColor, fRepaint);
				}

				inline BOOL SetColorTextVisted(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_VISTED, nColor, fRepaint);
				}

				inline BOOL SetColorTextShadow(EXARGB nColor, BOOL fRepaint = FALSE)
				{
					return Ex_ObjSetColor(m_handle, COLOR_EX_TEXT_SHADOW, nColor, fRepaint);
				}

				inline BOOL SetUIState(DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw)
				{
					return Ex_ObjSetUIState(m_handle, dwState, fRemove, lprcRedraw, fRedraw);
				}

				inline void SetLongAlpha(DWORD alpha)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_ALPHA, alpha);
				}

				inline DWORD GetLongAlpha()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_ALPHA);
				}

				inline void SetLongBlur(DWORD blur)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_BLUR, blur);
				}

				inline DWORD GetLongBlur()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_BLUR);
				}

				inline void SetLongCursor(HCURSOR cursor)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_CURSOR, (LPARAM)cursor);
				}

				inline HCURSOR GetLongCursor()
				{
					return (HCURSOR)Ex_ObjGetLong(m_handle, OBJECT_LONG_CURSOR);
				}

				inline void SetLongStyleEx(size_t styleEx)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_EXSTYLE, styleEx);
				}

				inline size_t GetLongStyleEx()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_EXSTYLE);
				}

				inline void SetLongStyle(size_t style)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_STYLE, style);
				}

				inline size_t GetLongStyle()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_STYLE);
				}

				inline void SetLongState(INT state)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_STATE, state);
				}

				inline INT GetLongState()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_STATE);
				}

				inline void SetLongID(size_t ID)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_ID, ID);
				}

				inline size_t GetLongID()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_ID);
				}

				inline void SetLongNodeID(size_t nodeID)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_NODEID, nodeID);
				}

				inline size_t GetLongNodeID()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_NODEID);
				}

				inline void SetLongLParam(LONG_PTR LParam)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_LPARAM, LParam);
				}

				inline LONG_PTR GetLongLParam()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_LPARAM);
				}

				inline void SetLongOwner(LONG_PTR owner)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_OWNER, owner);
				}

				inline LONG_PTR GetLongOwner()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_OWNER);
				}

				inline LONG_PTR SetLongUserData(LONG_PTR userdata)
				{
					return (LONG_PTR)Ex_ObjSetLong(m_handle, OBJECT_LONG_USERDATA, userdata);
				}

				inline LONG_PTR GetLongUserData()
				{
					return Ex_ObjGetLong(m_handle, OBJECT_LONG_USERDATA);
				}

				inline void SetLongProc(MsgPROC proc)
				{
					Ex_ObjSetLong(m_handle, OBJECT_LONG_OBJPROC, (size_t)proc);
				}
			};
		}
	}
}