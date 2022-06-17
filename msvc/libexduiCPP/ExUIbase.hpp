#pragma once
#include "ExDUIR_Func.h"

namespace ExDUIR
{
	namespace FrameWorks
	{
		class ExUIbase
		{
		public:
			EXHANDLE m_handle = 0;
			ExUIbase() = default;
			virtual ~ExUIbase() {};
		};
	}
}