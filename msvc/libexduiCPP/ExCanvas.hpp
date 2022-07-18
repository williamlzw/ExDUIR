#pragma once
#include "ExUIbase.hpp"
#include "ExBrush.hpp"
#include "ExPath.hpp"
#include "ExFont.hpp"
#include "ExRegion.hpp"
#include "ExImage.hpp"
#include "ExMatrix.hpp"
#include "ExSkin.hpp"
#include "ExControl.hpp"

using namespace ExDUIR::FrameWorks::Graphics;

namespace ExDUIR
{
	namespace FrameWorks
	{
		namespace Graphics
		{
			class ExCanvas
			{
			public:
				HEXCANVAS m_canvas = 0;

				ExCanvas() = default;
				ExCanvas(HEXCANVAS canvas)
				{
					m_canvas = canvas;
				}

				ExCanvas(ExSkin skin, INT width, INT height, INT dwFlags)
				{
					m_canvas = _canvas_createfromexdui(skin.m_handle, width, height, dwFlags);
				}

				ExCanvas(ExControl control, INT width, INT height, INT dwFlags)
				{
					m_canvas = _canvas_createfromobj(control.m_handle, width, height, dwFlags);
				}

				inline BOOL Destroy()
				{
					return _canvas_destroy(m_canvas);
				}

				inline BOOL DrawCanvas(ExCanvas canvas, INT dstLeft, INT dstTop, INT dstRight, INT dstBottom, INT srcLeft, INT srcTop, DWORD dwAlpha, DWORD dwCompositeMode)
				{
					return _canvas_drawcanvas(m_canvas, canvas.m_canvas, dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, dwAlpha, dwCompositeMode);
				}

				inline BOOL DrawEllipse(ExBrush brush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle)
				{
					return _canvas_drawellipse(m_canvas, brush.m_brush, x, y, radiusX, radiusY, strokeWidth, strokeStyle);
				}

				inline BOOL DrawImage(ExImage image, FLOAT left, FLOAT top, DWORD alpha)
				{
					return _canvas_drawimage(m_canvas, image.m_image, left, top, alpha);
				}

				inline BOOL DrawImageFromGrid(ExImage image, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
					FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, FLOAT gridPaddingLeft, FLOAT gridPaddingTop, FLOAT gridPaddingRight,
					FLOAT gridPaddingBottom, INT dwFlags, DWORD dwAlpha)
				{
					return _canvas_drawimagefromgrid(m_canvas, image.m_image, dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, srcRight, srcBottom,
						gridPaddingLeft, gridPaddingTop, gridPaddingRight, gridPaddingBottom, dwFlags, dwAlpha);
				}

				inline BOOL DrawImageRect(ExImage image, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, DWORD alpha)
				{
					return _canvas_drawimagerect(m_canvas, image.m_image, left, top, right, bottom, alpha);
				}

				inline BOOL DrawImageRectRect(ExImage image, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
					FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, DWORD alpha)
				{
					return _canvas_drawimagerectrect(m_canvas, image.m_image, dstLeft, dstTop, dstRight, dstBottom, srcLeft, srcTop, srcRight, srcBottom, alpha);
				}

				inline BOOL DrawLine(ExBrush brush, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT strokeWidth, DWORD strokeStyle)
				{
					return _canvas_drawline(m_canvas, brush.m_brush, x1, y1, x2, y2, strokeWidth, strokeStyle);
				}

				inline BOOL DrawPath(ExPath path, ExBrush brush, FLOAT strokeWidth, DWORD strokeStyle)
				{
					return _canvas_drawpath(m_canvas, path.m_path, brush.m_brush, strokeWidth, strokeStyle);
				}

				inline BOOL DrawRect(ExBrush brush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT strokeWidth, DWORD strokeStyle)
				{
					return _canvas_drawrect(m_canvas, brush.m_brush, left, top, right, bottom, strokeWidth, strokeStyle);
				}

				inline BOOL DrawRoundedRect(ExBrush brush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle)
				{
					return _canvas_drawroundedrect(m_canvas, brush.m_brush, left, top, right, bottom, radiusX, radiusY, strokeWidth, strokeStyle);
				}

