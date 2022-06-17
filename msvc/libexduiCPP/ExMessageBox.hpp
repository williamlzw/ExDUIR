#pragma once
#include "ExUIbase.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		class ExMessageBox
		{
		public:
			inline static INT Show(ExUIbase* pOwner, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, INT dwFlags)
			{
				return Ex_MessageBox(pOwner->m_handle, lpText, lpCaption, uType, dwFlags);
			}

			inline static INT ShowEx(ExUIbase* pOwner, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, LPCWSTR lpCheckBox, BOOL* lpCheckBoxChecked, INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc = NULL)
			{
				return Ex_MessageBoxEx(pOwner->m_handle, lpText, lpCaption, uType, lpCheckBox, lpCheckBoxChecked, dwMilliseconds, dwFlags, lpfnMsgProc);
			}
		};
	}
}