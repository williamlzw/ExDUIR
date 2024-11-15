#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"
#include "ExWindow.hpp"

using namespace ExDUIR::FrameWorks::Control;
using namespace ExDUIR::FrameWorks;

namespace ExDUIR
{
	namespace FrameWorks
	{
		class ExSkin : public ExUIbase
		{
		public:
			HWND m_hWnd = 0;
			ExSkin() = default;
			~ExSkin() {};
			ExSkin(EXHANDLE hObj)
			{
				m_handle = hObj;
			}

			ExSkin(ExSkin pOwner, INT dwStyleDui, DWORD dwStyleExWindow = NULL, WinMsgPROC lpfnMsgProc = NULL)
			{
				auto hWndParent = pOwner.m_hWnd == NULL ? 0 : pOwner.m_hWnd;
				m_handle = Ex_DUIBindWindowEx(hWndParent, 0, dwStyleDui, 0, lpfnMsgProc);
			}

			ExSkin(HWND pOwner, INT x, INT y, INT width, INT height, std::wstring sWindowName, INT dwStyleDui, DWORD dwStyleWindow = NULL, DWORD dwStyleExWindow = NULL, WinMsgPROC lpfnMsgProc = NULL)
			{
				m_hWnd = Ex_WndCreate(pOwner, 0, sWindowName.c_str(), x, y, width, height, dwStyleWindow, dwStyleExWindow);
				m_handle = Ex_DUIBindWindowEx(m_hWnd, 0, dwStyleDui, 0, lpfnMsgProc);
			}

			ExSkin(ExWindow pWindow, INT dwStyleDui, DWORD dwStyleExWindow = NULL, WinMsgPROC lpfnMsgProc = NULL)
			{
				m_handle = Ex_DUIBindWindowEx(pWindow.m_hWnd, 0, dwStyleDui, 0, lpfnMsgProc);
			}
			inline size_t SetLong(INT nIndex, size_t dwNewLong) { return Ex_DUISetLong(m_handle, nIndex, dwNewLong); }
			inline size_t GetLong(INT nIndex) { return Ex_DUIGetLong(m_handle, nIndex); }
			inline BOOL Show(INT nCmdShow = SW_SHOWNORMAL, INT dwTimer = NULL, INT dwFrames = NULL, INT dwFlags = NULL) { return Ex_DUIShowWindow(m_handle, nCmdShow, dwTimer, dwFrames, dwFlags); };
			inline BOOL IsValidate() { return IsWindow(m_hWnd) && m_handle != 0; }
			inline BOOL IsEnable() { return IsWindowEnabled(m_hWnd); }
			inline BOOL Enable(BOOL bEnable) { return EnableWindow(m_hWnd, bEnable); }
			inline BOOL IsVisible() { return IsWindowVisible(m_hWnd); }
			inline BOOL IsVisible(BOOL bVisible) { return Ex_DUIShowWindow(m_handle, bVisible ? SW_SHOWNORMAL : SW_HIDE, 0, 0, 0); }
			inline BOOL SetTitle(std::wstring title) { return SetWindowTextW(m_hWnd, title.c_str()); }
			inline BOOL Move(INT x, INT y, INT width, INT height, BOOL bRepaint = FALSE) { return MoveWindow(m_hWnd, x, y, width, height, bRepaint); }
			inline BOOL PostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) { PostMessageW(m_hWnd, uMsg, wParam, lParam); }
			inline std::wstring GetTitle()
			{
				INT len = GetWindowTextLengthW(m_hWnd);
				std::wstring retTitle;
				retTitle.resize(len + 1);
				GetWindowTextW(m_hWnd, (LPWSTR)retTitle.c_str(), len + 1);
				return retTitle;
			}

			inline BOOL GetClientRect(RECT* rc)
			{
				return Ex_DUIGetClientRect(m_handle, rc);
			}
			
			inline ExControl GetCaptionObj()
			{
				return ExControl(Ex_DUIGetLong(m_handle, ENGINE_LONG_OBJCAPTION));
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

			inline ExControl GetFromID(INT nID)
			{
				auto hObj = Ex_ObjGetFromID(m_handle, nID);
				return ExControl(hObj);
			}

			inline EXARGB GetBackgroundColor()
			{
				return (EXARGB)GetLong(ENGINE_LONG_CRBKG);
			}

			inline EXARGB SetBackgroundColor(EXARGB crColor)
			{
				return (EXARGB)SetLong(ENGINE_LONG_CRBKG, crColor);
			}

			inline DWORD GetAlpha()
			{
				return GetLong(ENGINE_LONG_ALPHA);
			}

			inline DWORD SetAlpha(DWORD alpha)
			{
				return (DWORD)SetLong(ENGINE_LONG_ALPHA, alpha);
			}

			inline DWORD GetBlur()
			{
				return GetLong(ENGINE_LONG_BLUR);
			}

			inline DWORD SetBlur(DWORD blur)
			{
				return (DWORD)SetLong(ENGINE_LONG_BLUR, blur);
			}

			inline DWORD GetRadius()
			{
				return GetLong(ENGINE_LONG_RADIUS);
			}

			inline DWORD SetRadius(DWORD blur)
			{
				return (DWORD)SetLong(ENGINE_LONG_RADIUS, blur);
			}

			inline EXARGB GetBorderColor()
			{
				return GetLong(ENGINE_LONG_CRBORDER);
			}

			inline EXARGB SetBorderColor(EXARGB color)
			{
				return (EXARGB)SetLong(ENGINE_LONG_CRBORDER, color);
			}

			inline HWND GetHwnd()
			{
				return (HWND)GetLong(ENGINE_LONG_HWND);
			}
		};
	}
}
