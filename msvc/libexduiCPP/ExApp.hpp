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
			ExApp(std::vector<CHAR> theme, DWORD dwGlobalFlags = 0)
			{
				if (!Ex_Init(GetModuleHandleW(NULL), dwGlobalFlags, 0, 0, theme.data(), theme.size(), 0, 0))
				{
					Ex_MessageBox(0, L"引擎初始化失败", L"引擎初始化失败", MB_ICONWARNING, EMBF_CENTEWINDOW);
				}
			}

			inline void UnInit(){ Ex_UnInit(); }
			inline WPARAM Run() { return Ex_WndMsgLoop(); }
			inline INT GetLastError() { return Ex_GetLastError(); }
			inline void SetLastError(INT error) { return Ex_SetLastError(error); }
		};
	}
}
