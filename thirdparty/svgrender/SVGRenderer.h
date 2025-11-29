/*
Copyright 2019 Adobe. All rights reserved.
This file is licensed to you under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License. You may obtain a copy
of the License at http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed under
the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR REPRESENTATIONS
OF ANY KIND, either express or implied. See the License for the specific language
governing permissions and limitations under the License.
*/

#ifndef SVGViewer_SVGRenderer_h
#define SVGViewer_SVGRenderer_h

#include "Config.h"
#include "Rect.h"

#include <array>
#include <limits>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include <variant>
#include "variant.hpp"
namespace SVGNative
{
	/**
	 * Supported image encoding formats are PNG and JPEG.
	 * The assumed encoding format based on the base64 string.
	 */
	enum class ImageEncoding
	{
		kPNG,
		kJPEG
	};

	/**
	 * Line caps as described in:
	 * https://www.w3.org/TR/SVG2/painting.html#LineCaps
	 */
	enum class LineCap
	{
		kButt,
		kRound,
		kSquare
	};

	/**
	 * Line joins as described in:
	 * https://www.w3.org/TR/SVG2/painting.html#LineJoin
	 */
	enum class LineJoin
	{
		kMiter,
		kRound,
		kBevel
	};

	/**
	 * Winding rules as described in:
	 * https://www.w3.org/TR/SVG2/painting.html#WindingRule
	 */
	enum class WindingRule
	{
		kNonZero,
		kEvenOdd
	};

	/**
	 * Gradient type. SVG Native supports the 2 gradient types
	 * * linear gradient and
	 * * radial gradient.
	 */
	enum class GradientType
	{
		kLinearGradient,
		kRadialGradient
	};

	/**
	 * Gradient spread method.
	 * * pad
	 * * reflect
	 * * repeat
	 *
	 * @note See https://www.w3.org/TR/SVG11/pservers.html#LinearGradientElementSpreadMethodAttribute
	 */
	enum class SpreadMethod
	{
		kPad,
		kReflect,
		kRepeat
	};

	struct Gradient;
	class Transform;
	class D2DSVGPath;

	using Color = std::array<float, 4>;
	using Paint = std::variant<Color, Gradient>;


	using ColorStop = std::pair<float, Color>;
	using ColorMap = std::map<std::string, Color>;

	/**
	 * Representation of a linear gradient paint server.
	 */
	struct Gradient
	{
		GradientType type = GradientType::kLinearGradient;
		SpreadMethod method = SpreadMethod::kPad;
		std::vector<ColorStop> colorStops; /** Color stops with offset-color pairs **/
		float x1 = std::numeric_limits<float>::quiet_NaN(); /** x1 for linearGradient **/
		float y1 = std::numeric_limits<float>::quiet_NaN(); /** y1 for linearGradient **/
		float x2 = std::numeric_limits<float>::quiet_NaN(); /** x2 for linearGradient **/
		float y2 = std::numeric_limits<float>::quiet_NaN(); /** y2 for linearGradient **/
		float cx = std::numeric_limits<float>::quiet_NaN(); /** cx for radialGradient **/
		float cy = std::numeric_limits<float>::quiet_NaN(); /** cy for radialGradient **/
		float fx = std::numeric_limits<float>::quiet_NaN(); /** fx for radialGradient **/
		float fy = std::numeric_limits<float>::quiet_NaN(); /** fy for radialGradient **/
		float r = std::numeric_limits<float>::quiet_NaN(); /** r for radialGradient **/
		std::shared_ptr<Transform> transform; /** Joined transformation matrix based to the "transform" attribute. **/
	};

	/**
	 * Stroke style information.
	 */
	struct StrokeStyle
	{
		bool hasStroke = false;
		float strokeOpacity = 1.0;
		float lineWidth = 1.0;
		LineCap lineCap = LineCap::kButt;
		LineJoin lineJoin = LineJoin::kMiter;
		float miterLimit = 4.0;
		std::vector<float> dashArray;
		float dashOffset = 0.0;
		Paint paint = Color{ {0, 0, 0, 1.0} };
	};

	/**
	 * Fill style information.
	 */
	struct FillStyle
	{
		bool hasFill = true;
		WindingRule fillRule = WindingRule::kNonZero;
		float fillOpacity = 1.0;
		Paint paint = Color{ {0, 0, 0, 1.0} };
	};

	/**
	 * Representation of a 2D affine transform with 6 values.
	 */
	class Transform
	{
	public:
		virtual ~Transform() = default;

		virtual void Set(float a, float b, float c, float d, float tx, float ty) = 0;
		virtual void Rotate(float r) = 0;
		virtual void Translate(float tx, float ty) = 0;
		virtual void Scale(float sx, float sy) = 0;
		virtual void Concat(float a, float b, float c, float d, float tx, float ty) = 0;
	};

	struct ClippingPath
	{
		ClippingPath(bool aHasClipContent, WindingRule aClipRule, std::shared_ptr<D2DSVGPath> aPath, std::shared_ptr<Transform> aTransform)
			: hasClipContent{ aHasClipContent }
			, clipRule{ aClipRule }
			, path{ aPath }
			, transform{ aTransform }
		{
		}

		bool hasClipContent = false;
		WindingRule clipRule = WindingRule::kNonZero;
		std::shared_ptr<D2DSVGPath> path; /** Clipping path. **/
		std::shared_ptr<Transform> transform; /** Joined transformation matrix based to the "transform" attribute. **/
	};

	/**
	 * All compositing related properties. With the exception of the
	 */
	struct GraphicStyle
	{
		// Add blend modes and other graphic style options here.
		float opacity = 1.0; /** Corresponds to the "opacity" CSS property. **/
		std::shared_ptr<Transform> transform; /** Joined transformation matrix based to the "transform" attribute. **/
		std::shared_ptr<ClippingPath> clippingPath;
	};

	

} // namespace SVGNative

#endif // SVGViewer_SVGRenderer_h
