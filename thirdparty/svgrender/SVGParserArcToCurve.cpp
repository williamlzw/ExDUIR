/*
Copyright 2016 Adobe. All rights reserved.
This file is licensed to you under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License. You may obtain a copy
of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
OF ANY KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
*/
#include "stdafx.h"

#include "SVGDocument.h"
#include "SVGRenderer.h"

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327f
#endif

namespace SVGNative
{
	static float __sqr(float x) { return x * x; }
	static float __vmag(float x, float y) { return sqrtf(x * x + y * y); }
	static float __vecrat(float ux, float uy, float vx, float vy)
	{
		return (ux * vx + uy * vy) / (__vmag(ux, uy) * __vmag(vx, vy));
	}
	static float __vecang(float ux, float uy, float vx, float vy)
	{
		float r = __vecrat(ux, uy, vx, vy);
		if (r < -1.0f) r = -1.0f;
		if (r > 1.0f) r = 1.0f;
		return ((ux * vy < uy * vx) ? -1.0f : 1.0f) * acosf(r);
	}
	static void __xformPoint(float* dx, float* dy, float x, float y, float* t)
	{
		*dx = x * t[0] + y * t[2] + t[4];
		*dy = x * t[1] + y * t[3] + t[5];
	}
	static void __xformVec(float* dx, float* dy, float x, float y, float* t)
	{
		*dx = x * t[0] + y * t[2];
		*dy = x * t[1] + y * t[3];
	}
	// 计算椭圆弧的控制点
    void ArcToCurve(SVGNative::D2DSVGPath& path, float startX, float startY, float radiusX, float radiusY, float angle, bool large,
        bool sweep, float endX, float endY, float& endControlX, float& endControlY)
    {
		// Ported from canvg (https://code.google.com/p/canvg/)
		float rx = radiusX, ry = radiusY, rotx;
		float x1, y1, x2, y2, cx, cy, dx, dy, d;
		float x1p, y1p, cxp, cyp, s, sa, sb;
		float ux, uy, vx, vy, a1, da;
		float x, y, tanx, tany, a, px = 0, py = 0, ptanx = 0, ptany = 0, t[6];
		float sinrx, cosrx;
		int fa, fs;
		int i, ndivs;
		float hda, kappa;

		rotx = angle / 180.0f * M_PI;	// x rotation angle
		fa = large ? 1 : 0;	                // Large arc
		fs = sweep ? 1 : 0;	                // Sweep direction
		x1 = startX;							// start point
		y1 = startY;
		x2 = endX;
		y2 = endY;
		dx = x1 - x2;
		dy = y1 - y2;
		d = sqrtf(dx * dx + dy * dy);
		if (d < 1e-6f || rx < 1e-6f || ry < 1e-6f) {
			// The arc degenerates to a line //椭圆圆弧退化为直线
			//nsvg__lineTo(p, x2, y2);
			//*cpx = x2;
			//*cpy = y2;
			//return;
		}

		sinrx = sinf(rotx);
		cosrx = cosf(rotx);

		// Convert to center point parameterization.
		// http://www.w3.org/TR/SVG11/implnote.html#ArcImplementationNotes
		// 1) Compute x1', y1'
		x1p = cosrx * dx / 2.0f + sinrx * dy / 2.0f;
		y1p = -sinrx * dx / 2.0f + cosrx * dy / 2.0f;
		d = __sqr(x1p) / __sqr(rx) + __sqr(y1p) / __sqr(ry);
		if (d > 1) {
			d = sqrtf(d);
			rx *= d;
			ry *= d;
		}
		// 2) Compute cx', cy'
		s = 0.0f;
		sa = __sqr(rx) * __sqr(ry) - __sqr(rx) * __sqr(y1p) - __sqr(ry) * __sqr(x1p);
		sb = __sqr(rx) * __sqr(y1p) + __sqr(ry) * __sqr(x1p);
		if (sa < 0.0f) sa = 0.0f;
		if (sb > 0.0f)
			s = sqrtf(sa / sb);
		if (fa == fs)
			s = -s;
		cxp = s * rx * y1p / ry;
		cyp = s * -ry * x1p / rx;

		// 3) Compute cx,cy from cx',cy'
		cx = (x1 + x2) / 2.0f + cosrx * cxp - sinrx * cyp;
		cy = (y1 + y2) / 2.0f + sinrx * cxp + cosrx * cyp;

		// 4) Calculate theta1, and delta theta.
		ux = (x1p - cxp) / rx;
		uy = (y1p - cyp) / ry;
		vx = (-x1p - cxp) / rx;
		vy = (-y1p - cyp) / ry;
		a1 = __vecang(1.0f, 0.0f, ux, uy);	// Initial angle
		da = __vecang(ux, uy, vx, vy);		// Delta angle

		//	if (vecrat(ux,uy,vx,vy) <= -1.0f) da = NSVG_PI;
		//	if (vecrat(ux,uy,vx,vy) >= 1.0f) da = 0;

		if (fs == 0 && da > 0)
			da -= 2 * M_PI;
		else if (fs == 1 && da < 0)
			da += 2 * M_PI;

		// Approximate the arc using cubic spline segments.
		t[0] = cosrx; t[1] = sinrx;
		t[2] = -sinrx; t[3] = cosrx;
		t[4] = cx; t[5] = cy;

		// Split arc into max 90 degree segments.
		// The loop assumes an iteration per end point (including start and end), this +1.
		ndivs = (int)(fabsf(da) / (M_PI * 0.5f) + 1.0f);
		hda = (da / (float)ndivs) / 2.0f;
		// Fix for ticket #179: division by 0: avoid cotangens around 0 (infinite)
		if ((hda < 1e-3f) && (hda > -1e-3f))
			hda *= 0.5f;
		else
			hda = (1.0f - cosf(hda)) / sinf(hda);
		kappa = fabsf(4.0f / 3.0f * hda);
		if (da < 0.0f)
			kappa = -kappa;

		for (i = 0; i <= ndivs; i++) {
			a = a1 + da * ((float)i / (float)ndivs);
			dx = cosf(a);
			dy = sinf(a);
			__xformPoint(&x, &y, dx * rx, dy * ry, t); // position
			__xformVec(&tanx, &tany, -dy * rx * kappa, dx * ry * kappa, t); // tangent
			if (i > 0)
			{
				//nsvg__cubicBezTo(p, px + ptanx, py + ptany, x - tanx, y - tany, x, y);
				D2D1_BEZIER_SEGMENT bs = {};
				bs.point1.x = px + ptanx;
				bs.point1.y = py + ptany;
				bs.point2.x = x - tanx;
				bs.point2.y = y - tany;
				bs.point3.x = x;
				bs.point3.y = y;
				//output(control1X, control1Y, control2X, control2Y, endX, endY,L"\r\n");
				// 添加贝塞尔曲线到路径
				path.mSink->AddBezier(bs);
			}
			px = x;
			py = y;
			ptanx = tanx;
			ptany = tany;
		}

		endControlX = x2;
		endControlY = y2;
       
    }

}
