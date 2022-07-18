#pragma once
#include "ExUIbase.hpp"
#include "ExImage.hpp"
#include "ExMatrix.hpp"

using namespace ExDUIR::FrameWorks::Graphics;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExBrush
			{
			public:
				HEXBRUSH m_brush = nullptr;
				EXARGB m_color = 0;

				ExBrush(HEXBRUSH brush)
				{
					m_brush = brush;
				}

				ExBrush(EXARGB Color)
				{
					m_brush = _brush_create(Color);
					m_color = Color;
				}

			/*	ExBrush(ExCanvas* Canvas)
				{
					m_brush = _brush_createfromcanvas(Canvas->m_canvas);
				}

				ExBrush(ExCanvas* Canvas, DWORD alpha)
				{
					m_brush = _brush_createfromcanvas2(Canvas->m_canvas, alpha);
				}*/

				ExBrush(ExImage Image)
				{
					m_brush = _brush_createfromimg(Image.m_image);
				}

				ExBrush(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, EXARGB crBegin, EXARGB crEnd)
				{
					m_brush = _brush_createlinear(xStart, yStart, xEnd, yEnd, crBegin, crEnd);
				}

				ExBrush(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, FLOAT* arrStopPts, INT cStopPts)
				{
					m_brush = _brush_createlinear_ex(xStart, yStart, xEnd, yEnd, arrStopPts, cStopPts);
				}

				inline BOOL SetColor(EXARGB Color)
				{
					return _brush_setcolor(m_brush, Color);
				}

				inline EXARGB GetColor()
				{
					return m_color;
				}

				inline void TransFrom(ExMatrix matrix)
				{
					_brush_settransform(m_brush, matrix.m_matrix);
				}

				inline BOOL Destroy()
				{
					return _brush_destroy(m_brush);
				}
			};
		}
	}
}