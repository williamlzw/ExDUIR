#pragma once
#include "ExUIbase.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		class ExApp
		{
		public:
			ExApp() = default;
			ExApp(std::vector<CHAR> themeData, DWORD dwGlobalFlags = 0, HCURSOR hDefaultCursor = NULL)
			{
				if (!Ex_Init(GetModuleHandleW(NULL), dwGlobalFlags, hDefaultCursor, 0, themeData.data(), themeData.size(), 0, 0))
				{
					Ex_MessageBox(0, L"engine init failed", L"engine init failed", MB_ICONWARNING, MESSAGEBOX_FLAG_CENTEWINDOW);
				}
			}

			ExApp(std::vector<CHAR> themeData, DWORD dwGlobalFlags = 0, std::wstring cursorFilePath = L"")
			{
				HCURSOR hCursor = NULL;
				if (cursorFilePath != L"")
				{
					std::vector<CHAR> data;
					Ex_ReadFile(cursorFilePath.c_str(), &data);
					hCursor = (HCURSOR)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_CURSOR, 1);
				}
				ExApp(themeData, dwGlobalFlags, hCursor);
			}

			ExApp(std::wstring themeFilePath, DWORD dwGlobalFlags = 0, std::wstring cursorFilePath = L"")
			{
				std::vector<CHAR> theme;
				Ex_ReadFile(themeFilePath.c_str(), &theme);
				HCURSOR hCursor = NULL;
				if (cursorFilePath != L"")
				{
					std::vector<CHAR> data;
					Ex_ReadFile(cursorFilePath.c_str(), &data);
					hCursor = (HCURSOR)Ex_LoadImageFromMemory(data.data(), data.size(), IMAGE_CURSOR, 1);
				}
				ExApp(theme, dwGlobalFlags, hCursor);
			}

			inline void UnInit(){ Ex_UnInit(); }
			inline WPARAM Run() { return Ex_WndMsgLoop(); }
			inline INT GetLastError() { return Ex_GetLastError(); }
			inline void SetLastError(INT error) { return Ex_SetLastError(error); }
		};
	}
}
