#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		static class ExWindow
		{
		public:
			HWND m_hWnd = 0;
			ExWindow(HWND hWnd)
			{
				m_hWnd = hWnd;
			}
			inline static WORD RegisterWndClass(std::wstring sWindowName, HICON hIcon = NULL, HICON hIconSm = NULL, HCURSOR hCursor = NULL)
			{
				return Ex_WndRegisterClass(sWindowName.c_str(), hIcon, hIconSm, hCursor);
			}

			inline static HWND Create(HWND hWndParent, std::wstring sWindowName, std::wstring lpwzWindowName, INT x, INT y, INT width, INT height, INT dwStyle = 0, INT dwStyleEx = 0)
			{
				return Ex_WndCreate(hWndParent, sWindowName.c_str(), lpwzWindowName.c_str(), x, y, width, height, dwStyle, dwStyleEx);
			}

			inline static void Center(HWND hWnd, HWND hWndFrom, BOOL bFullScreen)
			{
				Ex_WndCenterFrom(hWnd, hWndFrom, bFullScreen);
			}
		};
	}
}