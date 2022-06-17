#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"

using namespace ExDUIR::FrameWorks::Control;

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
			ExSkin(HWND pOwner, INT x, INT y, INT width, INT height, LPCWSTR sWindowName, int dwStyleDui, DWORD dwStyleWindow = NULL, DWORD dwStyleExWindow = NULL, WinMsgPROC lpfnMsgProc = NULL)
			{
				m_hWnd = Ex_WndCreate((HWND)0, 0, sWindowName, x, y, width, height, dwStyleWindow, dwStyleExWindow);
				m_handle = Ex_DUIBindWindowEx(m_hWnd, 0, dwStyleDui, 0, lpfnMsgProc);
			}
			inline size_t SetLong(INT nIndex, size_t dwNewLong) { return Ex_DUISetLong(m_handle, nIndex, dwNewLong); }
			inline BOOL Show(INT nCmdShow = SW_SHOWNORMAL, INT dwTimer = NULL, INT dwFrames = NULL, INT dwFlags = NULL) { return Ex_DUIShowWindow(m_handle, nCmdShow, dwTimer, dwFrames, dwFlags); };
			inline BOOL IsValidate() { return IsWindow(m_hWnd) && m_handle != 0; }
			inline BOOL IsEnable() { return IsWindowEnabled(m_hWnd); }
			inline BOOL Enable(BOOL bEnable) { return EnableWindow(m_hWnd, bEnable); }
			inline BOOL IsVisible() { return IsWindowVisible(m_hWnd); }
			inline BOOL IsVisible(BOOL bVisible) { return Ex_DUIShowWindow(m_handle, bVisible ? SW_SHOWNORMAL : SW_HIDE, 0, 0, 0); }
			inline BOOL SetTitle(LPCWSTR title) { return SetWindowTextW(m_hWnd, title); }
			inline BOOL Move(INT x, INT y, INT width, INT height, BOOL bRepaint) { return MoveWindow(m_hWnd, x, y, width, height, bRepaint); }
			inline BOOL PostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) { PostMessageW(m_hWnd, uMsg, wParam, lParam); }
			inline std::wstring GetTitle()
			{
				int len = GetWindowTextLengthW(m_hWnd);
				std::wstring retTitle;
				retTitle.resize(len + 1);
				GetWindowTextW(m_hWnd, (LPWSTR)retTitle.c_str(), len + 1);
				return retTitle;
			}
			inline BOOL SetBackgroundImage(LPVOID lpImage, size_t dwImageLen, INT x, INT y, DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha, BOOL fUpdate)
			{
				return Ex_ObjSetBackgroundImage(m_handle, lpImage, dwImageLen, x, y, dwRepeat, lpGrid, dwFlags, dwAlpha, fUpdate);
			}

			inline ExControl GetFromID(INT nID)
			{
				auto hObj = Ex_ObjGetFromID(m_handle, nID);

				return ExControl(hObj);
			}

		};
	}
}
