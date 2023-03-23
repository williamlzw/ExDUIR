#pragma once
#include "ExUIbase.hpp"
#include "ExControl.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		static class ExWindow
		{
		private:

		public:
			HWND m_hWnd = 0;

			static WORD RegisterWndClass(LPCWSTR sWindowName, HICON hIcon = NULL, HICON hIconSm = NULL, HCURSOR hCursor = NULL)
			{
				return Ex_WndRegisterClass(sWindowName, hIcon, hIconSm, hCursor);
			}

			static HWND Create(HWND hWndParent, LPCWSTR sWindowName, LPCWSTR lpwzWindowName, INT x, INT y, INT width, INT height, INT dwStyle = 0, INT dwStyleEx = 0)
			{
				return Ex_WndCreate(hWndParent, sWindowName, lpwzWindowName, x, y, width, height, dwStyle, dwStyleEx);
			}

			static void Center(HWND hWnd, HWND hWndFrom, BOOL bFullScreen)
			{
				Ex_WndCenterFrom(hWnd, hWndFrom, bFullScreen);
			}
		};
	}
}