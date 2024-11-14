#pragma once
#include "ExUIbase.hpp"


namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExEasing
			{
			public:
				HEXEASING m_easing = nullptr;
				ExEasing() = default;
				ExEasing(DWORD dwType, LPVOID pEasingContext, DWORD dwMode, LONG_PTR pContext, INT nTotalTime, INT nInterval, DWORD nState, INT nStart, INT nStop, LONG_PTR param1, LONG_PTR param2, LONG_PTR param3, LONG_PTR param4)
				{
					m_easing = _easing_create(dwType, pEasingContext, dwMode, pContext, nTotalTime, nInterval, nState, nStart, nStop, param1, param2, param3, param4);
				}

				inline DWORD GetState()
				{
					return _easing_getstate(m_easing);
				}

				inline BOOL SetState(DWORD nState)
				{
					return _easing_setstate(m_easing, nState);
				}
			};
		}
	}
}