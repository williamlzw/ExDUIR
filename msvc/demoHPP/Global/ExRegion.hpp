#pragma once
#include "ExUIbase.hpp"
#include "ExPath.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExRegion
			{
			public:
				HEXRGN m_region = nullptr;
				ExRegion() = default;
				ExRegion(HEXRGN region)
				{
					m_region = region;
				}

				ExRegion(ExPath path)
				{
					m_region = _rgn_createfrompath(path.m_path);
				}

				ExRegion(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
				{
					m_region = _rgn_createfromrect(left, top, right, bottom);
				}

				ExRegion(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY)
				{
					m_region = _rgn_createfromroundrect(left, top, right, bottom, radiusX, radiusY);
				}

				inline BOOL Destroy()
				{
					return _rgn_destroy(m_region);
				}
				
				inline BOOL Hittest(FLOAT x, FLOAT y)
				{
					return _rgn_hittest(m_region, x, y);
				}

				inline BOOL Hittest2(ExRegion rgn2, INT& retRelation)
				{
					return _rgn_hittest2(m_region, rgn2.m_region, &retRelation);
				}

				inline ExRegion Combine(ExRegion RgnDst, INT nCombineMode, INT dstOffsetX, INT dstOffsetY)
				{
					return ExRegion(_rgn_combine(m_region, RgnDst.m_region, nCombineMode, dstOffsetX, dstOffsetY));
				}

				inline BOOL GetLines(EX_POINTF** points, INT& pointsCount)
				{
					return _rgn_getlines(m_region, points, &pointsCount);
				}
			};
		}
	}
}