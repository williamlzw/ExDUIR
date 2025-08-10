/*
Copyright 2020 Adobe. All rights reserved.
This file is licensed to you under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License. You may obtain a copy
of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
OF ANY KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
*/
#include "stdafx.h"

#include <stdexcept>

#include <Windows.h>
#include <D2d1.h>
#include <Wincodec.h> // Windows Imaging Component (WIC)
#include <cctype>
#include "Config.h"
#include "SVGDocument.h"
#include "base64.h"
#include <memory>


namespace SVGNative
{

	/******************************************************************************/

	D2DSVGPath::D2DSVGPath(ID2D1Factory* inD2DFactory)
	{
		inD2DFactory->CreatePathGeometry(&mPath);
		SVG_ASSERT(mPath);
		if (!mSink)
		{
			mPath->Open(&mSink);
			//assert(SUCCEEDED(hr));
		}
	}

	D2DSVGPath::~D2DSVGPath()
	{
		SVG_ASSERT(mPath);
		ClosePathSink();
		mPath.Reset();
	}
	void D2DSVGPath::StopPath()
	{
		ClosePathSink();
	}
	void D2DSVGPath::ClosePath() //z
	{
		if (mHasOpenFigure)
		{
			SVG_ASSERT(mSink);
			mSink->EndFigure(D2D1_FIGURE_END_CLOSED);
			mHasOpenFigure = false;
		}
		//mSink->Close();
		//mSink.Reset();
	}
	
	void D2DSVGPath::AddArc(float x, float y, float rx, float ry)
	{
		SVG_ASSERT(mSink);
		mSink->AddArc(
			D2D1::ArcSegment(
				D2D1::Point2F(x, y),
				D2D1::SizeF(rx, ry),
				0,
				D2D1_SWEEP_DIRECTION_CLOCKWISE,
				D2D1_ARC_SIZE_SMALL
			)
		);
	}

	void D2DSVGPath::ClosePathSink() 
	{
		if (mHasOpenFigure)
		{
			SVG_ASSERT(mSink);
			mSink->EndFigure(D2D1_FIGURE_END_OPEN);
			mHasOpenFigure = false;
		}
		if (mSink)
		{
			mSink->Close();
                  mSink.Reset();
		}
	}

	void D2DSVGPath::Rect(float x, float y, float w, float h)
	{
		MoveTo(x, y);
		LineTo(x + w, y);
		LineTo(x + w, y + h);
		LineTo(x, y + h);
		ClosePath();
	}

	void D2DSVGPath::RoundedRect(float x, float y, float w, float h, float rx, float ry)
	{
		MoveTo(x + rx, y);
		LineTo(x + w - rx, y);
		AddArc(x + w, y + ry, rx, ry);
		LineTo(x + w, y + h - ry);
		AddArc(x + w - rx, y + h, rx, ry);
		LineTo(x + rx, y + h);
		AddArc(x, y + h - ry, rx, ry);
		LineTo(x, y + ry);
		AddArc(x + rx, y, rx, ry);
		ClosePath();
	}

	void D2DSVGPath::Ellipse(float cx, float cy, float rx, float ry)
	{
		MoveTo(cx, cy - ry);
		AddArc(cx, cy + ry, rx, ry);
		AddArc(cx, cy - ry, rx, ry);
		ClosePath();
	}

	void D2DSVGPath::MoveTo(float x, float y)
	{
		if (mHasOpenFigure)
		{
			SVG_ASSERT(mSink);
			mSink->EndFigure(D2D1_FIGURE_END_OPEN);
			mHasOpenFigure = false;
		}
		//if (!mSink)
		//{
		//	auto hr = mPath->Open(&mSink);
		//	assert(SUCCEEDED(hr));
		//}
		SVG_ASSERT(mSink);
		mSink->BeginFigure(
			D2D1::Point2F(x, y),
			D2D1_FIGURE_BEGIN_FILLED
		);
		mHasOpenFigure = true;
		mCurrentX = x;
		mCurrentY = y;
	}

	void D2DSVGPath::LineTo(float x, float y)
	{
		if (!mHasOpenFigure)
			MoveTo(x, y);
		mSink->AddLine(D2D1::Point2F(x, y));
		mCurrentX = x;
		mCurrentY = y;
	}

	void D2DSVGPath::CurveTo(float x1, float y1, float x2, float y2, float x3, float y3)
	{
		SVG_ASSERT(mSink);
		if (!mHasOpenFigure)
			MoveTo(x1, y1);
		mSink->AddBezier(
			D2D1::BezierSegment(
				D2D1::Point2F(x1, y1),
				D2D1::Point2F(x2, y2),
				D2D1::Point2F(x3, y3))
		);
		mCurrentX = x3;
		mCurrentY = y3;
	}

	void D2DSVGPath::CurveToV(float x2, float y2, float x3, float y3)
	{
		float cx1 = static_cast<float>(mCurrentX + 2.0 / 3.0 * (x2 - mCurrentX));
		float cy1 = static_cast<float>(mCurrentY + 2.0 / 3.0 * (y2 - mCurrentY));
		float cx2 = static_cast<float>(x3 + 2.0 / 3.0 * (x2 - x3));
		float cy2 = static_cast<float>(y3 + 2.0 / 3.0 * (y2 - y3));
		CurveTo(cx1, cy1, cx2, cy2, x3, y3);
	}

	
	ComPtr<ID2D1PathGeometry> D2DSVGPath::GetGraphicsPath()
	{
		ClosePathSink();
		return mPath;
	}

	/******************************************************************************/

	D2DSVGTransform::D2DSVGTransform(float a, float b, float c, float d, float tx, float ty)
	{
		mTransform = D2D1::Matrix3x2F{ a, b, c, d, tx, ty };
	}

	void D2DSVGTransform::Set(float a, float b, float c, float d, float tx, float ty)
	{
		mTransform = D2D1::Matrix3x2F{ a, b, c, d, tx, ty };
	}

	void D2DSVGTransform::Rotate(float r)
	{
		mTransform = D2D1::Matrix3x2F::Rotation(r) * mTransform;
	}

	void D2DSVGTransform::Translate(float tx, float ty)
	{
		mTransform = D2D1::Matrix3x2F::Translation(tx, ty) * mTransform;
	}

	void D2DSVGTransform::Scale(float sx, float sy)
	{
		mTransform = D2D1::Matrix3x2F::Scale(sx, sy) * mTransform;
	}

	void D2DSVGTransform::Concat(float a, float b, float c, float d, float tx, float ty)
	{
		mTransform = D2D1::Matrix3x2F{ a, b, c, d, tx, ty } *mTransform;
	}

	const D2D1::Matrix3x2F& D2DSVGTransform::GetMatrix() const
	{
		return mTransform;
	}

	D2DSVGImageData::D2DSVGImageData(ComPtr<IWICBitmapSource> bitmapSource)
		: mBitmapSource(bitmapSource)
	{
		if (mBitmapSource)
		{
			UINT width = 0;
			UINT height = 0;
			ThrowIfFailed(mBitmapSource->GetSize(&width, &height));
			mWidth = static_cast<float>(width);
			mHeight = static_cast<float>(height);
		}
	}

	D2DSVGImageData::~D2DSVGImageData()
	{
	}

	float D2DSVGImageData::Width() const
	{
		return mWidth;
	}

	float D2DSVGImageData::Height() const
	{
		return mHeight;
	}

	ComPtr<IWICBitmapSource> D2DSVGImageData::GetBitmapSource() const
	{
		return mBitmapSource;
	}


} // namespace SVGNative
