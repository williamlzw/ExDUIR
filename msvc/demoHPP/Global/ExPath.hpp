#pragma once
#include "ExUIbase.hpp"

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExPath
			{
			public:
				HEXPATH m_path = 0;

				ExPath(HEXPATH path)
				{
					m_path = path;
				}

				ExPath(DWORD flag = PATH_FLAG_DISABLESCALE)
				{
					_path_create(flag, &m_path);
				}

				inline BOOL Close()
				{
					return _path_close(m_path);
				}

				inline BOOL Destroy()
				{
					return _path_destroy(m_path);
				}

				inline BOOL AddArc(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radiusX, FLOAT radiusY, BOOL fClockwise)
				{
					return _path_addarc(m_path, x1, y1, x2, y2, radiusX, radiusY, fClockwise);
				}

				inline BOOL AddArc2(FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT startAngle, FLOAT sweepAngle)
				{
					return _path_addarc2(m_path, x, y, width, height, startAngle, sweepAngle);
				}

				inline BOOL AddArc3(FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT startAngle, FLOAT sweepAngle, BOOL fClockwise, BOOL barcSize)
				{
					return _path_addarc3(m_path, x, y, radiusX, radiusY, startAngle, sweepAngle, fClockwise, barcSize);
				}

				inline BOOL AddBezier(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3)
				{
					return _path_addbezier(m_path, x1, y1, x2, y2, x3, y3);
				}

				inline BOOL AddQuadraticBezier(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3)
				{
					return _path_addquadraticbezier(m_path, x1, y1, x2, y2);
				}

				inline BOOL AddLine(FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2)
				{
					return _path_addline(m_path, x1, y1, x2, y2);
				}

				inline BOOL AddRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
				{
					return _path_addrect(m_path, left, top, right, bottom);
				}

				inline BOOL AddRoundedRect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusTopLeft, FLOAT radiusTopRight, FLOAT radiusBottomLeft, FLOAT radiusBottomRight)
				{
					return _path_addroundedrect(m_path, left, top, right, bottom, radiusTopLeft, radiusTopRight, radiusBottomLeft, radiusBottomRight);
				}

				inline BOOL BeginFigure()
				{
					return _path_beginfigure(m_path);
				}

				inline BOOL BeginFigure(FLOAT x, FLOAT y)
				{
					return _path_beginfigure2(m_path, x, y);
				}

				inline BOOL BeginFigure(FLOAT x, FLOAT y, INT figureBegin)
				{
					return _path_beginfigure3(m_path, x, y, figureBegin);
				}

				inline BOOL EndFigure(BOOL fCloseFigure)
				{
					return _path_endfigure(m_path, fCloseFigure);
				}

				inline BOOL GetBounds(EX_RECTF& rc)
				{
					return _path_getbounds(m_path, &rc);
				}

				inline BOOL Hittest(FLOAT x, FLOAT y)
				{
					return _path_hittest(m_path, x, y);
				}

				inline BOOL Open()
				{
					return _path_open(m_path);
				}

				inline BOOL Reset()
				{
					return _path_reset(m_path);
				}
			};
		}
	}
}