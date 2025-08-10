/*
Copyright 2014 Adobe. All rights reserved.
This file is licensed to you under the Apache License, Version 2.0 (the
"License"); you may not use this file except in compliance with the License. You
may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
REPRESENTATIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
#include "stdafx.h"

#include "Constants.h"
#include "SVGRenderer.h"
#include "SVGStringParser.h"
#include "xml/XMLParser.h"

#include <cmath>
#include <limits>
#include "base64.h"

using namespace SVGNative::xml;

namespace SVGNative
{
	constexpr std::array<const char*, 14> gInheritedPropertyNames{
		{kColorProp, kClipRuleProp, kFillProp, kFillRuleProp, kFillOpacityProp,
		 kStrokeProp, kStrokeDasharrayProp, kStrokeDashoffsetProp,
		 kStrokeLinecapProp, kStrokeLinejoinProp, kStrokeMiterlimitProp,
		 kStrokeOpacityProp, kStrokeWidthProp, kVisibilityProp} };

	constexpr std::array<const char*, 5> gNonInheritedPropertyNames{
		{kClipPathProp, kDisplayProp, kOpacityProp, kStopOpacityProp,
		 kStopColorProp} };

	template <typename T> bool isCloseToZero(T x) {
		return std::abs(x) < std::numeric_limits<T>::epsilon();
	}
	bool SVGDocument::OpenSVGDocument(const char* s)
	{

		try {
			if (!mContext || !g_Ri.pD2Dfactory || !g_Ri.pWICFactory)
				return false;
			auto xmlDocument = xml::XMLDocument::CreateXMLDocument(s);
			if (!xmlDocument)
				return false;
			auto rootNode = xmlDocument->GetFirstNode();
			if (!rootNode)
				return false;
			TraverseSVGTree(rootNode.get());
			return true;
		}
		catch (...) {
		}

		return false;
	}

	std::int32_t SVGDocument::Width() const {
		return static_cast<std::int32_t>(mViewBox[2]);
	}

	std::int32_t SVGDocument::Height() const {
		return static_cast<std::int32_t>(mViewBox[3]);
	}


	void SVGDocument::Render(float width, float height)
	{
		ColorMap colorMap;
		Render(colorMap, width, height);
	}

	SVGDocument::SVGDocument() : mViewBox{ {0, 0, 320.0f, 200.0f} }
	{
		mContext = nullptr;

		mFillStyleStack.push(FillStyleImpl());
		mStrokeStyleStack.push(StrokeStyleImpl());

		GraphicStyleImpl graphicStyle{};
		std::set<std::string> classNames;
		mGroup = std::make_shared<Group>(graphicStyle, classNames);
		mGroupStack.push(mGroup);
	}

	SVGDocument::SVGDocument(ID2D1DeviceContext* renderTarget)
		: mViewBox{ {0, 0, 320.0f, 200.0f} }
	{
		mContext = renderTarget;

		mFillStyleStack.push(FillStyleImpl());
		mStrokeStyleStack.push(StrokeStyleImpl());

		GraphicStyleImpl graphicStyle{};
		std::set<std::string> classNames;
		mGroup = std::make_shared<Group>(graphicStyle, classNames);
		mGroupStack.push(mGroup);
	}

	void SVGDocument::TraverseSVGTree(XMLNode* rootNode)
	{
		if (!rootNode || strcmp(rootNode->GetName(), kSvgElem))
			return;
		mRootNode = rootNode;

		auto viewBoxAttr = rootNode->GetAttribute(kViewBoxAttr);
		if (!viewBoxAttr.found)
		{
			mViewBox[0] = SVGDocument::ParseLengthFromAttr(
				rootNode, kXAttr, LengthType::kHorizontal, mViewBox[0]);
			mViewBox[1] = SVGDocument::ParseLengthFromAttr(
				rootNode, kYAttr, LengthType::kVertical, mViewBox[1]);
			mViewBox[2] = SVGDocument::ParseLengthFromAttr(
				rootNode, kWidthAttr, LengthType::kHorizontal, mViewBox[2]);
			mViewBox[3] = SVGDocument::ParseLengthFromAttr(
				rootNode, kHeightAttr, LengthType::kVertical, mViewBox[3]);
		}
		else
		{
			std::vector<float> numberList;
			if (SVGStringParser::ParseListOfNumbers(viewBoxAttr.value, numberList) && numberList.size() == 4)
				mViewBox = { {numberList[0], numberList[1], numberList[2], numberList[3]} };
		}
		auto widthAttr = rootNode->GetAttribute(kWidthAttr);
		if (widthAttr.found)
			width = atoi(widthAttr.value);
		else
			width = mViewBox[2];
		auto heightAttr = rootNode->GetAttribute(kHeightAttr);
		if (heightAttr.found)
			height = atoi(heightAttr.value);
		else
			height = mViewBox[3];
		//#if DEBUG
		//  auto dataNameAttr = rootNode->GetAttribute(kDataNameAttr);
		//  if (dataNameAttr.found)
		//    mTitle = dataNameAttr.value;
		//#endif

		ParseChild(rootNode);

		// Clear all temporary sets
		mGradients.clear();
		mClippingPaths.clear();
		mRootNode = nullptr;
	}

	float SVGDocument::RelativeLength(LengthType lengthType) const {
		float relLength{};
		switch (lengthType) {
		case LengthType::kHorizontal:
			relLength = mViewBox[2];
			break;
		case LengthType::kVertical:
			relLength = mViewBox[3];
			break;
		case LengthType::kDiagonal:
			relLength = sqrtf(mViewBox[2] * mViewBox[2] + mViewBox[3] * mViewBox[3]);
			break;
		default:
			break;
		}
		return relLength;
	}

	float SVGDocument::ParseLengthFromAttr(const XMLNode* node,
		const char* attrName,
		LengthType lengthType,
		float fallback) {
		if (!node)
			return fallback;

		float number{};
		auto attr = node->GetAttribute(attrName);
		if (!attr.found || !SVGStringParser::ParseLengthOrPercentage(
			attr.value, RelativeLength(lengthType), number, true))
			return fallback;

		return number;
	}

	void SVGDocument::ParseChildren(XMLNode* node) {
		SVG_ASSERT(node != nullptr);

		for (auto child = node->GetFirstNode(); child != nullptr;
			child = child->GetNextSibling()) {
			ParseChild(child.get());
		}
	}

	void SVGDocument::ParseChild(XMLNode* child) {
		SVG_ASSERT(child != nullptr);

		auto fillStyle = mFillStyleStack.top();
		auto strokeStyle = mStrokeStyleStack.top();
		std::set<std::string> classNames;
		auto graphicStyle = ParseGraphic(child, fillStyle, strokeStyle, classNames);

		std::string idString;
		auto idAttr = child->GetAttribute(kIdAttr);
		if (idAttr.found)
			idString = idAttr.value;

		// Check if we have a shape rect, circle, ellipse, line, polygon, polyline or path first.
		// 首先检查我们是否有一个形状——直、圆、椭圆、直线、多边形、折线或路径
		if (auto path = ParseShape(child, fillStyle))
		{
			AddChildToCurrentGroup(
				std::unique_ptr<Graphic>(new Graphic(graphicStyle, classNames, fillStyle, strokeStyle, std::move(path))), std::move(idString));

			return;
		}

		// Look at all elements that are no shapes.
		const auto elementName = child->GetName();
		if (!strcmp(elementName, kGElem) ||
			(!strcmp(elementName, kSvgElem) && child == mRootNode)) {
			mFillStyleStack.push(fillStyle);
			mStrokeStyleStack.push(strokeStyle);

			auto group = std::make_shared<Group>(graphicStyle, classNames);
			AddChildToCurrentGroup(group, std::move(idString));
			mGroupStack.push(group);

			ParseChildren(child);

			mGroupStack.pop();
			mFillStyleStack.pop();
			mStrokeStyleStack.pop();
		}
		else if (!strcmp(elementName, kDefsElem)) {
			mFillStyleStack.push(fillStyle);
			mStrokeStyleStack.push(strokeStyle);

			// Create dummmy group. All children w/o id will get cleaned up.
			mGroupStack.push(std::make_shared<Group>(graphicStyle, classNames));

			ParseChildren(child);

			mGroupStack.pop();
			mFillStyleStack.pop();
			mStrokeStyleStack.pop();
		}
		else if (!strcmp(elementName, kImageElem)) {
			std::unique_ptr<D2DSVGImageData> imageData;
			auto hrefAttr = child->GetAttribute(kHrefAttr, kXlinkNS);
			if (hrefAttr.found) {
				const std::string dataURL = hrefAttr.value;
				ImageEncoding encoding{};
				unsigned short base64Offset{ 22 };
				if (dataURL.find(kDataUrlPngVal) == 0)
					encoding = ImageEncoding::kPNG;
				else if (dataURL.find(kDataUrlJpgVal) == 0)
					encoding = ImageEncoding::kJPEG;
				else if (dataURL.find(kDataUrlJpegVal) == 0) {
					encoding = ImageEncoding::kJPEG;
					base64Offset = 23;
				}
				else
					return;
				imageData =
					CreateImageData(dataURL.substr(base64Offset), encoding);
			}

			if (imageData) {
				const float imageWidth =
					ParseLengthFromAttr(child, kWidthAttr, LengthType::kHorizontal);
				const float imageHeight =
					ParseLengthFromAttr(child, kHeightAttr, LengthType::kVertical);

				Rect clipArea{ ParseLengthFromAttr(child, kXAttr, LengthType::kHorizontal),
							  ParseLengthFromAttr(child, kYAttr, LengthType::kVertical),
							  ParseLengthFromAttr(child, kWidthAttr,
												  LengthType::kHorizontal, imageWidth),
							  ParseLengthFromAttr(child, kHeightAttr,
												  LengthType::kVertical, imageHeight) };

				std::string align;
				std::string meetOrSlice;
				std::vector<std::string> attrStringValues;
				auto preserveAspectRatioAttr =
					child->GetAttribute(kPreserveAspectRatioAttr);
				if (preserveAspectRatioAttr.found &&
					SVGStringParser::ParseListOfStrings(preserveAspectRatioAttr.value,
						attrStringValues) &&
					attrStringValues.size() >= 1 && attrStringValues.size() <= 2) {
					align = attrStringValues[0];
					if (attrStringValues.size() == 2)
						meetOrSlice = attrStringValues[1];
				}

				Rect fillArea{ clipArea };
				if (align.compare(kNoneVal) != 0) {
					fillArea.width = imageWidth;
					fillArea.height = imageHeight;
					float scaleX = clipArea.width / imageWidth;
					float scaleY = clipArea.height / imageHeight;
					float scale{};
					if (meetOrSlice.compare(kSliceVal) == 0)
						scale = max(scaleX, scaleY);
					else
						scale = min(scaleX, scaleY);
					fillArea.width *= scale;
					fillArea.height *= scale;

					if (align.compare(kXMinYMinVal) == 0) {
						fillArea.x = clipArea.x;
						fillArea.y = clipArea.y;
					}
					else if (align.compare(kXMidYMinVal) == 0) {
						fillArea.x = (clipArea.x + clipArea.width / 2) - fillArea.width / 2;
						fillArea.y = clipArea.y;
					}
					else if (align.compare(kXMaxYMinVal) == 0) {
						fillArea.x = clipArea.x + clipArea.width - fillArea.width;
						fillArea.y = clipArea.y;
					}
					else if (align.compare(kXMinYMidVal) == 0) {
						fillArea.x = clipArea.x;
						fillArea.y = (clipArea.y + clipArea.height / 2) - fillArea.height / 2;
					}
					else if (align.compare(kXMaxYMidVal) == 0) {
						fillArea.x = clipArea.x + clipArea.width - fillArea.width;
						fillArea.y = (clipArea.y + clipArea.height / 2) - fillArea.height / 2;
					}
					else if (align.compare(kXMinYMaxVal) == 0) {
						fillArea.x = clipArea.x;
						fillArea.y = clipArea.y + clipArea.height - fillArea.height;
					}
					else if (align.compare(kXMidYMaxVal) == 0) {
						fillArea.x = (clipArea.x + clipArea.width / 2) - fillArea.width / 2;
						fillArea.y = clipArea.y + clipArea.height - fillArea.height;
					}
					else if (align.compare(kXMaxYMaxVal) == 0) {
						fillArea.x = clipArea.x + clipArea.width - fillArea.width;
						fillArea.y = clipArea.y + clipArea.height - fillArea.height;
					}
					else // default and "xMidYMid"
					{
						fillArea.x = (clipArea.x + clipArea.width / 2) - fillArea.width / 2;
						fillArea.y = (clipArea.y + clipArea.height / 2) - fillArea.height / 2;
					}
				}

				// Do not render 0-sized elements.
				if (imageWidth && imageHeight && clipArea.width && clipArea.height &&
					fillArea.width && fillArea.height) {
					auto image = std::unique_ptr<Image>(new Image(graphicStyle, classNames,
						std::move(imageData),
						clipArea, fillArea));
					AddChildToCurrentGroup(std::move(image), std::move(idString));
				}
			}
		}
		else if (!strcmp(elementName, kUseElem)) {
			auto hrefAttr = child->GetAttribute(kHrefAttr, kXlinkNS);
			if (!hrefAttr.found || !hrefAttr.value || hrefAttr.value[0] != '#')
				return;

			const float x = ParseLengthFromAttr(child, kXAttr, LengthType::kHorizontal);
			const float y = ParseLengthFromAttr(child, kYAttr, LengthType::kVertical);
			if (!isCloseToZero(x) || !isCloseToZero(y)) {
				if (!graphicStyle.transform)
					graphicStyle.transform = CreateTransform();
				graphicStyle.transform->Concat(1, 0, 0, 1, x, y);
			}

			std::string href{ (hrefAttr.value + 1) };
			AddChildToCurrentGroup(std::make_shared<Reference>(graphicStyle, classNames,
				fillStyle, strokeStyle,
				std::move(href)),
				std::move(idString));
		}
		else if (!strcmp(elementName, kSymbolElem)) {
			// FIXME: Do not render <symbol> outside of <defs> section.
			// FIXME: Remove support for symbol ASAP.
			auto attr = child->GetAttribute(kViewBoxAttr);
			if (attr.found) {
				std::vector<float> numberList;
				if (SVGStringParser::ParseListOfNumbers(attr.value, numberList) &&
					numberList.size() == 4)
					graphicStyle.transform = CreateTransform(
						1, 0, 0, 1, -numberList[0], -numberList[1]);
			}

			auto group = std::make_shared<Group>(graphicStyle, classNames);
			AddChildToCurrentGroup(group, std::move(idString));
			mGroupStack.push(group);

			ParseChildren(child);

			mGroupStack.pop();
		}
		else if (!strcmp(elementName, kLinearGradientElem) ||
			!strcmp(elementName, kRadialGradientElem)) {
			mFillStyleStack.push(fillStyle);
			mStrokeStyleStack.push(strokeStyle);

			ParseGradient(child);

			mFillStyleStack.pop();
			mStrokeStyleStack.pop();
		}
#ifdef STYLE_SUPPORT
		else if (!strcmp(elementName, kStyleElem))
			ParseStyle(child);
#endif
		else if (!strcmp(elementName, kClipPathElem)) {
			auto id = child->GetAttribute(kIdAttr);
			if (!id.found)
				return;

			mFillStyleStack.push(fillStyle);
			mStrokeStyleStack.push(strokeStyle);

			// SVG only allows shapes (and <use> elements referencing shapes) as
			// children of <clipPath>. Ignore all other elements.
			bool hasClipContent{ false };
			for (auto clipPathChild = child->GetFirstNode(); clipPathChild != nullptr;
				clipPathChild = clipPathChild->GetNextSibling()) 
			{
				// WebKit and Blink allow the clipping path if there is at least one valid basic shape child.
				// 
				auto fillStyleChild = mFillStyleStack.top();
				if (auto path = ParseShape(clipPathChild.get(), fillStyleChild)) 
				{
					std::unique_ptr<Transform> transform;
					auto attr = clipPathChild->GetAttribute(kTransformAttr);
					if (attr.found) {

						transform = CreateTransform();
						if (!SVGStringParser::ParseTransform(attr.value, *transform))
							transform.reset();
					}
					auto strokeStyleChild = mStrokeStyleStack.top();
					std::set<std::string> classNames;
					ParseGraphic(child, fillStyleChild, strokeStyleChild, classNames);
					mClippingPaths[id.value] = std::make_shared<ClippingPath>(
						true, fillStyleChild.clipRule, std::move(path),
						std::move(transform));
					hasClipContent = true;
					break;
				}
			}
			if (!hasClipContent)
				mClippingPaths[id.value] = std::make_shared<ClippingPath>(
					false, WindingRule::kNonZero, nullptr, nullptr);
			mFillStyleStack.pop();
			mStrokeStyleStack.pop();
		}
	}

	std::unique_ptr<D2DSVGPath> SVGDocument::ParseShape(XMLNode* child, FillStyleImpl& fillStyle)
	{
		SVG_ASSERT(child != nullptr);
		bool isLinear = false;
		//bool isRadia = false;
		if (fillStyle.hasFill) // 有填充色  2025.01.14 尝试修复 渐变
		{
			if (std::holds_alternative<GradientImpl>(fillStyle.internalPaint))
			{
				auto& gradient = std::get<GradientImpl>(fillStyle.internalPaint);
				if (gradient.type == GradientType::kLinearGradient) // 如果是 线性渐变 则?
				{
					isLinear = true;
				}
				else if (gradient.type == GradientType::kRadialGradient) // 如果是 径向渐变 则?
				{
					//isRadia = true;
				}

			}
		}

		const auto elementName = child->GetName();
		if (!strcmp(elementName, kRectElem)) 
		{
			float x = ParseLengthFromAttr(child, kXAttr, LengthType::kHorizontal);
			float y = ParseLengthFromAttr(child, kYAttr, LengthType::kVertical);

			float width = ParseLengthFromAttr(child, kWidthAttr, LengthType::kHorizontal);
			float height = ParseLengthFromAttr(child, kHeightAttr, LengthType::kVertical);
			if (isLinear)//线性渐变 修改起始点
			{
				auto& gradient = std::get<GradientImpl>(fillStyle.internalPaint);
				float x1 = gradient.x1;
				float y1 = gradient.y1;
				float x2 = gradient.x2;
				float y2 = gradient.y2;

				gradient.x1 = x + x1 * width;
				gradient.y1 = y + y1 * height;
				gradient.x2 = x + x2 * width;
				gradient.y2 = y + y2 * height;
			}

			// SVG requires to disable rendering if width or height are 0.
			if (isCloseToZero(width) || isCloseToZero(height))
				return nullptr;

			auto rxAttr = child->GetAttribute(kRxAttr);
			auto ryAttr = child->GetAttribute(kRyAttr);

			float rx{};
			float ry{};
			if (rxAttr.found && ryAttr.found) {
				rx = ParseLengthFromAttr(child, kRxAttr, LengthType::kHorizontal);
				ry = ParseLengthFromAttr(child, kRyAttr, LengthType::kVertical);
			}
			else if (rxAttr.found) {
				// the svg spec says that rect elements that specify a rx but not a ry
				// should use the rx value for ry
				rx = ParseLengthFromAttr(child, kRxAttr, LengthType::kHorizontal);
				ry = rx;
			}
			else if (ryAttr.found) {
				// the svg spec says that rect elements that specify a ry but not a rx
				// should use the ry value for rx
				ry = ParseLengthFromAttr(child, kRyAttr, LengthType::kVertical);
				rx = ry;
			}
			else {
				rx = 0;
				ry = 0;
			}

			rx = min(rx, width / 2.0f);         //std::
			ry = min(ry, height / 2.0f);        //std::

			auto path = CreatePath();
			if (isCloseToZero(rx) || isCloseToZero(ry)) {
				path->Rect(x, y, width, height);
			}
			else {
				path->RoundedRect(x, y, width, height, rx, ry);
			}
			return path;
		}
		else if (!strcmp(elementName, kEllipseElem) || !strcmp(elementName, kCircleElem)) 
		{
			float rx{}, ry{};

			if (!strcmp(elementName, kEllipseElem)) {
				rx = ParseLengthFromAttr(child, kRxAttr, LengthType::kHorizontal);
				ry = ParseLengthFromAttr(child, kRyAttr, LengthType::kVertical);
			}
			else {
				rx = ParseLengthFromAttr(child, kRAttr, LengthType::kDiagonal);
				ry = rx;
			}

			// SVG requires to disable rendering if rx or ry are 0.
			if (isCloseToZero(rx) || isCloseToZero(ry))
				return nullptr;

			float cx = ParseLengthFromAttr(child, kCxAttr, LengthType::kHorizontal);
			float cy = ParseLengthFromAttr(child, kCyAttr, LengthType::kVertical);

			auto path = CreatePath();
			path->Ellipse(cx, cy, rx, ry);

			return path;
		}
		else if (!strcmp(elementName, kPolygonElem) || !strcmp(elementName, kPolylineElem)) 
		{
			auto attr = child->GetAttribute(kPointsAttr);
			if (!attr.found)
				return nullptr;
			// This does not follow the spec which requires at least one space or comma
			// between coordinate pairs. However, Blink and WebKit do it the same way.
			std::vector<float> numberList;
			SVGStringParser::ParseListOfNumbers(attr.value, numberList);
			auto size = numberList.size();
			auto path = CreatePath();
			if (size > 1) {
				if (size % 2 == 1)
					--size;
				decltype(size) i{};
				path->MoveTo(numberList[i], numberList[i + 1]);
				i += 2;
				for (; i < size; i += 2)
					path->LineTo(numberList[i], numberList[i + 1]);
				if (!strcmp(elementName, kPolygonElem))
					path->ClosePath();
			}

			return path;
		}
		else if (!strcmp(elementName, kPathElem)) // 转移出去处理,因为svg的path可能存在多个闭合曲线,而D2D一个ID2D1PathGeometry对象只能open一次
		{  
			auto attr = child->GetAttribute(kDAttr);
			if (!attr.found)
				return nullptr;
		
			auto path = CreatePath();
			SVGStringParser::ParsePathString(attr.value, *path);
			path->StopPath();

			return path;
		}
		else if (!strcmp(elementName, kLineElem)) 
		{
			auto path = CreatePath();
			path->MoveTo(ParseLengthFromAttr(child, kX1Attr, LengthType::kHorizontal),
				ParseLengthFromAttr(child, kY1Attr, LengthType::kVertical));
			path->LineTo(ParseLengthFromAttr(child, kX2Attr, LengthType::kHorizontal),
				ParseLengthFromAttr(child, kY2Attr, LengthType::kVertical));

			return path;
		}
		return nullptr;
	}

	GraphicStyleImpl
		SVGDocument::ParseGraphic(const XMLNode* node, FillStyleImpl& fillStyle,
			StrokeStyleImpl& strokeStyle,
			std::set<std::string>& classNames) {
		SVG_ASSERT(node != nullptr);

		std::vector<PropertySet> propertySets;
		propertySets.push_back(ParsePresentationAttributes(node));
		ParseStyleAttr(node, propertySets, classNames);

		GraphicStyleImpl graphicStyle{};
		for (const auto& propertySet : propertySets) {
			ParseGraphicsProperties(graphicStyle, propertySet);
			ParseFillProperties(fillStyle, propertySet);
			ParseStrokeProperties(strokeStyle, propertySet);
		}

		auto transformAttr = node->GetAttribute(kTransformAttr);
		if (transformAttr.found &&
			node != mRootNode) // Ignore transforms on root SVG node
		{

			graphicStyle.transform = CreateTransform();
			if (!SVGStringParser::ParseTransform(transformAttr.value,
				*graphicStyle.transform))
				graphicStyle.transform.reset();
		}

		return graphicStyle;
	}

	static inline void AddDetectedProperty(const XMLNode* node,
		PropertySet& propertySet,
		const char* propertyName) {
		auto attr = node->GetAttribute(propertyName);
		if (attr.found)
			propertySet.insert({ propertyName, attr.value });
	}

	PropertySet SVGDocument::ParsePresentationAttributes(const XMLNode* node) {
		SVG_ASSERT(node != nullptr);

		PropertySet propertySet;
		for (const auto& propertyName : gInheritedPropertyNames)
			AddDetectedProperty(node, propertySet, propertyName);
		for (const auto& propertyName : gNonInheritedPropertyNames)
			AddDetectedProperty(node, propertySet, propertyName);
		return propertySet;
	}

	void SVGDocument::ParseFillProperties(FillStyleImpl& fillStyle,
		const PropertySet& propertySet) {
		auto prop = propertySet.find(kFillProp);
		auto iterEnd = propertySet.end();
		if (prop != iterEnd)
		{
			auto result = SVGStringParser::ParsePaint(
				prop->second, mGradients, mViewBox, fillStyle.internalPaint);
			if (result == SVGDocument::Result::kDisabled)
				fillStyle.hasFill = false;
			else if (result == SVGDocument::Result::kSuccess)
				fillStyle.hasFill = true;
		}

		prop = propertySet.find(kFillOpacityProp);
		if (prop != iterEnd)
		{
			float opacity{};
			if (SVGStringParser::ParseAlphaValue(prop->second, opacity))
				fillStyle.fillOpacity =
				std::max<float>(0.0, std::min<float>(1.0, opacity));
		}

		prop = propertySet.find(kFillRuleProp);
		if (prop != iterEnd)
		{
			if (prop->second == kEvenoddVal)
				fillStyle.fillRule = WindingRule::kEvenOdd;
			else if (prop->second == kNonzeroVal)
				fillStyle.fillRule = WindingRule::kNonZero;
		}

		// Other inherited properties
		prop = propertySet.find(kColorProp);
		if (prop != iterEnd) {
			ColorImpl color = Color{ {0.0f, 0.0f, 0.0f, 1.0f} };
			auto result = SVGStringParser::ParseColor(prop->second, color, false);
			if (result == SVGDocument::Result::kSuccess)
				fillStyle.color = color;
		}

		prop = propertySet.find(kVisibilityProp);
		if (prop != iterEnd) {
			const auto& visibilityString = prop->second;
			if (visibilityString == kHiddenVal)
				fillStyle.visibility = false;
			else if (visibilityString == kCollapseVal ||
				visibilityString == kVisibleVal)
				fillStyle.visibility = true;
		}

		prop = propertySet.find(kClipRuleProp);
		if (prop != iterEnd) {
			if (prop->second == kEvenoddVal)
				fillStyle.clipRule = WindingRule::kEvenOdd;
			else if (prop->second == kNonzeroVal)
				fillStyle.clipRule = WindingRule::kNonZero;
		}
	}

	void SVGDocument::ParseStrokeProperties(StrokeStyleImpl& strokeStyle,
		const PropertySet& propertySet) {
		auto prop = propertySet.find(kStrokeProp);
		auto iterEnd = propertySet.end();
		if (prop != iterEnd) {
			auto result = SVGStringParser::ParsePaint(
				prop->second, mGradients, mViewBox, strokeStyle.internalPaint);
			if (result == SVGDocument::Result::kDisabled)
				strokeStyle.hasStroke = false;
			else if (result == SVGDocument::Result::kSuccess)
				strokeStyle.hasStroke = true;
		}

		prop = propertySet.find(kStrokeWidthProp);
		if (prop != iterEnd) {
			float strokeWidth{};
			// Ignore stroke-width if invalid or negative.
			if (SVGStringParser::ParseLengthOrPercentage(
				prop->second, RelativeLength(LengthType::kDiagonal), strokeWidth,
				true) &&
				strokeWidth >= 0)
				strokeStyle.lineWidth = strokeWidth;
			// Disable stroke on a stroke-width of 0.
			if (strokeWidth == 0.0)
				strokeStyle.hasStroke = false;
		}

		prop = propertySet.find(kStrokeLinecapProp);
		if (prop != iterEnd) {
			if (prop->second == kRoundVal)
				strokeStyle.lineCap = LineCap::kRound;
			else if (prop->second == kSquareVal)
				strokeStyle.lineCap = LineCap::kSquare;
		}

		prop = propertySet.find(kStrokeLinejoinProp);
		if (prop != iterEnd) {
			if (prop->second == kRoundVal)
				strokeStyle.lineJoin = LineJoin::kRound;
			else if (prop->second == kBevelVal)
				strokeStyle.lineJoin = LineJoin::kBevel;
		}

		prop = propertySet.find(kStrokeMiterlimitProp);
		if (prop != iterEnd) {
			float miter{};
			// Miter must be bigger 1. Otherwise ignore.
			if (SVGStringParser::ParseNumber(prop->second, miter) && miter >= 1)
				strokeStyle.miterLimit = miter;
		}

		prop = propertySet.find(kStrokeDashoffsetProp);
		if (prop != iterEnd) {
			float strokeDashoffset{};
			if (SVGStringParser::ParseLengthOrPercentage(
				prop->second, RelativeLength(LengthType::kDiagonal),
				strokeDashoffset, true))
				strokeStyle.dashOffset = strokeDashoffset;
		}

		prop = propertySet.find(kStrokeDasharrayProp);
		if (prop != iterEnd) {
			float diagonal =
				sqrtf(mViewBox[2] * mViewBox[2] + mViewBox[3] * mViewBox[3]);
			if (!SVGStringParser::ParseListOfLengthOrPercentage(
				prop->second.c_str(), diagonal, strokeStyle.dashArray, true))
				strokeStyle.dashArray.clear();
			for (auto it = strokeStyle.dashArray.begin();
				it < strokeStyle.dashArray.end(); ++it) {
				if (*it < 0) {
					strokeStyle.dashArray.clear();
					break;
				}
			}
			const auto sizeOfDashArray = strokeStyle.dashArray.size();
			if (sizeOfDashArray % 2 != 0) {
				// If stroke-dasharray[] is odd-sized, the array should be twiced.
				// See SVG 1.1 (2nd ed), 11.4 "Stroke Properties" for detail.
				strokeStyle.dashArray.reserve(sizeOfDashArray * 2);
				for (size_t i = 0; i < sizeOfDashArray; ++i)
					strokeStyle.dashArray.push_back(strokeStyle.dashArray[i]);
			}
		}

		prop = propertySet.find(kStrokeOpacityProp);
		if (prop != iterEnd) {
			float opacity{};
			if (SVGStringParser::ParseAlphaValue(prop->second, opacity))
				strokeStyle.strokeOpacity =
				std::max<float>(0.0, std::min<float>(1.0, opacity));
		}
	}

	void SVGDocument::ParseGraphicsProperties(GraphicStyleImpl& graphicStyle,
		const PropertySet& propertySet) {
		auto prop = propertySet.find(kOpacityProp);
		auto iterEnd = propertySet.end();
		if (prop != iterEnd) {
			float opacity{};
			if (SVGStringParser::ParseAlphaValue(prop->second, opacity))
				graphicStyle.opacity =
				std::max<float>(0.0, std::min<float>(1.0, opacity));
		}

		prop = propertySet.find(kClipPathProp);
		if (prop != iterEnd) {
			// FIXME: Use proper parser.
			const auto urlLength = strlen(kUrlVal);
			const auto& valueString = prop->second;
			auto id = valueString.substr(urlLength, valueString.size() - urlLength - 1);
			auto clippingPathIt = mClippingPaths.find(id);
			if (clippingPathIt != mClippingPaths.end())
				graphicStyle.clippingPath = clippingPathIt->second;
		}

		prop = propertySet.find(kDisplayProp);
		if (prop != iterEnd) {
			if (prop->second.compare(kNoneVal))
				graphicStyle.display = false;
		}

		prop = propertySet.find(kStopOpacityProp);
		if (prop != iterEnd) {
			float opacity{};
			if (SVGStringParser::ParseAlphaValue(prop->second, opacity))
				graphicStyle.stopOpacity =
				std::max<float>(0.0, std::min<float>(1.0, opacity));
		}

		prop = propertySet.find(kStopColorProp);
		if (prop != iterEnd) {
			ColorImpl color = Color{ {0.0f, 0.0f, 0.0f, 1.0f} };
			const auto result = SVGStringParser::ParseColor(prop->second, color, true);
			if (result == SVGDocument::Result::kSuccess)
				graphicStyle.stopColor = color;
		}
	}

	float SVGDocument::ParseColorStop(const XMLNode* node,
		std::vector<ColorStopImpl>& colorStops,
		float lastOffset) {
		SVG_ASSERT(node != nullptr);

		auto fillStyle = mFillStyleStack.top();
		auto strokeStyle = mStrokeStyleStack.top();
		std::set<std::string> classNames;
		auto graphicStyle = ParseGraphic(node, fillStyle, strokeStyle, classNames);

		// * New stops may never appear before previous stops. Use offset of previous
		// stop otherwise.
		// * Stops must be in the range [0.0, 1.0].
		float offset{};
		auto attr = node->GetAttribute(kOffsetAttr);
		offset = (attr.found && SVGStringParser::ParseAlphaValue(attr.value, offset))
			? offset
			: lastOffset;
		offset = std::max<float>(lastOffset, offset);
		offset = std::min<float>(1.0, std::max<float>(0.0, offset));

		ColorImpl& paint = graphicStyle.stopColor;
		if (std::holds_alternative<ColorKeys>(paint)) {
			// Value is "currentColor". Simply set value to CSS color property.
			paint = fillStyle.color;
		}

		graphicStyle.stopOpacity =
			std::max<float>(0.0, std::min<float>(1.0, graphicStyle.stopOpacity));

		colorStops.push_back(
			std::make_tuple(offset, paint, graphicStyle.stopOpacity));
		return offset;
	}

	void SVGDocument::ParseColorStops(XMLNode* node, GradientImpl& gradient)
	{
		SVG_ASSERT(node != nullptr);
		float lastOffset{};
		std::vector<ColorStopImpl> colorStops;
		for (auto child = node->GetFirstNode(); child != nullptr;
			child = child->GetNextSibling()) {
			if (!strcmp(child->GetName(), kStopElem))
				lastOffset = ParseColorStop(child.get(), colorStops, lastOffset);
		}
		// Make sure we always have stops in the range 0% and 100%.
		if (colorStops.size() > 1) {
			const auto& firstStop = colorStops.front();
			if (std::get<0>(firstStop) != 0.0f)
				colorStops.insert(colorStops.begin(),
					std::make_tuple(0.0f, std::get<1>(firstStop),
						std::get<2>(firstStop)));
			const auto& lastStop = colorStops.back();
			if (std::get<0>(lastStop) != 1.0f)
				colorStops.push_back(
					std::make_tuple(1.0f, std::get<1>(lastStop), std::get<2>(lastStop)));
		}
		// Keep the color stops from referenced gradients if the current gradient
		// has none.
		if (!colorStops.empty())
			gradient.internalColorStops = colorStops;
	}

	void SVGDocument::ParseGradient(XMLNode* node)
	{
		SVG_ASSERT(node != nullptr);

		GradientImpl gradient{};

		// SVG allows referencing other gradients. For now, we only look at already
		// parsed gradients. Since we add the current gradient after successful
		// parsing, this also avoids circular references.
		// https://www.w3.org/TR/SVG11/pservers.html#LinearGradientElementHrefAttribute
		auto attr = node->GetAttribute(kHrefAttr, kXlinkNS);
		if (attr.found) {
			std::string href{ attr.value };
			// href starts with a #, ignore it.
			auto it = mGradients.find(href.substr(1));
			if (it != mGradients.end())
				gradient = it->second;
		}

		ParseColorStops(node, gradient);

		const auto elementName = node->GetName();
		if (!strcmp(elementName, kLinearGradientElem))
			gradient.type = GradientType::kLinearGradient;
		else if (!strcmp(elementName, kRadialGradientElem))
			gradient.type = GradientType::kRadialGradient;
		else {
			SVG_ASSERT_MSG(false, "Gradient parser called with invalid element");
			return;
		}

		// TODO: Do we want to support `gradientUnits="objectBoundingBox"` at all?
		// This would require us to get the bounding box of the filled/stroked shape
		// when the gradient gets applied.

		if (gradient.type == GradientType::kLinearGradient) {
			// https://www.w3.org/TR/SVG11/pservers.html#LinearGradients
			gradient.x1 = ParseLengthFromAttr(node, kX1Attr, LengthType::kHorizontal,
				gradient.x1);
			gradient.y1 =
				ParseLengthFromAttr(node, kY1Attr, LengthType::kVertical, gradient.y1);
			gradient.x2 = ParseLengthFromAttr(node, kX2Attr, LengthType::kHorizontal,
				gradient.x2);
			gradient.y2 =
				ParseLengthFromAttr(node, kY2Attr, LengthType::kVertical, gradient.y2);
		}
		else {
			// https://www.w3.org/TR/SVG11/pservers.html#RadialGradients
			gradient.cx = ParseLengthFromAttr(node, kCxAttr, LengthType::kHorizontal,
				gradient.cx);
			gradient.cy =
				ParseLengthFromAttr(node, kCyAttr, LengthType::kVertical, gradient.cy);
			gradient.fx = ParseLengthFromAttr(node, kFxAttr, LengthType::kHorizontal,
				gradient.fx);
			gradient.fy =
				ParseLengthFromAttr(node, kFyAttr, LengthType::kVertical, gradient.fy);
			gradient.r =
				ParseLengthFromAttr(node, kRAttr, LengthType::kDiagonal, gradient.r);
		}

		attr = node->GetAttribute(kSpreadMethodAttr);
		if (attr.found) {
			const auto spreadMethodString = attr.value;
			if (!strcmp(spreadMethodString, kPadVal))
				gradient.method = SpreadMethod::kPad;
			else if (!strcmp(spreadMethodString, kReflectVal))
				gradient.method = SpreadMethod::kReflect;
			else if (!strcmp(spreadMethodString, kRepeatVal))
				gradient.method = SpreadMethod::kRepeat;
		}
		attr = node->GetAttribute(kGradientTransformAttr);
		if (attr.found)
		{
			gradient.transform = CreateTransform();
			if (!SVGStringParser::ParseTransform(attr.value, *gradient.transform))
				gradient.transform.reset();
		}
		attr = node->GetAttribute(kIdAttr);
		if (attr.found)
			mGradients.insert({ attr.value, gradient });
	}

	void SVGDocument::Render(const ColorMap& colorMap, float width, float height)
	{
		SVG_ASSERT(mGroup);
		if (!mGroup)
			return;

		RenderElement(*mGroup, colorMap, width, height);
	}

	void SVGDocument::Render(const char* id, const ColorMap& colorMap, float width, float height)
	{
		// Referenced glyph identifiers shall be rendered as if they were contained in a <defs> section under the root SVG element:
		// Therefore, the referenced shape/group should:
		// * ignore all styling and transforms on ancestors.
		// * inherit property values from the root SVG element,https://docs.microsoft.com/en-us/typography/opentype/spec/svg#glyph-identifiers
		auto elementIter = mIdToElementMap.find(id);
		if (elementIter != mIdToElementMap.end())
			RenderElement(*elementIter->second, colorMap, width, height);
	}

	void SVGDocument::RenderElement(const Element& element, const ColorMap& colorMap, float width_, float height_)
	{
		if (width_ == 0)
		{
			width_ = width;
		}
		if (height_ == 0)
		{
			height_ = height;
		}
		float scalew = width_ / mViewBox[2];
		float scaleh = height_ / mViewBox[3];
		scale = scalew;
		if (scalew > scaleh)
		{
			scale = scaleh;
		}
		GraphicStyleImpl graphicStyle{};
		graphicStyle.transform = CreateTransform();
		graphicStyle.transform->Translate(-1 * mViewBox[0], -1 * mViewBox[1]);
		graphicStyle.transform->Scale(scale, scale);

		Save(graphicStyle);

		TraverseTree(colorMap, element);
		SVG_ASSERT(mVisitedElements.empty());

		Restore();

	}

	bool SVGDocument::GetBoundingBox(Rect& bound) {
		SVG_ASSERT(mGroup);
		if (!mGroup)
			return false;

		GraphicStyleImpl graphicStyle{};
		graphicStyle.transform = CreateTransform();
		graphicStyle.transform->Translate(-1 * mViewBox[0], -1 * mViewBox[1]);


		Save(graphicStyle);
		ExtractBounds(*mGroup);

		Restore();

		SVG_ASSERT(mVisitedElements.empty());

		Rect sumBound{ 0, 0, 0, 0 };
#ifdef DEBUG_API
		for (auto const& bound : mBounds)
			sumBound = sumBound | bound;
#else
		sumBound = mBound;
#endif
		bound = sumBound;
		return true;
	}

	bool SVGDocument::GetBoundingBox(const char* id, Rect& bound) {
		SVG_ASSERT(mGroup);
		if (!mGroup)
			return false;

		// TODO: Maybe this needs fixing as I'm not doing any scaling, we must
		// figure out a way to supply width/height for this I guess?
		GraphicStyleImpl graphicStyle{};
		graphicStyle.transform = CreateTransform();
		graphicStyle.transform->Translate(-1 * mViewBox[0], -1 * mViewBox[1]);


		Save(graphicStyle);
		const auto elementIter = mIdToElementMap.find(id);
		SVG_ASSERT(elementIter != mIdToElementMap.end());
		ExtractBounds(*elementIter->second);

		Restore();
		SVG_ASSERT(mVisitedElements.empty());

		Rect sumBound{ 0, 0, 0, 0 };
#ifdef DEBUG_API
		for (auto const& bound : mBounds)
			sumBound = sumBound | bound;
#else
		sumBound = mBound;
#endif
		bound = sumBound;
		return true;
	}

#ifdef DEBUG_API
	bool GetSubBoundingBoxes(std::vector<Rect>& bounds);
	{
		SVG_ASSERT(mGroup);
		if (!mGroup)
			return false;
		GraphicStyleImpl graphicStyle{};
		graphicStyle.transform = CreateTransform();
		graphicStyle.transform->Translate(-1 * mViewBox[0], -1 * mViewBox[1]);


		Save(graphicStyle);
		ExtractBounds(*mGroup);

		Restore();
		SVG_ASSERT(mVisitedElements.empty());
		bounds = mBounds;
		return true;
	}

	bool GetSubBoundingBoxes(const char* id, std::vector<Rect>& bounds);
	{
		SVG_ASSERT(mGroup);
		if (!mGroup)
			return false;
		GraphicStyleImpl graphicStyle{};
		graphicStyle.transform = CreateTransform();
		graphicStyle.transform->Translate(-1 * mViewBox[0], -1 * mViewBox[1]);


		Save(graphicStyle);
		const auto elementIter = mIdToElementMap.find(id);
		SVG_ASSERT(elementIter != mIdToElementMap.end());
		ExtractBounds(*elementIter->second);

		Restore();
		SVG_ASSERT(mVisitedElements.empty());
		bounds = mBounds;
		return true;
	}
#endif

	void SVGDocument::ExtractBounds(const Element& element) {
		// This function is based on the TraverseTree function, we just calculate
		// the bounds instead of doing any drawing.

		auto graphicStyle = element.graphicStyle;
		FillStyleImpl fillStyle{};
		StrokeStyleImpl strokeStyle{};
		// Has no bound contribution if there is no clipContent and clip path is set
		if (graphicStyle.clippingPath && !graphicStyle.clippingPath->hasClipContent)
			return;

		switch (element.Type()) {
		case ElementType::kReference: {
			const auto& reference = static_cast<const Reference&>(element);
			const auto it = mVisitedElements.find(&reference);
			if (it != mVisitedElements.end())
				break; // We found a cycle. Do not continue rendering.
			auto insertResult = mVisitedElements.insert(&reference);

			// Render referenced content.
			auto refIt = mIdToElementMap.find(reference.href);
			if (refIt != mIdToElementMap.end()) {
				ApplyCSSStyle(reference.classNames, graphicStyle, fillStyle, strokeStyle);


				Save(graphicStyle);
				ExtractBounds(*(refIt->second));

				Restore();
			}

			// Done processing current element.
			mVisitedElements.erase(insertResult.first);
			break;
		}
		case ElementType::kGraphic: {
			const auto& graphic = static_cast<const Graphic&>(element);
			// TODO: Since we keep the original fill, stroke and color property values
			// we should be able to do w/o a copy.
			fillStyle = graphic.fillStyle;
			strokeStyle = graphic.strokeStyle;
			ApplyCSSStyle(graphic.classNames, graphicStyle, fillStyle, strokeStyle);
			Rect bounds = GetBounds(*(graphic.path.get()), graphicStyle,
				fillStyle, strokeStyle);
			if (!bounds.IsEmpty()) {
#ifdef DEBUG_API
				mBounds.push_back(bounds);
#else
				mBound = mBound | bounds;
#endif
			}
			break;
		}
		case ElementType::kImage: {
			const auto& image = static_cast<const Image&>(element);
			ApplyCSSStyle(image.classNames, graphicStyle, fillStyle, strokeStyle);
			// TODO: How to handle image's bounds?
			auto path = CreatePath();
			path->Rect(image.fillArea.x, image.fillArea.y, image.fillArea.width,
				image.fillArea.height);
			Rect bounds = GetBounds(*path.get(), GraphicStyle{}, FillStyle{},
				StrokeStyle{});
			if (!bounds.IsEmpty()) {
#ifdef DEBUG_API
				mBounds.push_back(bounds);
#else
				mBound = mBound | bounds;
#endif
			}
			break;
		}
		case ElementType::kGroup: {
			const auto& group = static_cast<const Group&>(element);
			ApplyCSSStyle(group.classNames, graphicStyle, fillStyle, strokeStyle);


			Save(graphicStyle);
			for (const auto& child : group.children)
				ExtractBounds(*child);

			Restore();
			break;
		}
		default:
			SVG_ASSERT_MSG(false, "Unknown element type");
		}
	}

	void SVGDocument::AddChildToCurrentGroup(std::shared_ptr<Element> element,
		std::string idString) {
		SVG_ASSERT(!mGroupStack.empty());
		if (mGroupStack.empty())
			return;

		mGroupStack.top()->children.push_back(element);

		if (!idString.empty() &&
			mIdToElementMap.find(idString) == mIdToElementMap.end())
			mIdToElementMap.emplace(std::move(idString), element);
	}

	static void ResolveColorImpl(const ColorMap& colorMap,
		const ColorImpl& colorImpl, Color& color) {
		if (std::holds_alternative<Variable>(colorImpl)) {
			const auto& var = std::get<Variable>(colorImpl);
			const auto colorIt = colorMap.find(var.first);
			color = colorIt != colorMap.end() ? colorIt->second : var.second;
		}
		else if (std::holds_alternative<Color>(colorImpl))
			color = std::get<Color>(colorImpl);
		else
			// Can only be reached if fallback color value of var() is currentColor.
			color = Color{ {0.0f, 0.0f, 0.0f, 1.0f} };
	}

	static void ResolvePaintImpl(const ColorMap& colorMap,
		const PaintImpl& internalPaint,
		const Color& currentColor, Paint& paint) {
		if (std::holds_alternative<Variable>(internalPaint)) {
			const auto& var = std::get<Variable>(internalPaint);
			const auto colorIt = colorMap.find(var.first);
			paint = colorIt != colorMap.end() ? colorIt->second : var.second;
		}
		else if (std::holds_alternative<GradientImpl>(internalPaint)) {
			// Stop colors may have variables as well.
			const auto& internalGradient = std::get<GradientImpl>(internalPaint);
			paint = std::move(internalGradient);
			auto& gradient = std::get<Gradient>(paint);
			for (const auto& colorStop : internalGradient.internalColorStops) {
				Color stopColor{ {0, 0, 0, 1.0} };
				const auto& colorImpl = std::get<1>(colorStop);
				if (std::holds_alternative<Variable>(colorImpl)) {
					const auto& var = std::get<Variable>(colorImpl);
					const auto colorIt = colorMap.find(var.first);
					stopColor = colorIt != colorMap.end() ? colorIt->second : var.second;
				}
				else if (std::holds_alternative<Color>(colorImpl))
					stopColor = std::get<Color>(colorImpl);
				else {
					SVG_ASSERT_MSG(false, "Unhandled ColorImpl type");
				}
				stopColor[3] *= std::get<2>(colorStop);
				gradient.colorStops.push_back({ std::get<0>(colorStop), stopColor });
			}
		}
		else if (std::holds_alternative<Color>(internalPaint))
			paint = std::get<Color>(internalPaint);
		else if (std::holds_alternative<ColorKeys>(internalPaint))
			// currentColor is the only possible enum value for now.
			paint = currentColor;
		else
			SVG_ASSERT_MSG(false, "Unhandled PaintImpl type");
	}

	void SVGDocument::TraverseTree(const ColorMap& colorMap, const Element& element)
	{
		// Inheritance doesn't work for override styles. Since override styles
		// are deprecated, we are not going to fix this nor is this expected by
		// (still existing) clients.
		auto graphicStyle = element.graphicStyle;
		FillStyleImpl fillStyle{};
		StrokeStyleImpl strokeStyle{};
		// Do not draw element if an applied clipPath has no content.
		if (graphicStyle.clippingPath && !graphicStyle.clippingPath->hasClipContent)
			return;
		switch (element.Type())
		{
		case ElementType::kReference:
		{
			const auto& reference = static_cast<const Reference&>(element);
			const auto it = mVisitedElements.find(&reference);
			if (it != mVisitedElements.end())
				break; // We found a cycle. Do not continue rendering.
			auto insertResult = mVisitedElements.insert(&reference);

			// Render referenced content.
			auto refIt = mIdToElementMap.find(reference.href);
			if (refIt != mIdToElementMap.end())
			{
				ApplyCSSStyle(reference.classNames, graphicStyle, fillStyle, strokeStyle);


				Save(graphicStyle);
				TraverseTree(colorMap, *(refIt->second));

				Restore();
			}

			// Done processing current element.
			mVisitedElements.erase(insertResult.first);
			break;
		}
		case ElementType::kGraphic: {
			const auto& graphic = static_cast<const Graphic&>(element);
			// TODO: Since we keep the original fill, stroke and color property values
			// we should be able to do w/o a copy.
			fillStyle = graphic.fillStyle;
			strokeStyle = graphic.strokeStyle;
			ApplyCSSStyle(graphic.classNames, graphicStyle, fillStyle, strokeStyle);
			// If we have a CSS var() function we need to replace the placeholder with
			// an actual color from our externally provided color map here.
			Color color{ {0.0f, 0.0f, 0.0f, 1.0f} };
			ResolveColorImpl(colorMap, fillStyle.color, color);
			ResolvePaintImpl(colorMap, fillStyle.internalPaint, color, fillStyle.paint);
			ResolvePaintImpl(colorMap, strokeStyle.internalPaint, color,
				strokeStyle.paint);
			DrawPath(*(graphic.path.get()), graphicStyle, fillStyle, strokeStyle);
			break;
		}
		case ElementType::kImage: {
			const auto& image = static_cast<const Image&>(element);
			ApplyCSSStyle(image.classNames, graphicStyle, fillStyle, strokeStyle);
			DrawImage(*(image.imageData.get()), graphicStyle, image.clipArea, image.fillArea);
			break;
		}
		case ElementType::kGroup: {
			const auto& group = static_cast<const Group&>(element);
			ApplyCSSStyle(group.classNames, graphicStyle, fillStyle, strokeStyle);


			Save(graphicStyle);
			for (const auto& child : group.children)
				TraverseTree(colorMap, *child);

			Restore();
			break;
		}
		default:
			SVG_ASSERT_MSG(false, "Unknown element type");
		}
	}

#ifndef STYLE_SUPPORT
	// Deprecated style support
	void SVGDocument::ApplyCSSStyle(const std::set<std::string>&,
		GraphicStyleImpl&, FillStyleImpl&,
		StrokeStyleImpl&) {
	}
	void SVGDocument::ParseStyleAttr(const XMLNode*,
		std::vector<PropertySet>&,
		std::set<std::string>&) {
	}
	void SVGDocument::ParseStyle(const XMLNode*) {}
#endif




	//--------------------------------------------------------------------------------------------------



	std::unique_ptr<D2DSVGImageData> SVGDocument::CreateImageData(const std::string& base64, ImageEncoding encoding)
	{
		auto ptr = base64.c_str();
		auto len = Base64Decode(ptr, base64.size(), (unsigned char*)ptr);
		ComPtr<IStream> stream{ SHCreateMemStream((const BYTE*)ptr, (UINT)len) };
		ThrowIfNull(stream);

		ComPtr<IWICBitmapDecoder> imgDecoder;
		ThrowIfFailed(g_Ri.pWICFactory->CreateDecoderFromStream(
                    stream.Get(), nullptr, WICDecodeMetadataCacheOnDemand,
                    &imgDecoder));

		ComPtr<IWICBitmapFrameDecode> frame;
		ThrowIfFailed(imgDecoder->GetFrame(0, &frame));

		// Convert to a format that Direct2D can use (pre-multiplied BRGA is best)
		ComPtr<IWICFormatConverter> converter;
		ThrowIfFailed(g_Ri.pWICFactory->CreateFormatConverter(&converter));
		ThrowIfFailed(converter->Initialize(
                    frame.Get(),
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			nullptr,
			0.0,
			WICBitmapPaletteTypeMedianCut));

		// Keep a BitmapSource and not an ID2D1Bitmap because the latter is tied to a specific ID2D1RenderTarget.
		ComPtr<IWICBitmapSource> bitmapSource = converter.Detach();
		return std::unique_ptr<D2DSVGImageData>(new D2DSVGImageData(bitmapSource));
	}

	std::unique_ptr<D2DSVGPath> SVGDocument::CreatePath()
	{
		return std::unique_ptr<D2DSVGPath>(new D2DSVGPath{ g_Ri.pD2Dfactory });
	}

	std::unique_ptr<Transform> SVGDocument::CreateTransform(float a, float b, float c, float d, float tx, float ty)
	{
		return std::unique_ptr<D2DSVGTransform>(new D2DSVGTransform(a, b, c, d, tx, ty));
	}

	void SVGDocument::Save(const GraphicStyle& graphicStyle)
	{
		ComPtr<ID2D1Geometry> maskPath;
		D2D1_MATRIX_3X2_F maskTransform = D2D1::IdentityMatrix();
		if (graphicStyle.clippingPath)
		{
			if (graphicStyle.clippingPath->transform)
				maskTransform = dynamic_cast<D2DSVGTransform*>(graphicStyle.clippingPath->transform.get())->GetMatrix();
			const auto constPath = dynamic_cast<const D2DSVGPath*>(graphicStyle.clippingPath->path.get());
			maskPath = const_cast<D2DSVGPath*>(constPath)->GetGraphicsPath();
		}

		ComPtr<ID2D1Layer> layer;
		mContext->CreateLayer(&layer);
		mContext->PushLayer(
			D2D1::LayerParameters(
				D2D1::InfiniteRect(), maskPath.Get(),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
				maskTransform,
				graphicStyle.opacity
			),
			layer.Get());

		// FIXME: May need to get applied before creating the layer to apply to clipping path.
		D2D1_MATRIX_3X2_F transform;
		mContext->GetTransform(&transform);
		mContextTransform.push(transform);
		if (graphicStyle.transform)
		{
			D2D1_MATRIX_3X2_F gsTransform = dynamic_cast<D2DSVGTransform*>(graphicStyle.transform.get())->GetMatrix();
			mContext->SetTransform(gsTransform * transform);
		}
	}

	void SVGDocument::Restore()
	{
		mContext->SetTransform(mContextTransform.top());
		mContextTransform.pop();
		mContext->PopLayer();
	}

	inline D2D1_EXTEND_MODE D2DSpreadMethod(SpreadMethod spreadMethod)
	{
		switch (spreadMethod)
		{
		case SpreadMethod::kReflect:
			return D2D1_EXTEND_MODE_MIRROR;
		case SpreadMethod::kRepeat:
			return D2D1_EXTEND_MODE_WRAP;
		case SpreadMethod::kPad:
		default:
			return D2D1_EXTEND_MODE_CLAMP;
		}
	}

	inline D2D1_CAP_STYLE D2DCapStyle(LineCap lineCap)
	{
		switch (lineCap)
		{
		case LineCap::kRound:
			return D2D1_CAP_STYLE_ROUND;
		case LineCap::kSquare:
			return D2D1_CAP_STYLE_SQUARE;
		case LineCap::kButt:
		default:
			return D2D1_CAP_STYLE_FLAT;
		}
	}

	inline D2D1_LINE_JOIN D2DLineJoin(LineJoin lineJoin)
	{
		switch (lineJoin)
		{
		case LineJoin::kRound:
			return D2D1_LINE_JOIN_ROUND;
		case LineJoin::kBevel:
			return D2D1_LINE_JOIN_BEVEL;
		case LineJoin::kMiter:
		default:
			return D2D1_LINE_JOIN_MITER;
		}
	}

	ComPtr<ID2D1Brush> SVGDocument::CreateBrush(const Paint& paint)
	{
		SVG_ASSERT(mContext);
		ComPtr<ID2D1Brush> brush;
		if (std::holds_alternative<Color>(paint))
		{
			const auto& color = std::get<Color>(paint);
			ComPtr<ID2D1SolidColorBrush> solidColorBrush;
			mContext->CreateSolidColorBrush({ color[0], color[1], color[2], color[3] }, &solidColorBrush);
			solidColorBrush.As(&brush);
		}
		else if (std::holds_alternative<Gradient>(paint))
		{
			const auto& gradient = std::get<Gradient>(paint);
			std::vector<D2D1_GRADIENT_STOP> colorsStops;
			for (const auto& stop : gradient.colorStops)
			{
				const auto& color = stop.second;
				colorsStops.push_back({ stop.first, { color[0], color[1], color[2], color[3] } });
			}
			ComPtr<ID2D1GradientStopCollection> gradientStopCollection;
			mContext->CreateGradientStopCollection(
				colorsStops.data(),
				static_cast<UINT32>(colorsStops.size()),
				D2D1_GAMMA_2_2,
				D2DSpreadMethod(gradient.method),
				&gradientStopCollection);

			if (gradient.type == GradientType::kLinearGradient)
			{
				ComPtr<ID2D1LinearGradientBrush> linearGradientBrush;
				mContext->CreateLinearGradientBrush(
					D2D1::LinearGradientBrushProperties(
						D2D1::Point2F(gradient.x1, gradient.y1),
						D2D1::Point2F(gradient.x2, gradient.y2)),
                                    gradientStopCollection.Get(),
					&linearGradientBrush);
				if (gradient.transform)
					linearGradientBrush->SetTransform(std::static_pointer_cast<D2DSVGTransform>(gradient.transform)->GetMatrix());
				linearGradientBrush.As(&brush);
			}
			else
			{
				float realcx = gradient.cx * scale;
				float realcy = gradient.cy * scale;
				float realfx = gradient.fx * scale - realcx;
				float realfy = gradient.fy * scale - realcy;
				float realr = gradient.r * scale;

				ComPtr<ID2D1RadialGradientBrush> radialGradientBrush;
				mContext->CreateRadialGradientBrush(
					D2D1::RadialGradientBrushProperties(
						D2D1::Point2F(realcx, realcy),
						D2D1::Point2F(realfx, realfy),
						realr,
						realr),
                                    gradientStopCollection.Get(),
					&radialGradientBrush);
				if (gradient.transform)
					radialGradientBrush->SetTransform(std::static_pointer_cast<D2DSVGTransform>(gradient.transform)->GetMatrix());
				radialGradientBrush.As(&brush);
			}
		}
		else
		{
			SVG_ASSERT_MSG(false, "Unknown paint type");
		}
		return brush;
	}

	void SVGDocument::DrawPath(const D2DSVGPath& renderPath, const GraphicStyle& graphicStyle, const FillStyle& fillStyle, const StrokeStyle& strokeStyle)
	{
		SVG_ASSERT(mContext);
		Save(graphicStyle);

		const auto& constPath = dynamic_cast<const D2DSVGPath&>(renderPath);
		auto path = const_cast<D2DSVGPath&>(constPath).GetGraphicsPath();
		if (fillStyle.hasFill)
		{
			auto brush = CreateBrush(fillStyle.paint);
                  mContext->FillGeometry(path.Get(), brush.Get(), nullptr);
			brush.Reset();
		}
		if (strokeStyle.hasStroke)
		{
			auto lineCap = D2DCapStyle(strokeStyle.lineCap);
			auto lineJoin = D2DLineJoin(strokeStyle.lineJoin);

			auto brush = CreateBrush(strokeStyle.paint);

			SVG_ASSERT(g_Ri.pD2Dfactory);
			ComPtr<ID2D1StrokeStyle> d2dStrokeStyle;
			g_Ri.pD2Dfactory->CreateStrokeStyle(
				D2D1::StrokeStyleProperties(lineCap, lineCap, lineCap, lineJoin, strokeStyle.miterLimit, D2D1_DASH_STYLE_CUSTOM, strokeStyle.dashOffset),
				strokeStyle.dashArray.data(),
				static_cast<UINT32>(strokeStyle.dashArray.size()),
				&d2dStrokeStyle);
			//auto hr =  //SVG_ASSERT(SUCCEEDED(hr)); //总是失败

			mContext->DrawGeometry(path.Get(), brush.Get(),
                                               strokeStyle.lineWidth,
                                               d2dStrokeStyle.Get());

			d2dStrokeStyle.Reset();
                        brush.Reset();
		}

		Restore();
	}

	void SVGDocument::DrawImage(const D2DSVGImageData& image, const GraphicStyle& graphicStyle, const Rect& clipArea, const Rect& fillArea)
	{
		const D2DSVGImageData& imageData(dynamic_cast<const D2DSVGImageData&>(image));
		ComPtr<IWICBitmapSource> bitmapSource = imageData.GetBitmapSource();
		if (bitmapSource)
		{
			ComPtr<ID2D1Bitmap> bitmap;
                  ThrowIfFailed(mContext->CreateBitmapFromWicBitmap(
                      bitmapSource.Get(), &bitmap));

			SVG_ASSERT(mContext);
			Save(graphicStyle);
			D2D1_RECT_F clipRect{};
			if (clipArea.width < fillArea.width || clipArea.height < fillArea.height)
			{
				clipRect = D2D1::RectF(clipArea.x, clipArea.y, clipArea.x + clipArea.width, clipArea.y + clipArea.height);
				ComPtr<ID2D1Layer> layer;
				mContext->CreateLayer(&layer);
				mContext->PushLayer(D2D1::LayerParameters(clipRect), layer.Get());
			}

			D2D1_RECT_F drawRect = D2D1::RectF(fillArea.x, fillArea.y, fillArea.x + fillArea.width, fillArea.y + fillArea.height);
                        mContext->DrawBitmap(bitmap.Get(), drawRect);

			if (clipRect.right > clipRect.left)
			{
				mContext->PopLayer();
			}

			Restore();
		}
	}

} // namespace SVGNative