				inline BOOL DrawTextFromColor(ExFont font, EXARGB crText, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
				{
					return _canvas_drawtext(m_canvas, font.m_font, crText, lpwzText, dwLen, dwDTFormat, left, top, right, bottom);
				}

				inline BOOL DrawTextFromColorEx(ExFont font, EXARGB crText, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, EXARGB crShadom, LPARAM lParam)
				{
					return _canvas_drawtextex(m_canvas, font.m_font, crText, lpwzText, dwLen, dwDTFormat, left, top, right, bottom, iGlowsize, crShadom, lParam);
				}

				inline BOOL DrawTextFromBrush(ExFont font, ExBrush brush, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
				{
					return _canvas_drawtext2(m_canvas, font.m_font, brush.m_brush, lpwzText, dwLen, dwDTFormat, left, top, right, bottom);
				}

				inline BOOL BeginDraw()
				{
					return _canvas_begindraw(m_canvas);
				}

				inline BOOL EndDraw()
				{
					return _canvas_enddraw(m_canvas);
				}

				inline BOOL FillEllipse(ExBrush brush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY)
				{
					return _canvas_fillellipse(m_canvas, brush.m_brush, x, y, radiusX, radiusY);
				}

				inline BOOL FillPath(ExPath path, ExBrush brush)
				{
					return _canvas_fillpath(m_canvas, path.m_path, brush.m_brush);
				}

				inline BOOL FillRect(ExBrush brush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom)
				{
					return _canvas_fillrect(m_canvas, brush.m_brush, left, top, right, bottom);
				}

				inline BOOL FillRegion(ExRegion region, ExBrush brush)
				{
					return _canvas_fillregion(m_canvas, region.m_region, brush.m_brush);
				}

				inline BOOL FillRoundedImage(ExImage image, FLOAT left, FLOAT top, FLOAT width, FLOAT height, FLOAT radiusX, FLOAT radiusY, INT* shadowNum, INT number, EXARGB crShadow)
				{
					return _canvas_fillroundedimage(m_canvas, image.m_image, left, top, width, height, radiusX, radiusY, shadowNum, number, crShadow);
				}

				inline BOOL FillRoundedRect(ExBrush brush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY)
				{
					return _canvas_fillroundedrect(m_canvas, brush.m_brush, left, top, right, bottom, radiusX, radiusY);
				}

				inline BOOL Flush()
				{
					return _canvas_flush(m_canvas);
				}

				inline LPVOID GetContext(INT nType)
				{
					return _canvas_getcontext(m_canvas, nType);
				}

				inline HDC GetDC()
				{
					return _canvas_getdc(m_canvas);
				}

				inline BOOL GetSize(INT* width, INT* height)
				{
					return _canvas_getsize(m_canvas, width, height);
				}

				inline BOOL ReleaseDC()
				{
					return _canvas_releasedc(m_canvas);
				}

				inline BOOL Clear(EXARGB color)
				{
					return _canvas_clear(m_canvas, color);
				}

				inline BOOL CalcTextSize(ExFont font, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight)
				{
					return _canvas_calctextsize(m_canvas, font.m_font, lpwzText, dwLen, dwDTFormat, lParam, layoutWidth, layoutHeight, lpWidth, lpHeight);
				}

				inline BOOL Blur(FLOAT fDeviation, RECT* lprc)
				{
					return _canvas_blur(m_canvas, fDeviation, lprc);
				}

				inline BOOL ClipRect(INT left, INT top, INT right, INT bottom)
				{
					return _canvas_cliprect(m_canvas, left, top, right, bottom);
				}

				inline BOOL ResetClip()
				{
					return _canvas_resetclip(m_canvas);
				}

				inline BOOL ReSize(INT width, INT height)
				{
					return _canvas_resize(m_canvas, width, height);
				}

				inline BOOL RotateHue(FLOAT fAngle)
				{
					return _canvas_rotate_hue(m_canvas, fAngle);
				}

				inline BOOL SetAntialias(BOOL antialias)
				{
					return _canvas_setantialias(m_canvas, antialias);
				}

				inline BOOL SetImageAntialias(BOOL antialias)
				{
					return _canvas_setimageantialias(m_canvas, antialias);
				}

				inline BOOL SetTextAntialias(BOOL antialias)
				{
					return _canvas_settextantialiasmode(m_canvas, antialias);
				}

				inline BOOL SetTransform(ExMatrix matrix)
				{
					return _canvas_settransform(m_canvas, matrix.m_matrix);
				}
			};
		}
	}
}