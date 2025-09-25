#pragma once
#include "ExUIbase.hpp"

using namespace ExDUIR::FrameWorks::Graphics;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExSvg
			{
			public:
				HEXSVG m_svg = 0;

				ExSvg(HEXSVG svg)
				{
					m_svg = svg;
				}

				ExSvg(const char* data)
				{
					HEXSVG svg = -1;
					auto ret = _svg_create(data, &svg);
					if (ret)
					{
						m_svg = svg;
					}
				}

				ExSvg(const wchar_t* file)
				{
					HEXSVG svg = -1;
					auto ret = _svg_createfromfile(file, &svg);
					if (ret)
					{
						m_svg = svg;
					}
				}

				void SetElementFillColor(LPCWSTR id, EXARGB color)
				{
					_svg_setelementfillcolor(m_svg, id, color);
				}

				void SetElementStrokeColor(LPCWSTR id, EXARGB color, float strokeWidth = 0.0f)
				{
					_svg_setelementstrokecolor(m_svg, id, color, strokeWidth);
				}

				void SetFillColor(EXARGB color)
				{
					_svg_setfillcolor(m_svg, color);
				}

				void SetStrokeColor(EXARGB color, FLOAT width = 0)
				{
					_svg_setstrokecolor(m_svg, color, width);
				}

				inline BOOL Destroy()
				{
					return _svg_destroy(m_svg);
				}
			};
		}
	}
}