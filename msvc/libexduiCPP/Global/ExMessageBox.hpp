#pragma once
#include "ExUIbase.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		class ExMessageBox
		{
		public:
			inline static INT Show(ExUIbase pOwner, std::wstring lpText, std::wstring lpCaption, INT uType, INT dwFlags)
			{
				return Ex_MessageBox(pOwner.m_handle, lpText.c_str(), lpCaption.c_str(), uType, dwFlags);
			}

			inline static INT ShowEx(ExUIbase pOwner, std::wstring lpText, std::wstring lpCaption, INT uType, std::wstring lpCheckBox, BOOL* lpCheckBoxChecked, INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc = NULL)
			{
				return Ex_MessageBoxEx(pOwner.m_handle, lpText.c_str(), lpCaption.c_str(), uType, lpCheckBox.c_str(), lpCheckBoxChecked, dwMilliseconds, dwFlags, lpfnMsgProc);
			}
		};
	}
}