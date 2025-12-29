#include "stdafx.h"

void _prototype_board_register()
{
	Ex_ObjRegister(L"PrototypeBoard", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_TABSTOP, 0, 8 * 59, NULL, NULL, _prototype_board_proc);
}

LRESULT CALLBACK _prototype_board_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_CREATE)
	{
		_prototype_board_initparam(hObj);
	}
	else if (uMsg == WM_DESTROY)
	{
		_prototype_board_uninitparam(hObj);
	}
	else if (uMsg == WM_PAINT)
	{
		_prototype_board_paint(hObj);
	}
	else if (uMsg == WM_SIZE)
	{
		INT width = LOWORD(lParam);
		INT height = HIWORD(lParam);
		_prototype_board_calc_size(hObj, width, height);
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		_prototype_board_leftbuttondown(hObj, lParam);
	}
	else if (uMsg == WM_LBUTTONUP)
	{
		_prototype_board_leftbuttonup(hObj, lParam);
	}
	else if (uMsg == WM_RBUTTONDOWN)
	{
		auto mode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);

		// 只有在绘制模式下才处理右键取消点
		if (mode == PROTOTYPEBOARD_MODE_DRAW)
		{
			// 右键取消上一个点
			_prototype_board_path_remove_last_point(hObj);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		_prototype_board_mousemove(hObj, lParam);
	}
	else if (uMsg == WM_MOUSEWHEEL)
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 32768) != 0)
		{
			float scale;
			auto zDelta = (SHORT)HIWORD(wParam);//正数往上翻，负数往下翻
			if (zDelta > 0)
			{
				//放大比例
				scale = 1.1;
			}
			else {
				scale = 1.0 / 1.1;
			}
			RECT rc;
			Ex_ObjGetRect(hObj, &rc);
			_prototype_board_calc_scale(hObj, rc.right - rc.left, rc.bottom - rc.top, scale);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_DRAW_LINE)
	{
		Ex_ObjSendMessage(hObj, PROTOTYPEBOARD_MESSAGE_MODE, 32515, PROTOTYPEBOARD_MODE_SELECT);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_LINE);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_DRAW_RECT)
	{
		Ex_ObjSendMessage(hObj, PROTOTYPEBOARD_MESSAGE_MODE, 32515, PROTOTYPEBOARD_MODE_SELECT);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_RECT);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_DRAW_ELLIPSE)
	{
		Ex_ObjSendMessage(hObj, PROTOTYPEBOARD_MESSAGE_MODE, 32515, PROTOTYPEBOARD_MODE_SELECT);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_ELLIPSE);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_DRAW_TEXT)
	{
		Ex_ObjSendMessage(hObj, PROTOTYPEBOARD_MESSAGE_MODE, 32515, PROTOTYPEBOARD_MODE_SELECT);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_TEXT);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_DRAW_IMAGE)
	{
		Ex_ObjSendMessage(hObj, PROTOTYPEBOARD_MESSAGE_MODE, 32515, PROTOTYPEBOARD_MODE_SELECT);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_IMAGE);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_MODE)
	{
		INT oldMode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);
		INT newMode = lParam;

		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_MODE, newMode);
		// 如果从绘制模式切换到其他模式，清除路径绘制状态
		if (oldMode == PROTOTYPEBOARD_MODE_DRAW && newMode != PROTOTYPEBOARD_MODE_DRAW)
		{
			_prototype_board_reset_path_drawing(hObj);
			// 额外清理：重置鼠标状态
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
		}
		// 清除选择状态（除非是从选择模式切换到其他模式）
		if (newMode != PROTOTYPEBOARD_MODE_SELECT)
		{
			_prototype_board_clear_selections(hObj);
		}
		// 重置绘制类型（除非是绘制模式）
		if (newMode != PROTOTYPEBOARD_MODE_DRAW)
		{
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		}
		if (newMode == PROTOTYPEBOARD_MODE_DRAW)
		{
			// 清除所有选中状态
			_prototype_board_clear_selections(hObj);
			// 重置路径绘制状态
			_prototype_board_reset_path_drawing(hObj);
			// 设置绘制类型
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_PATH);
			// 重置鼠标状态
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
		}

		if (newMode == PROTOTYPEBOARD_MODE_MOVE)
		{
			_prototype_board_set_cursor(hObj, 32649);// 手型光标
		}
		else if (newMode == PROTOTYPEBOARD_MODE_DRAW)
		{
			_prototype_board_set_cursor(hObj, 32515); // 十字光标
		}
		else
		{
			_prototype_board_set_cursor(hObj, (INT)wParam);
		}
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_SET_TEXT)
	{
		// 获取字体句柄和文本内容
		HEXFONT hFont = (HEXFONT)wParam;
		LPCWSTR textContent = (LPCWSTR)lParam;

		// 为所有选中的文本类型形状设置文本
		_prototype_board_set_text_to_selected(hObj, hFont, textContent);

		// 重画画布
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == PROTOTYPEBOARD_MESSAGE_SET_IMAGE)
	{
		// 获取图片句柄和是否保持宽高比
		HEXIMAGE hImage = (HEXIMAGE)wParam;
		BOOL keepAspectRatio = (BOOL)lParam;

		// 为所有选中的图片类型形状设置图片
		_prototype_board_set_image_to_selected(hObj, hImage, keepAspectRatio);

		// 重画画布
		Ex_ObjInvalidateRect(hObj, 0);
	}
	return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

void _prototype_board_set_text_to_selected(HEXOBJ hObj, HEXFONT hFont, LPCWSTR textContent)
{
	if (textContent == nullptr)
		return;

	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);

	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

		for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;

			// 只处理选中的文本类型形状
			if (ptrShape->type == ShapeType::SHAPE_TEXT && ptrShape->bSelect)
			{
				// 释放旧的文本内容内存
				if (ptrShape->textContent != nullptr)
				{
					Ex_MemFree(ptrShape->textContent);
					ptrShape->textContent = nullptr;
				}

				// 复制新的文本内容
				size_t textLen = wcslen(textContent);
				ptrShape->textContent = (WCHAR*)malloc((textLen + 1) * sizeof(WCHAR));
				if (ptrShape->textContent != nullptr)
				{
					ptrShape->textContent = StrDupW( textContent);
				}

				// 设置字体
				ptrShape->textFont = hFont;

				// 获取字体信息
				if (hFont != 0)
				{
					// 可以获取字体大小等信息
					// 例如：ptrShape->textSize = _font_getsize(hFont);
				}
			}
		}
	}
}


void _prototype_board_addtexttoshape(HEXOBJ hObj, int left, int top, int right, int bottom)
{
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_TEXT;

	// 保存实时坐标
	ptrShape->currentLeft = left;
	ptrShape->currentTop = top;
	ptrShape->currentRight = right;
	ptrShape->currentBottom = bottom;

	// 初始化文本相关字段
	ptrShape->textContent = nullptr;  // 初始无文本内容
	ptrShape->textFont = 0;           // 初始无字体
	ptrShape->textColor = ExARGB(0, 0, 0, 255);  // 默认黑色
	ptrShape->textFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;  // 默认居中
	ptrShape->textSize = 12;          // 默认12号字

	// 创建矩形路径（用于点击检测）
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);
	_path_beginfigure2(path, left + 0.5, top + 0.5);
	_path_addrect(path, left, top, right - 1, bottom - 1);
	_path_endfigure(path, TRUE);
	_path_close(path);

	EX_RECTF rc;
	_path_getbounds(path, &rc);
	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;

	// 取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	// 形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

/// <summary>
/// 为选中的图片形状设置图片
/// </summary>
void _prototype_board_set_image_to_selected(HEXOBJ hObj, HEXIMAGE hImage, BOOL keepAspectRatio)
{
	// 获取图片尺寸
	INT imageWidth = 0, imageHeight = 0;
	if (hImage != 0)
	{
		_img_getsize(hImage, &imageWidth, &imageHeight);
	}

	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);

	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

		for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;

			// 只处理选中的图片类型形状
			if (ptrShape->type == ShapeType::SHAPE_IMAGE && ptrShape->bSelect)
			{
				if (ptrShape->imageHandle != 0) 
				{
					_img_destroy(ptrShape->imageHandle);
				}
				ptrShape->imageHandle = hImage;
				ptrShape->keepAspectRatio = keepAspectRatio;

				if (hImage != 0)
				{
					// 保存图片原始尺寸
					ptrShape->imageWidth = imageWidth;
					ptrShape->imageHeight = imageHeight;

					// 如果保持宽高比，调整矩形区域
					if (keepAspectRatio && imageWidth > 0 && imageHeight > 0)
					{
						FLOAT rectWidth = ptrShape->currentRight - ptrShape->currentLeft;
						FLOAT rectHeight = ptrShape->currentBottom - ptrShape->currentTop;
						FLOAT imageRatio = (FLOAT)imageWidth / (FLOAT)imageHeight;
						FLOAT rectRatio = rectWidth / rectHeight;

						if (imageRatio > rectRatio)
						{
							// 以宽度为准，调整高度
							FLOAT newHeight = rectWidth / imageRatio;
							FLOAT centerY = (ptrShape->currentTop + ptrShape->currentBottom) / 2;
							ptrShape->currentTop = centerY - newHeight / 2;
							ptrShape->currentBottom = centerY + newHeight / 2;
						}
						else
						{
							// 以高度为准，调整宽度
							FLOAT newWidth = rectHeight * imageRatio;
							FLOAT centerX = (ptrShape->currentLeft + ptrShape->currentRight) / 2;
							ptrShape->currentLeft = centerX - newWidth / 2;
							ptrShape->currentRight = centerX + newWidth / 2;
						}

						// 更新路径
						_path_destroy(ptrShape->path);
						HEXPATH newPath;
						_path_create(1, &newPath);
						_path_open(newPath);
						_path_beginfigure2(newPath, ptrShape->currentLeft + 0.5, ptrShape->currentTop + 0.5);
						_path_addrect(newPath, ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight - 1, ptrShape->currentBottom - 1);
						_path_endfigure(newPath, TRUE);
						_path_close(newPath);
						ptrShape->path = newPath;
						_path_getbounds(newPath, &ptrShape->boundingRect);
					}
				}
			}
		}
	}
}

// 基于区域中心点更新选中图形的位置
void _prototype_board_update_selected_shapes_position(HEXOBJ hObj, FLOAT mouseWorldX, FLOAT mouseWorldY)
{
	INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
	LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);

	if (selectedCount == 0 || selectedShapes == 0)
		return;

	// 获取中心点和偏移
	FLOAT centerX, centerY, offsetX, offsetY;
	auto centerXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X);
	auto centerYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y);
	auto offsetXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X);
	auto offsetYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y);

	if (!centerXPtr || !centerYPtr || !offsetXPtr || !offsetYPtr)
		return;

	memmove(&centerX, centerXPtr, sizeof(FLOAT));
	memmove(&centerY, centerYPtr, sizeof(FLOAT));
	memmove(&offsetX, offsetXPtr, sizeof(FLOAT));
	memmove(&offsetY, offsetYPtr, sizeof(FLOAT));

	// 计算新的中心点（鼠标位置减去偏移）
	FLOAT newCenterX = mouseWorldX - offsetX;
	FLOAT newCenterY = mouseWorldY - offsetY;

	// 计算整体偏移量
	FLOAT totalOffsetX = newCenterX - centerX;
	FLOAT totalOffsetY = newCenterY - centerY;

	// 更新中心点
	centerX = newCenterX;
	centerY = newCenterY;
	memmove(centerXPtr, &centerX, sizeof(FLOAT));
	memmove(centerYPtr, &centerY, sizeof(FLOAT));

	// 更新所有选中的图形
	for (INT i = 0; i < selectedCount; i++)
	{
		EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)selectedShapes[i];

		// 根据图形类型更新实时坐标并创建新路径
		if (ptrShape->type == ShapeType::SHAPE_RECTANGLE)
		{
			// 更新矩形实时坐标
			FLOAT newLeft = ptrShape->currentLeft + totalOffsetX;
			FLOAT newTop = ptrShape->currentTop + totalOffsetY;
			FLOAT newRight = ptrShape->currentRight + totalOffsetX;
			FLOAT newBottom = ptrShape->currentBottom + totalOffsetY;

			// 更新实时坐标
			ptrShape->currentLeft = newLeft;
			ptrShape->currentTop = newTop;
			ptrShape->currentRight = newRight;
			ptrShape->currentBottom = newBottom;

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);
			_path_beginfigure2(newPath, newLeft + 0.5, newTop + 0.5);
			_path_addrect(newPath, newLeft, newTop, newRight - 1, newBottom - 1);
			_path_endfigure(newPath, TRUE);
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
		}
		else if (ptrShape->type == ShapeType::SHAPE_ELLIPSE)
		{
			// 更新椭圆实时坐标
			FLOAT newCenterX = ptrShape->currentCenterX + totalOffsetX;
			FLOAT newCenterY = ptrShape->currentCenterY + totalOffsetY;

			// 更新实时坐标
			ptrShape->currentCenterX = newCenterX;
			ptrShape->currentCenterY = newCenterY;

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);
			_path_beginfigure2(newPath, newCenterX - ptrShape->currentRadiusX + 0.5, newCenterY + 0.5);
			_path_addarc(newPath, newCenterX - ptrShape->currentRadiusX + 0.5, newCenterY + 0.5,
				newCenterX + 0.5, newCenterY - ptrShape->currentRadiusY + 0.5,
				ptrShape->currentRadiusX, ptrShape->currentRadiusY, TRUE);
			_path_addarc(newPath, newCenterX + 0.5, newCenterY - ptrShape->currentRadiusY + 0.5,
				newCenterX + ptrShape->currentRadiusX + 0.5, newCenterY + 0.5,
				ptrShape->currentRadiusX, ptrShape->currentRadiusY, TRUE);
			_path_addarc(newPath, newCenterX + ptrShape->currentRadiusX + 0.5, newCenterY + 0.5,
				newCenterX + 0.5, newCenterY + ptrShape->currentRadiusY + 0.5,
				ptrShape->currentRadiusX, ptrShape->currentRadiusY, TRUE);
			_path_addarc(newPath, newCenterX + 0.5, newCenterY + ptrShape->currentRadiusY + 0.5,
				newCenterX - ptrShape->currentRadiusX + 0.5, newCenterY + 0.5,
				ptrShape->currentRadiusX, ptrShape->currentRadiusY, TRUE);
			_path_endfigure(newPath, TRUE);
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
		}
		else if (ptrShape->type == ShapeType::SHAPE_LINE)
		{
			// 更新直线实时坐标
			FLOAT newX1 = ptrShape->currentX1 + totalOffsetX;
			FLOAT newY1 = ptrShape->currentY1 + totalOffsetY;
			FLOAT newX2 = ptrShape->currentX2 + totalOffsetX;
			FLOAT newY2 = ptrShape->currentY2 + totalOffsetY;

			// 更新实时坐标
			ptrShape->currentX1 = newX1;
			ptrShape->currentY1 = newY1;
			ptrShape->currentX2 = newX2;
			ptrShape->currentY2 = newY2;

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);
			_path_beginfigure2(newPath, newX1 + 0.5, newY1 + 0.5);
			_path_addline(newPath, newX1, newY1, newX2 + 0.5, newY2 + 0.5);
			_path_endfigure(newPath, TRUE);
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
		}
		else if (ptrShape->type == ShapeType::SHAPE_TEXT)
		{
			// 更新文本实时坐标
			FLOAT newLeft = ptrShape->currentLeft + totalOffsetX;
			FLOAT newTop = ptrShape->currentTop + totalOffsetY;
			FLOAT newRight = ptrShape->currentRight + totalOffsetX;
			FLOAT newBottom = ptrShape->currentBottom + totalOffsetY;

			// 更新实时坐标
			ptrShape->currentLeft = newLeft;
			ptrShape->currentTop = newTop;
			ptrShape->currentRight = newRight;
			ptrShape->currentBottom = newBottom;

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);
			_path_beginfigure2(newPath, newLeft + 0.5, newTop + 0.5);
			_path_addrect(newPath, newLeft, newTop, newRight - 1, newBottom - 1);
			_path_endfigure(newPath, TRUE);
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
		}
		else if (ptrShape->type == ShapeType::SHAPE_IMAGE)
		{
			// 更新图片实时坐标
			FLOAT newLeft = ptrShape->currentLeft + totalOffsetX;
			FLOAT newTop = ptrShape->currentTop + totalOffsetY;
			FLOAT newRight = ptrShape->currentRight + totalOffsetX;
			FLOAT newBottom = ptrShape->currentBottom + totalOffsetY;

			// 更新实时坐标
			ptrShape->currentLeft = newLeft;
			ptrShape->currentTop = newTop;
			ptrShape->currentRight = newRight;
			ptrShape->currentBottom = newBottom;

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);
			_path_beginfigure2(newPath, newLeft + 0.5, newTop + 0.5);
			_path_addrect(newPath, newLeft, newTop, newRight - 1, newBottom - 1);
			_path_endfigure(newPath, TRUE);
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
		}
		else if (ptrShape->type == ShapeType::SHAPE_PATH)
		{
			// 更新路径所有点
			for (INT i = 0; i < ptrShape->pathPointsCount; i++)
			{
				ptrShape->pathPoints[i].x += totalOffsetX;
				ptrShape->pathPoints[i].y += totalOffsetY;
			}

			// 销毁旧路径
			_path_destroy(ptrShape->path);

			// 创建新路径
			HEXPATH newPath;
			_path_create(1, &newPath);
			_path_open(newPath);

			if (ptrShape->pathPointsCount > 0)
			{
				_path_beginfigure2(newPath, ptrShape->pathPoints[0].x, ptrShape->pathPoints[0].y);

				for (INT i = 1; i < ptrShape->pathPointsCount; i++)
				{
					_path_addline(newPath,
						ptrShape->pathPoints[i - 1].x, ptrShape->pathPoints[i - 1].y,
						ptrShape->pathPoints[i].x, ptrShape->pathPoints[i].y);
				}

				// 如果是闭合路径，连接到起始点
				if (ptrShape->pathClosed)
				{
					_path_addline(newPath,
						ptrShape->pathPoints[ptrShape->pathPointsCount - 1].x,
						ptrShape->pathPoints[ptrShape->pathPointsCount - 1].y,
						ptrShape->pathPoints[0].x, ptrShape->pathPoints[0].y);
				}

				_path_endfigure(newPath, TRUE);
			}
			_path_close(newPath);

			// 更新路径和包围框
			ptrShape->path = newPath;
			_path_getbounds(newPath, &ptrShape->boundingRect);
			}
	}

	// 更新选中区域
	_prototype_board_update_selected_region(hObj);
}

void _prototype_board_mousemove(HEXOBJ hObj, LPARAM lParam)
{
	auto mode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);
	auto hold = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD);
	auto draggingSelected = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED);
	auto resizeMode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE);
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	// 保存当前鼠标位置
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_X, pt.x);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_Y, pt.y);

	auto canvasOffsetX = _prototype_board_get_canvasoffsetx(hObj);
	auto canvasOffsetY = _prototype_board_get_canvasoffsety(hObj);
	auto scale = _prototype_board_get_scalefactor(hObj);
	auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
	FLOAT worldX = (canvasOffsetX + pt.x) / (scale / scaleinit);
	FLOAT worldY = (canvasOffsetY + pt.y) / (scale / scaleinit);

	// 绘制模式下的处理
	if (mode == PROTOTYPEBOARD_MODE_DRAW)
	{
		// 只有在路径绘制过程中才更新临时点
		BOOL pathDrawing = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING);
		BOOL pathClosed = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED);

		if (pathDrawing && !pathClosed)
		{
			// 更新临时点
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X, (LONG_PTR)worldX);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y, (LONG_PTR)worldY);
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return;  // 绘制模式下不处理其他操作
	}

	// 检查是否处于调整大小模式 - 仅在选择模式下
	if (resizeMode == 1 && hold == 1) {// && mode == PROTOTYPEBOARD_MODE_SELECT
		// 获取当前鼠标位置和上一次鼠标位置
		FLOAT startX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X);
		FLOAT startY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y);
		INT corner = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER);

		FLOAT deltaX = worldX - startX;
		FLOAT deltaY = worldY - startY;

		// 获取单个选中的形状
		INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
		LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);

		if (selectedCount == 1 && selectedShapes != 0) {
			EX_PROTOTYPEBOARD_SHAPE* shape = (EX_PROTOTYPEBOARD_SHAPE*)selectedShapes[0];
			_prototype_board_resize_shape(hObj, shape, corner, deltaX, deltaY);
			Ex_ObjInvalidateRect(hObj, 0);
		}
		return;
	}

	// 检查是否处于拖动选中图形模式 - 仅在选择模式下
	if (draggingSelected == 1 && hold == 1 && mode == PROTOTYPEBOARD_MODE_SELECT)
	{
		// 获取当前鼠标位置和上一次鼠标位置
		FLOAT lastWorldX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X);
		FLOAT lastWorldY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y);

		// 如果是第一次移动，记录当前位置
		if (lastWorldX == 0 && lastWorldY == 0)
		{
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, (LONG_PTR)worldX);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, (LONG_PTR)worldY);
			return;
		}

		// 计算增量偏移量
		FLOAT offsetX = worldX - lastWorldX;
		FLOAT offsetY = worldY - lastWorldY;

		// 更新选中图形的位置，使区域中心点跟随鼠标
		_prototype_board_update_selected_shapes_position(hObj, worldX, worldY);

		// 更新上一次位置
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, (LONG_PTR)worldX);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, (LONG_PTR)worldY);

		Ex_ObjInvalidateRect(hObj, 0);
		return;
	}
	else
	{
		// 重置上一次位置
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, 0);
	}
	// 移动模式下的处理
	if (mode == PROTOTYPEBOARD_MODE_MOVE)
	{
		if (hold == 1)//按住画布拖动
		{
			//拖动画布
			auto startX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X);
			auto startY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y);
			auto offsetX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_X);
			auto offsetY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_Y);
			auto moveX = (startX - pt.x);
			auto moveY = (startY - pt.y);
			_prototype_board_set_canvasoffsetx(hObj, offsetX + moveX);
			_prototype_board_set_canvasoffsety(hObj, offsetY + moveY);
			Ex_ObjInvalidateRect(hObj, 0);
		}
	}
	else if (mode == PROTOTYPEBOARD_MODE_SELECT)
	{
		if (hold == 1)//按住画布绘制形状
		{
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_X, pt.x);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_Y, pt.y);
			INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);
			// 如果正在拖动或调整大小，不更新临时画布
			if (!draggingSelected && !resizeMode)
			{
				// 如果已经有特定的绘制类型（直线、矩形等），直接更新临时画布
				if (drawType >= PROTOTYPEBOARD_DRAW_TYPE_LINE && drawType <= PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
				{
					Ex_ObjInvalidateRect(hObj, 0);
				}
				// 否则设置为选择类型，绘制绿色半透明矩形
				else if (drawType == -1 || drawType == PROTOTYPEBOARD_DRAW_TYPE_SELECT)
				{
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, PROTOTYPEBOARD_DRAW_TYPE_SELECT);
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
		}
		else if (hold == 0)
		{
			// 首先检查鼠标是否在选中区域内
			BOOL inSelectedRegion = FALSE;
			auto hRgnSelect = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
			if (hRgnSelect != 0)
			{
				// 获取选中区域边界并扩大检测范围
				EX_RECTF bounds;
				_rgn_getbounds(hRgnSelect, &bounds);

				// 扩大检测范围，确保包围盒边缘也能检测到
				FLOAT expandAmount = 5.0f;
				if (worldX >= bounds.left - expandAmount && worldX <= bounds.right + expandAmount &&
					worldY >= bounds.top - expandAmount && worldY <= bounds.bottom + expandAmount)
				{
					// 鼠标在扩大后的边界内
					inSelectedRegion = TRUE;
				}
			}

			// 如果鼠标在选中区域内，不进行悬停检测
			if (!inSelectedRegion)
			{
				//寻找点燃最高层路径,没有则为-1
				INT maxShapeLevel = -1;
				INT hoverLayerIndex = -1;
				INT hoverShapeIndex = -1;
				auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
				for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
				{
					size_t layerValue = 0;
					memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
					EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;
					for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
					{
						size_t shapeValue = 0;
						memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
						EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;
						// 对于椭圆，需要特别处理：检查鼠标是否在椭圆内部或其边界附近
						if (ptrShape->type == ShapeType::SHAPE_ELLIPSE)
						{
							// 计算鼠标点到椭圆中心的距离（归一化）
							FLOAT dx = worldX - ptrShape->currentCenterX;
							FLOAT dy = worldY - ptrShape->currentCenterY;
							FLOAT rx = ptrShape->currentRadiusX;
							FLOAT ry = ptrShape->currentRadiusY;

							// 椭圆方程: (x^2 / a^2) + (y^2 / b^2) <= 1
							// 增加容差，使边界附近也能被检测到
							FLOAT tolerance = 0.2f; // 20%的容差
							FLOAT normalizedX = dx / (rx + tolerance);
							FLOAT normalizedY = dy / (ry + tolerance);

							if (normalizedX * normalizedX + normalizedY * normalizedY <= 1.0f)
							{
								if (ptrShape->shapeLevel > maxShapeLevel)
								{
									maxShapeLevel = ptrShape->shapeLevel;
									hoverLayerIndex = layerIndex;
									hoverShapeIndex = shapeIndex;
								}
							}
						}
						else if (_path_hittest(ptrShape->path, worldX, worldY))//加入偏移缩放
						{
							if (ptrShape->shapeLevel > maxShapeLevel)
							{
								maxShapeLevel = ptrShape->shapeLevel;
								hoverLayerIndex = layerIndex;
								hoverShapeIndex = shapeIndex;
							}
						}
					}
				}
				INT oldHoverLayerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX);
				INT oldHoverShapeIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX);
				if (oldHoverLayerIndex != hoverLayerIndex || oldHoverShapeIndex != hoverShapeIndex)
				{
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX, hoverLayerIndex);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX, hoverShapeIndex);
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
			else
			{
				// 鼠标在选中区域内，清除悬停状态
				INT oldHoverLayerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX);
				INT oldHoverShapeIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX);
				if (oldHoverLayerIndex != -1 || oldHoverShapeIndex != -1)
				{
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX, -1);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX, -1);
					Ex_ObjInvalidateRect(hObj, 0);
				}
			}
		}
	}
}

void _prototype_board_calc_selectregion(HEXOBJ hObj, int left, int top, int right, int bottom)
{
	auto canvasOffsetX = _prototype_board_get_canvasoffsetx(hObj);
	auto canvasOffsetY = _prototype_board_get_canvasoffsety(hObj);
	auto scale = _prototype_board_get_scalefactor(hObj);
	auto scaleinit = _prototype_board_get_scalefactorinit(hObj);

	HEXRGN hRgn = _rgn_createfromrect((left + canvasOffsetX) / (scale / scaleinit), 
		(top + canvasOffsetY) / (scale / scaleinit), 
		(right + canvasOffsetX) / (scale / scaleinit), 
		(bottom + canvasOffsetY) / (scale / scaleinit));
	// 创建一个扩大后的选择区域，用于检测直线
	FLOAT expandAmount = 2.0f; // 扩大2个单位
	HEXRGN expandedRgn = _rgn_createfromrect(
		(left + canvasOffsetX) / (scale / scaleinit) - expandAmount,
		(top + canvasOffsetY) / (scale / scaleinit) - expandAmount,
		(right + canvasOffsetX) / (scale / scaleinit) + expandAmount,
		(bottom + canvasOffsetY) / (scale / scaleinit) + expandAmount
	);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	INT totalCount = 0;
	HEXRGN hRgnNew = 0;
	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;
		for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;

			HEXRGN hRgn2 = _rgn_createfrompath(ptrShape->path);
			INT retRelation = -1;
			_rgn_hittest2(hRgn, hRgn2, &retRelation);
			// 如果是直线，使用扩大后的区域重新检测
			if (retRelation <= 1 || retRelation >= 5)
			{
				if (ptrShape->type == ShapeType::SHAPE_LINE)
				{
					_rgn_hittest2(expandedRgn, hRgn2, &retRelation);
				}
			}
			//计算选中区域,如果是属于，包含，相交就加入
			if (retRelation > 1 && retRelation < 5)
			{
				ptrShape->bSelect = TRUE;
				if (totalCount == 0)
				{
					hRgnNew = _rgn_createfrompath(ptrShape->path);
				}
				else {
					auto hRgnNew2 = _rgn_combine(hRgn2, hRgnNew, REGION_COMBINE_UNION, 0, 0);
					_rgn_destroy(hRgnNew);
					hRgnNew = hRgnNew2;
				}
				totalCount += 1;
			}
			else {
				ptrShape->bSelect = FALSE;
			}
		}
		if (hRgnNew != 0)
		{
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, (LONG_PTR)hRgnNew);
		}
		else {
			auto oldhRgn = (HEXRGN)Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, 0);
			_rgn_destroy(oldhRgn);
		}
	}
	
	// 清理临时区域
	_rgn_destroy(hRgn);
	_rgn_destroy(expandedRgn);
	// 清除调整大小模式
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);
	// 收集选中图形的指针（用于控制点显示和调整）
	_prototype_board_collect_selected_shapes(hObj);
}


void _prototype_board_leftbuttondown(HEXOBJ hObj, LPARAM lParam)
{
	auto mode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);

	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, pt.x);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, pt.y);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_X, (INT)_prototype_board_get_canvasoffsetx(hObj));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_Y, (INT)_prototype_board_get_canvasoffsety(hObj));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 1);

	auto canvasOffsetX = _prototype_board_get_canvasoffsetx(hObj);
	auto canvasOffsetY = _prototype_board_get_canvasoffsety(hObj);
	auto scale = _prototype_board_get_scalefactor(hObj);
	auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
	FLOAT worldX = (canvasOffsetX + pt.x) / (scale / scaleinit);
	FLOAT worldY = (canvasOffsetY + pt.y) / (scale / scaleinit);
	// 保存世界坐标（用于绘制形状）
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_X, (LONG_PTR)worldX);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_Y, (LONG_PTR)worldY);
	// 保存当前鼠标位置
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_X, pt.x);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_Y, pt.y);

	// 绘制模式下的处理
	if (mode == PROTOTYPEBOARD_MODE_DRAW)
	{
		// 添加路径点
		_prototype_board_path_add_point(hObj, worldX, worldY);
		Ex_ObjInvalidateRect(hObj, 0);
		return;  // 绘制模式下不处理其他操作
	}

	// 检查是否点击了控制点（只有当选中单个形状时）- 仅在选择模式下
	if (mode == PROTOTYPEBOARD_MODE_SELECT)
	{
		// 首先检查是否点击了选中区域
		HEXRGN hRgnSelect = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
		BOOL inSelectedRegion = FALSE;
		if (hRgnSelect != 0)
		{
			inSelectedRegion = _rgn_hittest(hRgnSelect, worldX, worldY);

			// 如果不在选中区域内，检查是否在包围盒的扩展范围内（用于检测椭圆等形状）
			if (!inSelectedRegion)
			{
				EX_RECTF bounds;
				_rgn_getbounds(hRgnSelect, &bounds);

				// 扩大检测范围，确保包围盒也能被检测到
				FLOAT expandAmount = 10.0f;
				if (worldX >= bounds.left - expandAmount && worldX <= bounds.right + expandAmount &&
					worldY >= bounds.top - expandAmount && worldY <= bounds.bottom + expandAmount)
				{
					// 鼠标在扩大后的包围盒内，检查是否是椭圆等特殊形状
					INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
					if (selectedCount == 1)
					{
						// 对于单个选中的形状，即使不在路径内，但在包围盒内，也视为在选中区域内
						inSelectedRegion = TRUE;
					}
				}
			}
		}

		INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);

		// 如果有特定的绘制类型（矩形、直线等），直接开始绘制，不检查点击形状
		if (drawType >= PROTOTYPEBOARD_DRAW_TYPE_LINE && drawType <= PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
		{
			// 开始绘制形状，不需要其他处理
		}
		else if (inSelectedRegion)
		{
			// 鼠标在选中区域内，不进行悬停检测
			// 直接处理选中区域的点击事件（控制点检测、拖动等）
			// 这里不需要设置绘制类型，这样就不会绘制选择框
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);

			// 检查是否点击了控制点
			INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
			LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);

			if (selectedCount == 1 && selectedShapes != 0) {
				EX_PROTOTYPEBOARD_SHAPE* singleShape = (EX_PROTOTYPEBOARD_SHAPE*)selectedShapes[0];

				ResizeCorner corner = _prototype_board_hittest_resize_handle(hObj, worldX, worldY, singleShape);
				if (corner != CORNER_NONE) {

					// 进入调整大小模式
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 1);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, corner);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X, (LONG_PTR)worldX);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y, (LONG_PTR)worldY);

					// 保存形状的初始状态
					if (singleShape->type == ShapeType::SHAPE_RECTANGLE ||
						singleShape->type == ShapeType::SHAPE_TEXT ||
						singleShape->type == ShapeType::SHAPE_IMAGE) {
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_LEFT, (LONG_PTR)singleShape->currentLeft);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_TOP, (LONG_PTR)singleShape->currentTop);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT, (LONG_PTR)singleShape->currentRight);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM, (LONG_PTR)singleShape->currentBottom);
					}
					else if (singleShape->type == ShapeType::SHAPE_ELLIPSE) {
						// 保存椭圆中心点和半径
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERX, (LONG_PTR)singleShape->currentCenterX);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERY, (LONG_PTR)singleShape->currentCenterY);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSX, (LONG_PTR)singleShape->currentRadiusX);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSY, (LONG_PTR)singleShape->currentRadiusY);

						// 同时保存椭圆的包围盒（用于调整）
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_LEFT, (LONG_PTR)(singleShape->currentCenterX - singleShape->currentRadiusX));
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_TOP, (LONG_PTR)(singleShape->currentCenterY - singleShape->currentRadiusY));
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT, (LONG_PTR)(singleShape->currentCenterX + singleShape->currentRadiusX));
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM, (LONG_PTR)(singleShape->currentCenterY + singleShape->currentRadiusY));
					}
					else if (singleShape->type == ShapeType::SHAPE_LINE) {
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X1, (LONG_PTR)singleShape->currentX1);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y1, (LONG_PTR)singleShape->currentY1);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X2, (LONG_PTR)singleShape->currentX2);
						Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y2, (LONG_PTR)singleShape->currentY2);
					}

					// 立即重绘显示控制点
					Ex_ObjInvalidateRect(hObj, 0);
					return;
				}
			}

			// 检查是否按下了Shift键
			BOOL bShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;

			// 如果按下Shift键且有选中区域，开始拖动
			HEXRGN hRgnSelect = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
			if (bShiftPressed && hRgnSelect != 0) {
				// 检查鼠标是否在选中区域内（包括边界）或者在一个扩大的矩形区域内
				BOOL bInRegion = _rgn_hittest(hRgnSelect, worldX, worldY);
				if (!bInRegion) {
					// 如果不在区域内，检查是否在扩大后的矩形区域内
					EX_RECTF unionRect;
					_rgn_getbounds(hRgnSelect, &unionRect);

					FLOAT tolerance = 10.0f; // 扩大10个单位的容差
					EX_RECTF expandedRect = {
						unionRect.left - tolerance,
						unionRect.top - tolerance,
						unionRect.right + tolerance,
						unionRect.bottom + tolerance
					};

					bInRegion = (worldX >= expandedRect.left && worldX <= expandedRect.right &&
						worldY >= expandedRect.top && worldY <= expandedRect.bottom);
				}

				// 如果鼠标在选中区域内，开始拖动
				if (bInRegion) {
					// 开始拖动选中图形
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED, 1);

					// 清除调整大小模式
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);

					// 收集选中图形的指针
					_prototype_board_collect_selected_shapes(hObj);

					// 计算选中区域的整体包围盒
					EX_RECTF unionRect;
					_rgn_getbounds(hRgnSelect, &unionRect);

					// 计算包围盒中心点
					FLOAT centerX = (unionRect.left + unionRect.right) / 2.0f;
					FLOAT centerY = (unionRect.top + unionRect.bottom) / 2.0f;

					// 存储鼠标点击点相对于中心点的偏移
					FLOAT offsetX = worldX - centerX;
					FLOAT offsetY = worldY - centerY;

					// 存储中心点和偏移
					auto centerXPtr = malloc(sizeof(FLOAT));
					auto centerYPtr = malloc(sizeof(FLOAT));
					auto offsetXPtr = malloc(sizeof(FLOAT));
					auto offsetYPtr = malloc(sizeof(FLOAT));
					memmove(centerXPtr, &centerX, sizeof(FLOAT));
					memmove(centerYPtr, &centerY, sizeof(FLOAT));
					memmove(offsetXPtr, &offsetX, sizeof(FLOAT));
					memmove(offsetYPtr, &offsetY, sizeof(FLOAT));

					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X, (LONG_PTR)centerXPtr);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y, (LONG_PTR)centerYPtr);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X, (LONG_PTR)offsetXPtr);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y, (LONG_PTR)offsetYPtr);

					// 记录初始拖拽位置
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, (LONG_PTR)worldX);
					Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, (LONG_PTR)worldY);

					return;
				}
			}

			// 如果没有按下Shift键或不在选中区域内，清除拖动和调整大小模式
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);
			_prototype_board_free_selected_shapes(hObj);

			// 更新悬停状态
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX, -1);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX, -1);

			// 不需要清除选中状态，因为点击的是选中区域内的形状
			Ex_ObjInvalidateRect(hObj, 0);
			return;

		}
	}
	else if (mode == PROTOTYPEBOARD_MODE_MOVE)
	{
		// 移动模式下只处理画布拖动，不处理形状选择
		// 已经设置的变量会在鼠标移动事件中处理
	}
}

void _prototype_board_leftbuttonup(HEXOBJ hObj, LPARAM lParam)
{
	// 恢复鼠标光标
	auto mode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);
	// 如果不是绘制模式，清除路径绘制状态
	if (mode != PROTOTYPEBOARD_MODE_DRAW)
	{
		// 清除路径绘制状态，防止影响其他绘图操作
		_prototype_board_reset_path_drawing(hObj);
	}
	// 绘制模式下的处理
	if (mode == PROTOTYPEBOARD_MODE_DRAW)
	{
		// 绘制模式下鼠标弹起不做特殊处理
		// 路径点的添加已经在左键按下时处理
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
		return;
	}
	// 恢复鼠标光标
	if (mode == PROTOTYPEBOARD_MODE_SELECT)
	{
		_prototype_board_set_cursor(hObj, 0);
	}
	// 结束调整大小模式
	if (Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE) == 1) {
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);
		// 清理调整大小的起始位置
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_LEFT, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_TOP, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERX, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERY, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSX, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSY, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X1, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y1, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X2, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y2, 0);
		Ex_ObjInvalidateRect(hObj, 0); // 重绘以更新控制点
	}

	// 结束拖动选中图形
	if (Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED) == 1)
	{
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED, 0);
		// 释放存储的中心点和偏移
		auto centerXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X);
		auto centerYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y);
		auto offsetXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X);
		auto offsetYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y);

		if (centerXPtr) free(centerXPtr);
		if (centerYPtr) free(centerYPtr);
		if (offsetXPtr) free(offsetXPtr);
		if (offsetYPtr) free(offsetYPtr);

		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y, 0);

		// 重置上一次位置
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, 0);

		// 释放选中图形指针数组
		_prototype_board_free_selected_shapes(hObj);
		Ex_ObjInvalidateRect(hObj, 0); // 重绘以更新控制点
	}

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };

	auto canvasOffsetX = _prototype_board_get_canvasoffsetx(hObj);
	auto canvasOffsetY = _prototype_board_get_canvasoffsety(hObj);

	auto scale = _prototype_board_get_scalefactor(hObj);
	auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
	FLOAT worldEndX = (canvasOffsetX + pt.x) / (scale / scaleinit);
	FLOAT worldEndY = (canvasOffsetY + pt.y) / (scale / scaleinit);

	// 获取起始点的世界坐标
	FLOAT worldStartX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_X);
	FLOAT worldStartY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_Y);

	INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);

	// 在开始绘制新形状前，再次确认清除路径绘制状态
	if (drawType != -1 && drawType != PROTOTYPEBOARD_DRAW_TYPE_PATH)
	{
		_prototype_board_reset_path_drawing(hObj);
	}

	if (drawType == PROTOTYPEBOARD_DRAW_TYPE_LINE)
	{
		// 使用世界坐标创建直线
		_prototype_board_addlinetoshape(hObj, worldStartX, worldStartY, worldEndX, worldEndY);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_RECT)
	{
		// 使用世界坐标，避免重复转换
		FLOAT left = min(worldStartX, worldEndX);
		FLOAT top = min(worldStartY, worldEndY);
		FLOAT right = max(worldStartX, worldEndX);
		FLOAT bottom = max(worldStartY, worldEndY);

		_prototype_board_addrecttoshape(hObj, left, top, right, bottom);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_ELLIPSE)
	{
		FLOAT radiusX = abs(worldEndX - worldStartX) / 2;
		FLOAT radiusY = abs(worldEndY - worldStartY) / 2;

		// 计算中心点（世界坐标）
		FLOAT centerX = (worldStartX + worldEndX) / 2;
		FLOAT centerY = (worldStartY + worldEndY) / 2;

		_prototype_board_addellipsetoshape(hObj, centerX, centerY, radiusX, radiusY);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_TEXT)
	{
		// 使用世界坐标创建文本形状
		FLOAT left = min(worldStartX, worldEndX);
		FLOAT top = min(worldStartY, worldEndY);
		FLOAT right = max(worldStartX, worldEndX);
		FLOAT bottom = max(worldStartY, worldEndY);

		_prototype_board_addtexttoshape(hObj, left, top, right, bottom);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
	{
		// 使用世界坐标创建图片形状
		FLOAT left = min(worldStartX, worldEndX);
		FLOAT top = min(worldStartY, worldEndY);
		FLOAT right = max(worldStartX, worldEndX);
		FLOAT bottom = max(worldStartY, worldEndY);

		_prototype_board_addimagetoshape(hObj, left, top, right, bottom);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_SELECT)
	{
		// 计算选择区域（屏幕坐标）
		INT startX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X);
		INT startY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y);

		//取消绘制选中区域
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
		_prototype_board_calc_selectregion(hObj, startX, startY, pt.x, pt.y);
		Ex_ObjInvalidateRect(hObj, 0);
	}

	// 释放鼠标状态
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_Y, 0);

	// 确保重绘画布
	Ex_ObjInvalidateRect(hObj, 0);
}

INT _prototype_board_getshapecount(HEXOBJ hObj)
{
	int total = 0;
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;
		total += ptrLayer->shapesCount;
	}
	return total;
}

void _prototype_board_addimagetoshape(HEXOBJ hObj, int left, int top, int right, int bottom)
{
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_IMAGE;

	// 保存实时坐标
	ptrShape->currentLeft = left;
	ptrShape->currentTop = top;
	ptrShape->currentRight = right;
	ptrShape->currentBottom = bottom;

	// 初始化图片相关字段
	ptrShape->imageHandle = 0;
	ptrShape->imageWidth = 0;
	ptrShape->imageHeight = 0;
	ptrShape->keepAspectRatio = TRUE; // 默认保持宽高比

	// 创建矩形路径（用于点击检测）
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);
	_path_beginfigure2(path, left + 0.5, top + 0.5);
	_path_addrect(path, left, top, right - 1, bottom - 1);
	_path_endfigure(path, TRUE);
	_path_close(path);

	EX_RECTF rc;
	_path_getbounds(path, &rc);
	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;

	// 取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	// 形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

void _prototype_board_set_cursor(HEXOBJ hObj, int value)
{
	LPCWSTR CursorName = MAKEINTRESOURCE(value);
	HCURSOR hCursor = LoadCursorW(0, CursorName);
	Ex_ObjSetLong(hObj, OBJECT_LONG_CURSOR, (LONG_PTR)hCursor);
}

void _prototype_board_calc_size(HEXOBJ hObj, int width, int height)
{
	float canvasHeight;
	float canvasWidth;
	float scaleFactor;
	if (width >= height)
	{
		canvasHeight = height - 2 * 20 - 20;
		scaleFactor = canvasHeight / 3;
		canvasWidth = scaleFactor * 4;
		float offsetX = -((width - 20 - canvasWidth) / 2 + 20);
		_prototype_board_set_canvasoffsety(hObj, -40);
		_prototype_board_set_canvasoffsetx(hObj, offsetX);
	}
	else {
		canvasWidth = width - 2 * 20 - 20;
		scaleFactor = canvasWidth / 4;
		canvasHeight = scaleFactor * 3;
		float offsetY = -((height - 20 - canvasHeight) / 2 + 20);
		_prototype_board_set_canvasoffsetx(hObj, -40);
		_prototype_board_set_canvasoffsety(hObj, offsetY);
	}

	_prototype_board_set_scalefactor(hObj, scaleFactor);
	_prototype_board_set_scalefactorinit(hObj, scaleFactor);
	_prototype_board_set_canvaswidth(hObj, canvasWidth);
	_prototype_board_set_canvasheight(hObj, canvasHeight);
}

void _prototype_board_calc_scale(HEXOBJ hObj, int width, int height, float scale)
{
	float canvasHeight = _prototype_board_get_canvasheight(hObj);
	float canvasWidth = _prototype_board_get_canvaswidth(hObj);
	float scaleFactor = _prototype_board_get_scalefactor(hObj);

	scaleFactor = scaleFactor * scale;
	canvasHeight = canvasHeight * scale;
	canvasWidth = canvasWidth * scale;

	float offsetX = -((width - 20 - canvasWidth) / 2 + 20);
	_prototype_board_set_canvasoffsetx(hObj, offsetX);

	float offsetY = -((height - 20 - canvasHeight) / 2 + 20);
	_prototype_board_set_canvasoffsety(hObj, offsetY);
	_prototype_board_set_scalefactor(hObj, scaleFactor);

	_prototype_board_set_canvaswidth(hObj, canvasWidth);
	_prototype_board_set_canvasheight(hObj, canvasHeight);
}

void _prototype_board_addellipsetoshape(HEXOBJ hObj, int centerX, int centerY, int radiusX, int radiusY)
{
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_ELLIPSE;

	// 保存实时坐标
	ptrShape->currentCenterX = centerX;
	ptrShape->currentCenterY = centerY;
	ptrShape->currentRadiusX = radiusX;
	ptrShape->currentRadiusY = radiusY;

	// 创建椭圆路径
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);
	_path_beginfigure2(path, centerX - radiusX + 0.5, centerY + 0.5);
	_path_addarc(path, centerX - radiusX + 0.5, centerY + 0.5,
		centerX + 0.5, centerY - radiusY + 0.5,
		radiusX, radiusY, TRUE);
	_path_addarc(path, centerX + 0.5, centerY - radiusY + 0.5,
		centerX + radiusX + 0.5, centerY + 0.5,
		radiusX, radiusY, TRUE);
	_path_addarc(path, centerX + radiusX + 0.5, centerY + 0.5,
		centerX + 0.5, centerY + radiusY + 0.5,
		radiusX, radiusY, TRUE);
	_path_addarc(path, centerX + 0.5, centerY + radiusY + 0.5,
		centerX - radiusX + 0.5, centerY + 0.5,
		radiusX, radiusY, TRUE);
	_path_endfigure(path, TRUE);
	_path_close(path);

	// 获取路径的边界
	EX_RECTF rc;
	_path_getbounds(path, &rc);

	// 确保边界正确设置
	ptrShape->currentLeft = rc.left;
	ptrShape->currentTop = rc.top;
	ptrShape->currentRight = rc.right;
	ptrShape->currentBottom = rc.bottom;

	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;

	// 取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	// 形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

void _prototype_board_addlinetoshape(HEXOBJ hObj, int x1, int y1, int x2, int y2)
{
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_LINE;
	// 保存实时坐标
	ptrShape->currentX1 = x1;
	ptrShape->currentY1 = y1;
	ptrShape->currentX2 = x2;
	ptrShape->currentY2 = y2;
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);
	_path_beginfigure2(path, x1 + 0.5, y1 + 0.5);
	_path_addline(path, x1, y1, x2 + 0.5, y2 + 0.5);
	_path_endfigure(path, TRUE);
	_path_close(path);
	EX_RECTF rc;
	_path_getbounds(path, &rc);
	//output(L"加入直线坐标", x1, y1, x2 + 0.5, y2 + 0.5);
	//output(L"直线外包矩形坐标", rc.left, rc.top, rc.right, rc.bottom);
	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;
	//取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	//形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

void _prototype_board_addrecttoshape(HEXOBJ hObj, int left, int top, int right, int bottom)
{
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_RECTANGLE;
	// 保存实时坐标
	ptrShape->currentLeft = left;
	ptrShape->currentTop = top;
	ptrShape->currentRight = right;
	ptrShape->currentBottom = bottom;
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);
	_path_beginfigure2(path, left + 0.5, top + 0.5);
	_path_addrect(path, left, top, right - 1, bottom - 1);
	_path_endfigure(path, TRUE);
	_path_close(path);
	EX_RECTF rc;
	_path_getbounds(path, &rc);
	//output(L"加入矩形坐标", left, top, right - 1, bottom - 1);
	//output(L"矩形外包矩形坐标", rc.left, rc.top, rc.right, rc.bottom);
	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;
	//取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	//形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

// 收集所有选中图形的指针
void _prototype_board_collect_selected_shapes(HEXOBJ hObj)
{
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);

	// 计算选中图形数量
	INT selectedCount = 0;
	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

		for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;

			if (ptrShape->bSelect)
			{
				selectedCount++;
			}
		}
	}

	// 分配内存存储选中图形指针
	if (selectedCount > 0)
	{
		LPVOID* selectedShapes = (LPVOID*)malloc(selectedCount * sizeof(LPVOID));
		INT index = 0;

		for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
		{
			size_t layerValue = 0;
			memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

			for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
			{
				size_t shapeValue = 0;
				memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
				EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;

				if (ptrShape->bSelect)
				{
					selectedShapes[index++] = ptrShape;
				}
			}
		}

		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES, (LONG_PTR)selectedShapes);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT, selectedCount);
	}
	else
	{
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT, 0);
	}
}

// 释放选中图形指针数组
void _prototype_board_free_selected_shapes(HEXOBJ hObj)
{
	LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);
	if (selectedShapes != 0)
	{
		free(selectedShapes);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES, 0);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT, 0);
	}
}

// 更新选中区域
void _prototype_board_update_selected_region(HEXOBJ hObj)
{
	HEXRGN oldhRgn = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
	if (oldhRgn != 0)
	{
		_rgn_destroy(oldhRgn);
	}

	INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
	LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);

	if (selectedCount == 0 || selectedShapes == 0)
	{
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, 0);
		return;
	}

	// 合并所有选中图形的区域
	HEXRGN combinedRgn = 0;
	for (INT i = 0; i < selectedCount; i++)
	{
		EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)selectedShapes[i];
		HEXRGN shapeRgn = _rgn_createfrompath(ptrShape->path);

		if (i == 0)
		{
			combinedRgn = shapeRgn;
		}
		else
		{
			HEXRGN newRgn = _rgn_combine(shapeRgn, combinedRgn, REGION_COMBINE_UNION, 0, 0);
			_rgn_destroy(combinedRgn);
			_rgn_destroy(shapeRgn);
			combinedRgn = newRgn;
		}
	}

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, (LONG_PTR)combinedRgn);
}

void _prototype_board_paint_mark(EX_PAINTSTRUCT ps, float scaleFactor, float canvasOffsetX, float canvasOffsetY)
{
	auto hBrushCanvas = _brush_create(ExARGB(255, 255, 255, 255));
	auto hBrushBorderOut = _brush_create(ExARGB(219, 219, 219, 255));
	auto hBrushBorderIn = _brush_create(ExARGB(245, 245, 245, 255));
	auto hFont = _font_createfromfamily(L"Arial", 10, 0);

	//绘制顶部
	//填充矩形
	_canvas_fillrect(ps.hCanvas, hBrushBorderIn, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.top + 20);
	_canvas_fillrect(ps.hCanvas, hBrushBorderIn, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.left + 20, ps.rcPaint.bottom);
	//画外部边框
	_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.top, 1, 0);
	_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left, ps.rcPaint.top + 20, ps.rcPaint.right, ps.rcPaint.top + 20, 1, 0);
	_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.left, ps.rcPaint.bottom, 1, 0);
	_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left + 20, ps.rcPaint.top, ps.rcPaint.left + 20, ps.rcPaint.bottom, 1, 0);

	//绘制正值横坐标大刻度线
	int markCountX = (ps.rcPaint.right - ps.rcPaint.left - canvasOffsetX) / scaleFactor + 1;
	for (int markIndexX = 0; markIndexX < markCountX; markIndexX++)
	{
		std::wstring text = std::to_wstring(markIndexX * 100);
		_canvas_drawtext(ps.hCanvas, hFont, ExARGB(50, 50, 50, 255), text.c_str(), -1, DT_LEFT, canvasOffsetX + markIndexX * scaleFactor + 5, ps.rcPaint.top, canvasOffsetX + markIndexX * scaleFactor + 100, ps.rcPaint.top + 20);
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX + markIndexX * scaleFactor, ps.rcPaint.top, canvasOffsetX + markIndexX * scaleFactor, ps.rcPaint.top + 20, 1, 0);
	}
	//绘制负值横坐标大刻度线
	int markCountXNegative = (canvasOffsetX) / scaleFactor + 1;
	for (int markIndexX = 0; markIndexX < markCountXNegative; markIndexX++)
	{
		std::wstring text = std::to_wstring(-markIndexX * 100);
		_canvas_drawtext(ps.hCanvas, hFont, ExARGB(50, 50, 50, 255), text.c_str(), -1, DT_LEFT, canvasOffsetX - markIndexX * scaleFactor + 5, ps.rcPaint.top, canvasOffsetX - markIndexX * scaleFactor + 100, ps.rcPaint.top + 20);
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX - markIndexX * scaleFactor, ps.rcPaint.top, canvasOffsetX - markIndexX * scaleFactor, ps.rcPaint.top + 20, 1, 0);
	}

	//绘制正值横坐标小刻度线
	int markCountX2 = (float)(ps.rcPaint.right - ps.rcPaint.left - canvasOffsetX) / scaleFactor * 10 + 1;

	for (int markIndexX2 = 0; markIndexX2 < markCountX2; markIndexX2++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX + markIndexX2 * scaleFactor / 10, ps.rcPaint.top + 10, canvasOffsetX + markIndexX2 * scaleFactor / 10, ps.rcPaint.top + 20, 1, 0);
	}
	//绘制负值横坐标小刻度线
	int markCountXNegative2 = (float)(canvasOffsetX) / scaleFactor * 10 + 1;
	for (int markIndexX2 = 0; markIndexX2 < markCountXNegative2; markIndexX2++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX - markIndexX2 * scaleFactor / 10, ps.rcPaint.top + 10, canvasOffsetX - markIndexX2 * scaleFactor / 10, ps.rcPaint.top + 20, 1, 0);
	}

	//绘制正值纵坐标大刻度线
	int markCountY = (ps.rcPaint.bottom - ps.rcPaint.top - canvasOffsetY) / scaleFactor + 1;
	for (int markIndexY = 0; markIndexY < markCountY; markIndexY++)
	{
		std::wstring text = std::to_wstring(markIndexY * 100);
		_canvas_drawtext(ps.hCanvas, hFont, ExARGB(50, 50, 50, 255), text.c_str(), -1, DT_LEFT, ps.rcPaint.left, canvasOffsetY + markIndexY * scaleFactor + 5, ps.rcPaint.left + 90, canvasOffsetY + markIndexY * scaleFactor + 25);
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left, canvasOffsetY + markIndexY * scaleFactor, ps.rcPaint.left + 20, canvasOffsetY + markIndexY * scaleFactor, 1, 0);
	}
	//绘制负值纵坐标大刻度线
	int markCountYNegative = (canvasOffsetY) / scaleFactor + 1;
	for (int markIndexY = 0; markIndexY < markCountYNegative; markIndexY++)
	{
		std::wstring text = std::to_wstring(-markIndexY * 100);
		_canvas_drawtext(ps.hCanvas, hFont, ExARGB(50, 50, 50, 255), text.c_str(), -1, DT_LEFT, ps.rcPaint.left, canvasOffsetY - markIndexY * scaleFactor + 5, ps.rcPaint.left + 90, canvasOffsetY - markIndexY * scaleFactor + 25);
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left, canvasOffsetY - markIndexY * scaleFactor, ps.rcPaint.left + 20, canvasOffsetY - markIndexY * scaleFactor, 1, 0);
	}

	//绘制正值纵坐标小刻度线
	int markCountY2 = (float)(ps.rcPaint.bottom - ps.rcPaint.top - canvasOffsetY) / scaleFactor * 10 + 1;
	for (int markIndexY2 = 0; markIndexY2 < markCountY2; markIndexY2++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left + 10, canvasOffsetY + markIndexY2 * scaleFactor / 10, ps.rcPaint.left + 20, canvasOffsetY + markIndexY2 * scaleFactor / 10, 1, 0);
	}
	//绘制负值纵坐标小刻度线
	int markCountYNegative2 = (float)(canvasOffsetY) / scaleFactor * 10 + 1;
	for (int markIndexY2 = 0; markIndexY2 < markCountYNegative2; markIndexY2++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, ps.rcPaint.left + 10, canvasOffsetY - markIndexY2 * scaleFactor / 10, ps.rcPaint.left + 20, canvasOffsetY - markIndexY2 * scaleFactor / 10, 1, 0);
	}


	//刻度线左上角遮挡矩形
	_canvas_fillrect(ps.hCanvas, hBrushBorderIn, ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.left + 20, ps.rcPaint.top + 20);

	_brush_destroy(hBrushBorderOut);
	_brush_destroy(hBrushBorderIn);
	_brush_destroy(hBrushCanvas);
	_font_destroy(hFont);
}

void _prototype_board_paint_background(EX_PAINTSTRUCT ps, float scaleFactor, float canvasWidth, float canvasHeight, float canvasOffsetX, float canvasOffsetY)
{
	auto hBrushCanvas = _brush_create(ExARGB(255, 255, 255, 255));
	auto hBrushBorderOut = _brush_create(ExARGB(210, 210, 210, 255));
	auto hBrushBorderIn = _brush_create(ExARGB(235, 235, 235, 255));
	//填充画布灰色底
	_canvas_clear(ps.hCanvas, ExARGB(241, 241, 241, 255));
	//绘制方框底色
	_canvas_fillrect(ps.hCanvas, hBrushCanvas, canvasOffsetX, canvasOffsetY, canvasOffsetX + canvasWidth, canvasOffsetY + canvasHeight);

	//画方框内部灰色边框
	int rowCount2 = 31;
	int colCount2 = 41;

	for (int rowIndex = 0; rowIndex < rowCount2; rowIndex++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderIn, canvasOffsetX, canvasOffsetY + rowIndex * scaleFactor / 10.0f, canvasOffsetX + canvasWidth, canvasOffsetY + rowIndex * scaleFactor / 10.0f, 1, 0);
	}
	for (int colIndex = 0; colIndex < colCount2; colIndex++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderIn, canvasOffsetX + colIndex * scaleFactor / 10.0f, canvasOffsetY, canvasOffsetX + colIndex * scaleFactor / 10.0f, canvasOffsetY + canvasHeight, 1, 0);
	}
	//画方框外部灰色边框
	int rowCount = 4;
	int colCount = 5;
	//画横线
	for (int rowIndex = 0; rowIndex < rowCount; rowIndex++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX, canvasOffsetY + rowIndex * scaleFactor, canvasOffsetX + canvasWidth, canvasOffsetY + rowIndex * scaleFactor, 1.5, 0);
	}
	//画竖线
	for (int colIndex = 0; colIndex < colCount; colIndex++)
	{
		_canvas_drawline(ps.hCanvas, hBrushBorderOut, canvasOffsetX + colIndex * scaleFactor, canvasOffsetY, canvasOffsetX + colIndex * scaleFactor, canvasOffsetY + canvasHeight, 1.5, 0);
	}

	_brush_destroy(hBrushBorderOut);
	_brush_destroy(hBrushBorderIn);
	_brush_destroy(hBrushCanvas);
}

void _prototype_board_update_selectcanvas(HEXOBJ hObj)
{
	HEXCANVAS canvasSelect = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_SELECT);
	_canvas_begindraw(canvasSelect);
	_canvas_clear(canvasSelect, ExARGB(255, 255, 255, 0));

	INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);
	auto hold = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD);

	// 只有按住鼠标且绘制类型为SELECT时才绘制选择区域
	if (hold == 0 || drawType != PROTOTYPEBOARD_DRAW_TYPE_SELECT) {
		_canvas_enddraw(canvasSelect);
		return;
	}

	auto startX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X);
	auto startY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y);
	auto x = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_X);
	auto y = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_Y);

	// 使用绿色半透明画笔
	auto hBrush = _brush_create(ExARGB(9, 156, 8, 50));
	_canvas_fillrect(canvasSelect, hBrush, startX, startY, x, y);
	_brush_destroy(hBrush);
	_canvas_enddraw(canvasSelect);
}

/// <summary>
/// 绘制选中区域
/// </summary>
/// <param name="hObj"></param>
void _prototype_board_update_choicecanvas(HEXOBJ hObj)
{
	HEXCANVAS canvasChoice = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_CHOICE);
	_canvas_begindraw(canvasChoice);
	_canvas_clear(canvasChoice, ExARGB(255, 255, 255, 0));

	HEXRGN hRgn = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
	if (hRgn != 0)
	{
		auto canvasOffsetX = -_prototype_board_get_canvasoffsetx(hObj);
		auto canvasOffsetY = -_prototype_board_get_canvasoffsety(hObj);
		// 获取选中区域的包围盒
		EX_RECTF bounds;
		_rgn_getbounds(hRgn, &bounds);
		// 为包围盒添加容差，方便用户看到和拖动
		FLOAT tolerance = 5.0f;
		EX_RECTF expandedBounds = {
			bounds.left - tolerance,
			bounds.top - tolerance,
			bounds.right + tolerance,
			bounds.bottom + tolerance
		};
		auto hBrush = _brush_create(ExARGB(127, 192, 187, 255));
		HEXMATRIX mx = _matrix_create();
		_matrix_translate(mx, canvasOffsetX, canvasOffsetY);
		auto scale = _prototype_board_get_scalefactor(hObj);
		auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
		_matrix_scale(mx, (scale / scaleinit), (scale / scaleinit));
		_canvas_settransform(canvasChoice, mx);//因为形状是坐标系坐标，所以需要偏移矩阵

		// 绘制扩大后的包围盒，让用户知道可以拖动的区域
		_canvas_drawrect(canvasChoice, hBrush, expandedBounds.left, expandedBounds.top,
			expandedBounds.right, expandedBounds.bottom, 1, 0);
		// 如果只有一个选中的形状，绘制控制点
		INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
		if (selectedCount == 1) {
			auto hBrushHandle = _brush_create(ExARGB(255, 0, 120, 215)); // 蓝色控制点
			FLOAT halfSize = RESIZE_HANDLE_SIZE / 2.0f;

			// 绘制四个角的控制点
			_canvas_fillrect(canvasChoice, hBrushHandle,
				bounds.left - halfSize, bounds.top - halfSize,
				bounds.left + halfSize, bounds.top + halfSize);
			_canvas_fillrect(canvasChoice, hBrushHandle,
				bounds.right - halfSize, bounds.top - halfSize,
				bounds.right + halfSize, bounds.top + halfSize);
			_canvas_fillrect(canvasChoice, hBrushHandle,
				bounds.right - halfSize, bounds.bottom - halfSize,
				bounds.right + halfSize, bounds.bottom + halfSize);
			_canvas_fillrect(canvasChoice, hBrushHandle,
				bounds.left - halfSize, bounds.bottom - halfSize,
				bounds.left + halfSize, bounds.bottom + halfSize);

			_brush_destroy(hBrushHandle);
		}
		_canvas_settransform(canvasChoice, 0);
		_brush_destroy(hBrush);
		_matrix_destroy(mx);
	}
	_canvas_enddraw(canvasChoice);
}

void _prototype_board_update_tempcanvas(HEXOBJ hObj)
{
	HEXCANVAS canvasTemp = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_TEMP);
	_canvas_begindraw(canvasTemp);
	_canvas_clear(canvasTemp, ExARGB(255, 255, 255, 0));

	//绘制
	INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);
	auto hold = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD);

	// 只有按住鼠标且有绘制类型时才绘制临时形状
	if (hold == 0 || drawType < PROTOTYPEBOARD_DRAW_TYPE_LINE || drawType >= PROTOTYPEBOARD_DRAW_TYPE_SELECT) {
		_canvas_enddraw(canvasTemp);
		return;
	}

	// 获取当前鼠标位置和起始位置（屏幕坐标）
	INT ptX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_X);
	INT ptY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_Y);
	INT startX = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X);
	INT startY = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y);

	// 如果鼠标位置没有移动，不绘制
	if (ptX == startX && ptY == startY) {
		_canvas_enddraw(canvasTemp);
		return;
	}

	// 使用屏幕坐标直接绘制临时形状（与旧版代码一致）
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t ptrValue = 0;
	memmove(&ptrValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	auto ptr = (EX_PROTOTYPEBOARD_LAYER*)ptrValue;
	auto hBrush = _brush_create(ptr->color);

	if (drawType == PROTOTYPEBOARD_DRAW_TYPE_LINE)
	{
		_canvas_drawline(canvasTemp, hBrush, startX, startY, ptX, ptY, 2, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_RECT)
	{
		// 确保矩形坐标正确
		INT left = min(startX, ptX);
		INT top = min(startY, ptY);
		INT right = max(startX, ptX);
		INT bottom = max(startY, ptY);

		_canvas_drawrect(canvasTemp, hBrush, left, top, right, bottom, 2, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_ELLIPSE)
	{
		INT centerX = (startX + ptX) / 2;
		INT centerY = (startY + ptY) / 2;
		INT radiusX = abs(ptX - startX) / 2;
		INT radiusY = abs(ptY - startY) / 2;

		_canvas_drawellipse(canvasTemp, hBrush, centerX, centerY, radiusX, radiusY, 2, 0);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_TEXT)
	{
		INT left = min(startX, ptX);
		INT top = min(startY, ptY);
		INT right = max(startX, ptX);
		INT bottom = max(startY, ptY);
		// 绘制文本预览矩形
		auto hBrushGray = _brush_create(ExARGB(200, 200, 200, 255));
		_canvas_fillrect(canvasTemp, hBrushGray, left, top, right, bottom);
		_brush_destroy(hBrushGray);

		// 绘制边框
		_canvas_drawrect(canvasTemp, hBrush, left, top, right, bottom, 1, 0);

		// 绘制"文本"字样
		auto hFont = _font_createfromfamily(L"Arial", 12, 0);
		_canvas_drawtext(canvasTemp, hFont, ExARGB(100, 100, 100, 255),
			L"文本", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE,
			left, top, right, bottom);
		_font_destroy(hFont);
	}
	else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
	{
		INT left = min(startX, ptX);
		INT top = min(startY, ptY);
		INT right = max(startX, ptX);
		INT bottom = max(startY, ptY);
		// 绘制默认灰色矩形表示图片区域
		_canvas_drawrect(canvasTemp, hBrush, left, top, right, bottom, 1, 0);
	}
	_brush_destroy(hBrush);
	_canvas_enddraw(canvasTemp);
}

void _prototype_board_update_bkgcanvas(HEXOBJ hObj)
{
	HEXCANVAS canvasBkg = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_BACKGROUND);
	_canvas_begindraw(canvasBkg);
	_canvas_clear(canvasBkg, ExARGB(255, 255, 255, 0));
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	INT hoverLayerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX);
	INT hoverShapeIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX);

	auto canvasOffsetX = -_prototype_board_get_canvasoffsetx(hObj);
	auto canvasOffsetY = -_prototype_board_get_canvasoffsety(hObj);
	auto scale = _prototype_board_get_scalefactor(hObj);
	auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
	HEXMATRIX mx = _matrix_create();
	_matrix_translate(mx, canvasOffsetX, canvasOffsetY);
	_matrix_scale(mx, (scale / scaleinit), (scale / scaleinit));
	_canvas_settransform(canvasBkg, mx);//因为形状是坐标系坐标，所以需要偏移矩阵
	auto hBrush = _brush_create(ExARGB(0, 0, 0, 255));
	auto hBrushHover = _brush_create(ExARGB(67, 255, 253, 255));
	auto hBrushSelect = _brush_create(ExARGB(115, 180, 175, 255));
	auto hBrushChoice = _brush_create(ExARGB(127, 192, 187, 255));
	for (int i = 0; i < arr->layersCount; i++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + i * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;
		_brush_setcolor(hBrush, ptrLayer->color);
		for (int j = 0; j < ptrLayer->shapesCount; j++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + j * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;
			if (ptrShape->type == ShapeType::SHAPE_IMAGE)
			{
				// 绘制图片形状
				if (ptrShape->imageHandle != 0)
				{
					// 有图片，绘制图片
			// 计算图片绘制区域
					FLOAT drawLeft = ptrShape->currentLeft;
					FLOAT drawTop = ptrShape->currentTop;
					FLOAT drawRight = ptrShape->currentRight;
					FLOAT drawBottom = ptrShape->currentBottom;

					// 如果保持宽高比，需要调整绘制区域
					if (ptrShape->keepAspectRatio && ptrShape->imageWidth > 0 && ptrShape->imageHeight > 0)
					{
						FLOAT rectWidth = drawRight - drawLeft;
						FLOAT rectHeight = drawBottom - drawTop;
						FLOAT imageRatio = (FLOAT)ptrShape->imageWidth / (FLOAT)ptrShape->imageHeight;
						FLOAT rectRatio = rectWidth / rectHeight;

						if (imageRatio > rectRatio)
						{
							// 以宽度为准，调整高度
							FLOAT newHeight = rectWidth / imageRatio;
							FLOAT centerY = (drawTop + drawBottom) / 2;
							drawTop = centerY - newHeight / 2;
							drawBottom = centerY + newHeight / 2;
						}
						else
						{
							// 以高度为准，调整宽度
							FLOAT newWidth = rectHeight * imageRatio;
							FLOAT centerX = (drawLeft + drawRight) / 2;
							drawLeft = centerX - newWidth / 2;
							drawRight = centerX + newWidth / 2;
						}
					}

					// 有图片，绘制图片
					_canvas_drawimagerectrect(canvasBkg, ptrShape->imageHandle,
						drawLeft, drawTop,
						drawRight,
						drawBottom,
						0, 0, ptrShape->imageWidth, ptrShape->imageHeight, 255);
				
					// 绘制边框
					if (ptrShape->bSelect)
					{
						_canvas_drawrect(canvasBkg, hBrushChoice,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
					else if (i == hoverLayerIndex && j == hoverShapeIndex)
					{
						_canvas_drawrect(canvasBkg, hBrushHover,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
				}
				else
				{
					// 没有图片，绘制灰色背景
					auto hBrushGray = _brush_create(ExARGB(200, 200, 200, 255));
					_canvas_fillrect(canvasBkg, hBrushGray,
						ptrShape->currentLeft, ptrShape->currentTop,
						ptrShape->currentRight, ptrShape->currentBottom);
					_brush_destroy(hBrushGray);

					// 绘制边框和文字
					if (ptrShape->bSelect)
					{
						_canvas_drawrect(canvasBkg, hBrushChoice,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
					else if (i == hoverLayerIndex && j == hoverShapeIndex)
					{
						_canvas_drawrect(canvasBkg, hBrushHover,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
					else
					{
						_canvas_drawrect(canvasBkg, hBrush,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 1, 0);
					}

					// 绘制"图片"文字
					auto hFont = _font_createfromfamily(L"Arial", 12, 0);
					_canvas_drawtext(canvasBkg, hFont, ExARGB(100, 100, 100, 255),
						L"图片", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE,
						ptrShape->currentLeft, ptrShape->currentTop,
						ptrShape->currentRight, ptrShape->currentBottom);
					_font_destroy(hFont);
				}
			}
			else if (ptrShape->type == ShapeType::SHAPE_TEXT)
			{
				// 绘制文本形状
				if (ptrShape->textContent != nullptr && ptrShape->textFont != 0)
				{
					// 有文本内容，绘制文本
					auto hBrushText = _brush_create(ptrShape->textColor);

					// 绘制文本背景（可选）
					if (ptrShape->bSelect || i == hoverLayerIndex && j == hoverShapeIndex)
					{
						// 选中或悬停时绘制背景
						auto hBrushBg = _brush_create(ExARGB(240, 240, 240, 255));
						_canvas_fillrect(canvasBkg, hBrushBg,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom);
						_brush_destroy(hBrushBg);
					}

					// 绘制文本
					_canvas_drawtext(canvasBkg, ptrShape->textFont, ptrShape->textColor,
						ptrShape->textContent, -1, ptrShape->textFormat,
						ptrShape->currentLeft, ptrShape->currentTop,
						ptrShape->currentRight, ptrShape->currentBottom);

					_brush_destroy(hBrushText);
				}
				else
				{
					// 没有文本内容，绘制灰色背景和"文本"字样
					auto hBrushGray = _brush_create(ExARGB(200, 200, 200, 255));
					_canvas_fillrect(canvasBkg, hBrushGray,
						ptrShape->currentLeft, ptrShape->currentTop,
						ptrShape->currentRight, ptrShape->currentBottom);
					_brush_destroy(hBrushGray);

					// 绘制边框
					if (ptrShape->bSelect)
					{
						_canvas_drawrect(canvasBkg, hBrushChoice,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
					else if (i == hoverLayerIndex && j == hoverShapeIndex)
					{
						_canvas_drawrect(canvasBkg, hBrushHover,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 2, 0);
					}
					else
					{
						_canvas_drawrect(canvasBkg, hBrush,
							ptrShape->currentLeft, ptrShape->currentTop,
							ptrShape->currentRight, ptrShape->currentBottom, 1, 0);
					}

					// 绘制"文本"文字
					auto hFont = _font_createfromfamily(L"Arial", 12, 0);
					_canvas_drawtext(canvasBkg, hFont, ExARGB(100, 100, 100, 255),
						L"文本", -1, DT_CENTER | DT_VCENTER | DT_SINGLELINE,
						ptrShape->currentLeft, ptrShape->currentTop,
						ptrShape->currentRight, ptrShape->currentBottom);
					_font_destroy(hFont);
				}
			}
			else
			{
				// 其他形状的绘制代码保持不变...
				if (ptrShape->bSelect)
				{
					_canvas_drawpath(canvasBkg, ptrShape->path, hBrushChoice, 2, 0);
				}
				else {
					if (i == hoverLayerIndex && j == hoverShapeIndex)
					{
						_canvas_drawpath(canvasBkg, ptrShape->path, hBrushHover, 2, 0);
					}
					else {
						_canvas_drawpath(canvasBkg, ptrShape->path, hBrush, 2, 0);
					}
				}
			}

		}
	}

	_canvas_settransform(canvasBkg, 0);
	_matrix_destroy(mx);
	_brush_destroy(hBrush);
	_brush_destroy(hBrushHover);
	_brush_destroy(hBrushSelect);
	_brush_destroy(hBrushChoice);
	_canvas_enddraw(canvasBkg);
}

void _prototype_board_paint(HEXOBJ hObj)
{
	EX_PAINTSTRUCT ps{ 0 };

	float scaleFactor = _prototype_board_get_scalefactor(hObj);
	float canvasWidth = _prototype_board_get_canvaswidth(hObj);
	float canvasHeight = _prototype_board_get_canvasheight(hObj);
	float canvasOffsetX = -_prototype_board_get_canvasoffsetx(hObj);
	float canvasOffsetY = -_prototype_board_get_canvasoffsety(hObj);
	INT drawType = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE);
	auto mode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_MODE);
	auto hold = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_HOLD);
	BOOL draggingSelected = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED);
	BOOL resizeMode = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE);

	// 更新背景画布（包含所有已绘制形状）
	_prototype_board_update_bkgcanvas(hObj);

	// 更新选中区域画布（包含选中图形的包围盒和控制点）
	_prototype_board_update_choicecanvas(hObj);

	// 如果是绘制模式，更新路径预览
	if (mode == PROTOTYPEBOARD_MODE_DRAW)
	{
		// 路径预览在绘制函数中单独处理
	}
	else if (mode == PROTOTYPEBOARD_MODE_SELECT)
	{
		// 在选择模式下，如果正在拖拽选中区域，更新临时画布
		if (hold == 1)
		{
			// 如果有特定的绘制类型（直线、矩形、椭圆等）
			if (drawType >= PROTOTYPEBOARD_DRAW_TYPE_LINE && drawType <= PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
			{
				_prototype_board_update_tempcanvas(hObj);
			}
			// 如果是选择类型，更新选择画布（绿色半透明矩形）
			else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_SELECT)
			{
				_prototype_board_update_selectcanvas(hObj);
			}
		}
	}

	if (Ex_ObjBeginPaint(hObj, &ps))
	{
		//绘制底图和方框
		_prototype_board_paint_background(ps, scaleFactor, canvasWidth, canvasHeight, canvasOffsetX, canvasOffsetY);

		//绘制背景画布（包含所有形状）
		HEXCANVAS canvasBkg = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_BACKGROUND);
		_canvas_drawcanvas(ps.hCanvas, canvasBkg, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 255, CANVAS_COMPOSITE_MODE_SRCOVER);

		// 绘制路径预览（在绘制模式下）
		if (mode == PROTOTYPEBOARD_MODE_DRAW)
		{
			auto scale = _prototype_board_get_scalefactor(hObj);
			auto scaleinit = _prototype_board_get_scalefactorinit(hObj);
			_prototype_board_draw_path_preview(hObj, ps.hCanvas, scaleFactor, canvasOffsetX, canvasOffsetY, scale, scaleinit);
		}

		// 绘制选中区域画布（包含选中图形的包围盒和控制点）
		HEXCANVAS canvasChoice = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_CHOICE);
		_canvas_drawcanvas(ps.hCanvas, canvasChoice, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 255, CANVAS_COMPOSITE_MODE_SRCOVER);

		// 绘制临时形状或选择框（只有在按住鼠标时）
		if (hold == 1 && mode == PROTOTYPEBOARD_MODE_SELECT)
		{
			// 如果正在拖动选中图形或调整大小，不绘制临时图形
			if (!draggingSelected && !resizeMode)
			{
				// 如果有特定的绘制类型（直线、矩形、椭圆、文本、图片）
				if (drawType >= PROTOTYPEBOARD_DRAW_TYPE_LINE && drawType <= PROTOTYPEBOARD_DRAW_TYPE_IMAGE)
				{
					HEXCANVAS canvasTemp = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_TEMP);
					_canvas_drawcanvas(ps.hCanvas, canvasTemp, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 255, CANVAS_COMPOSITE_MODE_SRCOVER);
				}
				// 如果是选择类型，绘制绿色半透明矩形
				else if (drawType == PROTOTYPEBOARD_DRAW_TYPE_SELECT)
				{
					HEXCANVAS canvasSelect = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_SELECT);
					_canvas_drawcanvas(ps.hCanvas, canvasSelect, 0, 0, ps.uWidth, ps.uHeight, 0, 0, 255, CANVAS_COMPOSITE_MODE_SRCOVER);
				}
			}
		}

		//绘制刻度线,需要挡住全部图形在最后面绘制
		_prototype_board_paint_mark(ps, scaleFactor, canvasOffsetX, canvasOffsetY);
		Ex_ObjEndPaint(hObj, &ps);
	}
}

void _prototype_board_set_canvaswidth(HEXOBJ hObj, float width)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_WIDTH);
	memmove(ptr, &width, 4);
}

float _prototype_board_get_canvaswidth(HEXOBJ hObj)
{
	float width;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_WIDTH);
	memmove(&width, ptr, 4);
	return width;
}

void _prototype_board_set_canvasheight(HEXOBJ hObj, float height)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_HEIGHT);
	memmove(ptr, &height, 4);
}

float _prototype_board_get_canvasheight(HEXOBJ hObj)
{
	float height;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_HEIGHT);
	memmove(&height, ptr, 4);
	return height;
}

void _prototype_board_set_canvasoffsetx(HEXOBJ hObj, float offsetX)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_X);
	memmove(ptr, &offsetX, 4);
}

float _prototype_board_get_canvasoffsetx(HEXOBJ hObj)
{
	float offsetX;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_X);
	memmove(&offsetX, ptr, 4);
	return offsetX;
}

void _prototype_board_set_canvasoffsety(HEXOBJ hObj, float offsetY)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_Y);
	memmove(ptr, &offsetY, 4);
}

float _prototype_board_get_canvasoffsety(HEXOBJ hObj)
{
	float offsetY;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_Y);
	memmove(&offsetY, ptr, 4);
	return offsetY;
}

void _prototype_board_set_scalefactor(HEXOBJ hObj, float scaleFactor)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR);
	memmove(ptr, &scaleFactor, 4);
}

float _prototype_board_get_scalefactor(HEXOBJ hObj)
{
	float scaleFactor;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR);
	memmove(&scaleFactor, ptr, 4);
	return scaleFactor;
}

void _prototype_board_set_scalefactorinit(HEXOBJ hObj, float scaleFactor)
{
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR_INIT);
	memmove(ptr, &scaleFactor, 4);
}

float _prototype_board_get_scalefactorinit(HEXOBJ hObj)
{
	float scaleFactor;
	auto ptr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR_INIT);
	memmove(&scaleFactor, ptr, 4);
	return scaleFactor;
}

void _prototype_board_initparam(HEXOBJ hObj)
{
	auto canvasOffsetXPtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_X, (LONG_PTR)canvasOffsetXPtr);
	_prototype_board_set_canvasoffsetx(hObj, -20);

	auto canvasOffsetYPtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_Y, (LONG_PTR)canvasOffsetYPtr);
	_prototype_board_set_canvasoffsety(hObj, -20);

	auto canvasWidthPtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_WIDTH, (LONG_PTR)canvasWidthPtr);
	_prototype_board_set_canvaswidth(hObj, 400);

	auto canvasHeightPtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_HEIGHT, (LONG_PTR)canvasHeightPtr);
	_prototype_board_set_canvasheight(hObj, 300);

	auto scalePtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR, (LONG_PTR)scalePtr);
	_prototype_board_set_scalefactor(hObj, 100);

	auto scaleinitPtr = malloc(sizeof(float));
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR_INIT, (LONG_PTR)scaleinitPtr);
	_prototype_board_set_scalefactorinit(hObj, 100);

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_MODE, PROTOTYPEBOARD_MODE_SELECT);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_OFFSET_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAW_TYPE, -1);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_LEFT, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_TOP, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM, 0);

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED, 0);

	// 初始化新的坐标变量
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_WORLD_Y, 0);

	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)malloc(sizeof(EX_PROTOTYPEBOARD_LAYER_ARRAY));
	arr->layersCount = 1;
	arr->layers = malloc(sizeof(size_t));

	auto layerPtr = (EX_PROTOTYPEBOARD_LAYER*)malloc(sizeof(EX_PROTOTYPEBOARD_LAYER));
	layerPtr->shapesCount = 0;
	layerPtr->index = 0;
	layerPtr->shapes = nullptr;
	layerPtr->color = PROTOTYPEBOARD_LAYER_COLORS[0];
	size_t ptrValue = (size_t)layerPtr;
	memmove(arr->layers, &ptrValue, sizeof(size_t));

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS, (LONG_PTR)arr);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX, 0);
	RECT rc;
	Ex_ObjGetRect(hObj, &rc);

	auto canvasTemp = _canvas_createfromobj(hObj, (rc.right), (rc.bottom), 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_TEMP, canvasTemp);

	auto canvasSelect = _canvas_createfromobj(hObj, (rc.right), (rc.bottom), 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_SELECT, canvasSelect);

	auto canvasChoice = _canvas_createfromobj(hObj, (rc.right), (rc.bottom), 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_CHOICE, canvasChoice);

	auto canvasBkg = _canvas_createfromobj(hObj, (rc.right), (rc.bottom), 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_BACKGROUND, canvasBkg);

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX, -1);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX, -1);

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, 0);
}

void _prototype_board_uninitparam(HEXOBJ hObj)
{
	// 释放存储的中心点和偏移
	auto centerXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_X);
	auto centerYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y);
	auto offsetXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X);
	auto offsetYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y);

	if (centerXPtr) free(centerXPtr);
	if (centerYPtr) free(centerYPtr);
	if (offsetXPtr) free(offsetXPtr);
	if (offsetYPtr) free(offsetYPtr);
	// 释放选中图形指针数组
	_prototype_board_free_selected_shapes(hObj);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	for (int j = 0; j < arr->layersCount; j++)
	{
		size_t ptrValue = 0;
		memmove(&ptrValue, (LPVOID)((size_t)arr->layers + j * sizeof(size_t)), sizeof(size_t));
		auto ptr = (EX_PROTOTYPEBOARD_LAYER*)ptrValue;
		for (int i = 0; i < ptr->shapesCount; i++)
		{
			size_t ptrValue2 = 0;
			memmove(&ptrValue2, (LPVOID)((size_t)ptr->shapes + i * sizeof(size_t)), sizeof(size_t));
			auto ptr2 = (EX_PROTOTYPEBOARD_SHAPE*)ptrValue2;
			if (ptr2->type == ShapeType::SHAPE_IMAGE && ptr2->imageHandle != 0)
			{
				_img_destroy(ptr2->imageHandle);
			}
			else if (ptr2->type == ShapeType::SHAPE_TEXT)
			{
				// 释放文本内容内存
				if (ptr2->textContent != nullptr)
				{
					Ex_MemFree(ptr2->textContent);
					ptr2->textContent = nullptr;
				}
			}
			else if (ptr2->type == ShapeType::SHAPE_PATH)
			{
				// 释放路径点数组
				if (ptr2->pathPoints != 0)
				{
					free(ptr2->pathPoints);
					ptr2->pathPoints = 0;
				}
			}
			_path_destroy(ptr2->path);
			free(ptr2);
		}
		if (ptr->shapes)
		{
			free(ptr->shapes);
		}
		free(ptr);
	}
	free(arr->layers);
	free(arr);

	auto canvasTemp = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_TEMP);
	_canvas_destroy(canvasTemp);
	auto canvasSelect = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_SELECT);
	_canvas_destroy(canvasSelect);
	auto canvasChoice = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_CHOICE);
	_canvas_destroy(canvasChoice);
	auto canvasBkg = (HEXCANVAS)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_BACKGROUND);
	_canvas_destroy(canvasBkg);


	void* canvasOffsetXPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_X);
	free(canvasOffsetXPtr);

	void* canvasOffsetYPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_OFFSET_CANVAS_Y);
	free(canvasOffsetYPtr);

	void* canvasWidthPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_WIDTH);
	free(canvasWidthPtr);

	void* canvasHeightPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_CANVAS_HEIGHT);
	free(canvasHeightPtr);

	void* scalePtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR);
	free(scalePtr);

	void* scaleinitPtr = (void*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SCALE_FACTOR_INIT);
	free(scaleinitPtr);

	HEXRGN oldhRgn = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
	if (oldhRgn != 0)
	{
		_rgn_destroy(oldhRgn);
	}
	// 清理调整大小相关的属性
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);

	// 清理路径绘制数据
	LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);
	if (pathPoints != 0)
	{
		free(pathPoints);
	}
}

/// <summary>
/// 判断点是否在控制点上
/// </summary>
ResizeCorner _prototype_board_hittest_resize_handle(HEXOBJ hObj, FLOAT worldX, FLOAT worldY, EX_PROTOTYPEBOARD_SHAPE* shape)
{
	if (shape == NULL) return CORNER_NONE;

	// 获取形状的包围盒
	EX_RECTF bounds;

	// 对于所有形状，直接使用其boundingRect（这是实际的包围盒）
	bounds = shape->boundingRect;

	// 对于椭圆，使用boundingRect（已经是最小的包围盒，不是外接矩形）
	// 之前的逻辑存在问题，因为椭圆的实际绘制区域可能小于外接矩形

	FLOAT halfSize = RESIZE_HANDLE_SIZE / 2.0f;
	FLOAT handleOffset = halfSize * 2; // 控制点检测范围扩大到2倍，更容易点击

	// 检查四个角 - 使用椭圆的实际包围盒（boundingRect）
	if (worldX >= bounds.left - handleOffset && worldX <= bounds.left + handleOffset &&
		worldY >= bounds.top - handleOffset && worldY <= bounds.top + handleOffset) {
		return CORNER_TOP_LEFT;
	}

	if (worldX >= bounds.right - handleOffset && worldX <= bounds.right + handleOffset &&
		worldY >= bounds.top - handleOffset && worldY <= bounds.top + handleOffset) {
		return CORNER_TOP_RIGHT;
	}

	if (worldX >= bounds.right - handleOffset && worldX <= bounds.right + handleOffset &&
		worldY >= bounds.bottom - handleOffset && worldY <= bounds.bottom + handleOffset) {
		return CORNER_BOTTOM_RIGHT;
	}

	if (worldX >= bounds.left - handleOffset && worldX <= bounds.left + handleOffset &&
		worldY >= bounds.bottom - handleOffset && worldY <= bounds.bottom + handleOffset) {
		return CORNER_BOTTOM_LEFT;
	}

	return CORNER_NONE;
}

/// <summary>
/// 获取单个选中的形状
/// </summary>
EX_PROTOTYPEBOARD_SHAPE* _prototype_board_get_single_selected_shape(HEXOBJ hObj)
{
	INT selectedCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT);
	LPVOID* selectedShapes = (LPVOID*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECTED_SHAPES);

	if (selectedCount == 1 && selectedShapes != 0) {
		return (EX_PROTOTYPEBOARD_SHAPE*)selectedShapes[0];
	}
	return NULL;
}

/// <summary>
/// 调整形状大小
/// </summary>
void _prototype_board_resize_shape(HEXOBJ hObj, EX_PROTOTYPEBOARD_SHAPE* shape, INT corner, FLOAT deltaX, FLOAT deltaY)
{
	if (shape == NULL) return;

	// 获取初始状态
	FLOAT startLeft = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_LEFT);
	FLOAT startTop = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_TOP);
	FLOAT startRight = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT);
	FLOAT startBottom = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM);
	FLOAT startCenterX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERX);
	FLOAT startCenterY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERY);
	FLOAT startRadiusX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSX);
	FLOAT startRadiusY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSY);

	// 根据形状类型调整
	if (shape->type == ShapeType::SHAPE_RECTANGLE ||
		shape->type == ShapeType::SHAPE_TEXT ||
		shape->type == ShapeType::SHAPE_IMAGE) {
		FLOAT newLeft = shape->currentLeft;
		FLOAT newTop = shape->currentTop;
		FLOAT newRight = shape->currentRight;
		FLOAT newBottom = shape->currentBottom;

		switch (corner) {
		case CORNER_TOP_LEFT:
			newLeft = startLeft + deltaX;
			newTop = startTop + deltaY;
			break;
		case CORNER_TOP_RIGHT:
			newRight = startRight + deltaX;
			newTop = startTop + deltaY;
			break;
		case CORNER_BOTTOM_RIGHT:
			newRight = startRight + deltaX;
			newBottom = startBottom + deltaY;
			break;
		case CORNER_BOTTOM_LEFT:
			newLeft = startLeft + deltaX;
			newBottom = startBottom + deltaY;
			break;
		}

		// 应用限制：最小宽高为10
		if (newRight - newLeft < 10.0f) {
			if (corner == CORNER_TOP_LEFT || corner == CORNER_BOTTOM_LEFT) {
				newLeft = newRight - 10.0f;
			}
			else {
				newRight = newLeft + 10.0f;
			}
		}
		if (newBottom - newTop < 10.0f) {
			if (corner == CORNER_TOP_LEFT || corner == CORNER_TOP_RIGHT) {
				newTop = newBottom - 10.0f;
			}
			else {
				newBottom = newTop + 10.0f;
			}
		}

		shape->currentLeft = newLeft;
		shape->currentTop = newTop;
		shape->currentRight = newRight;
		shape->currentBottom = newBottom;

		// 重新创建路径
		_path_destroy(shape->path);
		HEXPATH newPath;
		_path_create(1, &newPath);
		_path_open(newPath);
		_path_beginfigure2(newPath, newLeft + 0.5, newTop + 0.5);
		_path_addrect(newPath, newLeft, newTop, newRight - 1, newBottom - 1);
		_path_endfigure(newPath, TRUE);
		_path_close(newPath);
		shape->path = newPath;
		_path_getbounds(newPath, &shape->boundingRect);
	}
	else if (shape->type == ShapeType::SHAPE_ELLIPSE) {
		// 对于椭圆，基于中心点和半径进行调整
		FLOAT startCenterX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERX);
		FLOAT startCenterY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_CENTERY);
		FLOAT startRadiusX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSX);
		FLOAT startRadiusY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSY);

		FLOAT newRadiusX = startRadiusX;
		FLOAT newRadiusY = startRadiusY;

		switch (corner) {
		case CORNER_TOP_LEFT:
			// 左上角：向左上拖动，两个半径都减小
			newRadiusX = startRadiusX - deltaX;
			newRadiusY = startRadiusY - deltaY;
			break;
		case CORNER_TOP_RIGHT:
			// 右上角：向右上拖动，水平半径增加，垂直半径减小
			newRadiusX = startRadiusX + deltaX;
			newRadiusY = startRadiusY - deltaY;
			break;
		case CORNER_BOTTOM_RIGHT:
			// 右下角：向右下拖动，两个半径都增加
			newRadiusX = startRadiusX + deltaX;
			newRadiusY = startRadiusY + deltaY;
			break;
		case CORNER_BOTTOM_LEFT:
			// 左下角：向左下拖动，水平半径减小，垂直半径增加
			newRadiusX = startRadiusX - deltaX;
			newRadiusY = startRadiusY + deltaY;
			break;
		}

		// 确保半径不为负且有一定最小值
		if (newRadiusX < 5.0f) newRadiusX = 5.0f;
		if (newRadiusY < 5.0f) newRadiusY = 5.0f;

		// 更新椭圆属性 - 保持中心点不变
		shape->currentCenterX = startCenterX;
		shape->currentCenterY = startCenterY;
		shape->currentRadiusX = newRadiusX;
		shape->currentRadiusY = newRadiusY;

		// 更新椭圆的包围盒（用于显示控制点）
		shape->currentLeft = startCenterX - newRadiusX;
		shape->currentTop = startCenterY - newRadiusY;
		shape->currentRight = startCenterX + newRadiusX;
		shape->currentBottom = startCenterY + newRadiusY;

		// 重新创建椭圆路径 - 使用正确的椭圆绘制方法
		_path_destroy(shape->path);
		HEXPATH newPath;
		_path_create(1, &newPath);
		_path_open(newPath);

		// 使用四段圆弧绘制椭圆，方法与创建椭圆时一致
		_path_beginfigure2(newPath, startCenterX - newRadiusX + 0.5, startCenterY + 0.5);
		_path_addarc(newPath, startCenterX - newRadiusX + 0.5, startCenterY + 0.5,
			startCenterX + 0.5, startCenterY - newRadiusY + 0.5,
			newRadiusX, newRadiusY, TRUE);
		_path_addarc(newPath, startCenterX + 0.5, startCenterY - newRadiusY + 0.5,
			startCenterX + newRadiusX + 0.5, startCenterY + 0.5,
			newRadiusX, newRadiusY, TRUE);
		_path_addarc(newPath, startCenterX + newRadiusX + 0.5, startCenterY + 0.5,
			startCenterX + 0.5, startCenterY + newRadiusY + 0.5,
			newRadiusX, newRadiusY, TRUE);
		_path_addarc(newPath, startCenterX + 0.5, startCenterY + newRadiusY + 0.5,
			startCenterX - newRadiusX + 0.5, startCenterY + 0.5,
			newRadiusX, newRadiusY, TRUE);
		_path_endfigure(newPath, TRUE);
		_path_close(newPath);

		shape->path = newPath;
		_path_getbounds(newPath, &shape->boundingRect);
	}
	else if (shape->type == ShapeType::SHAPE_LINE) {
		FLOAT startX1 = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X1);
		FLOAT startY1 = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y1);
		FLOAT startX2 = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_X2);
		FLOAT startY2 = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_START_Y2);

		FLOAT newX1 = startX1;
		FLOAT newY1 = startY1;
		FLOAT newX2 = startX2;
		FLOAT newY2 = startY2;

		switch (corner) {
		case CORNER_TOP_LEFT:
			// 直线没有实际的角点，我们假设控制点在起点
			newX1 = startX1 + deltaX;
			newY1 = startY1 + deltaY;
			break;
		case CORNER_TOP_RIGHT:
			// 右上角控制点在终点
			newX2 = startX2 + deltaX;
			newY1 = startY1 + deltaY;
			break;
		case CORNER_BOTTOM_RIGHT:
			// 右下角控制点在终点
			newX2 = startX2 + deltaX;
			newY2 = startY2 + deltaY;
			break;
		case CORNER_BOTTOM_LEFT:
			// 左下角控制点在起点
			newX1 = startX1 + deltaX;
			newY2 = startY2 + deltaY;
			break;
		}

		// 确保直线长度足够
		FLOAT length = sqrt(pow(newX2 - newX1, 2) + pow(newY2 - newY1, 2));
		if (length < 10.0f) {
			// 如果长度太小，恢复到之前的状态
			newX1 = shape->currentX1;
			newY1 = shape->currentY1;
			newX2 = shape->currentX2;
			newY2 = shape->currentY2;
		}

		shape->currentX1 = newX1;
		shape->currentY1 = newY1;
		shape->currentX2 = newX2;
		shape->currentY2 = newY2;

		// 更新直线包围盒
		shape->currentLeft = min(newX1, newX2);
		shape->currentTop = min(newY1, newY2);
		shape->currentRight = max(newX1, newX2);
		shape->currentBottom = max(newY1, newY2);

		_path_destroy(shape->path);
		HEXPATH newPath;
		_path_create(1, &newPath);
		_path_open(newPath);
		_path_beginfigure2(newPath, newX1, newY1);
		_path_addline(newPath, newX1, newY1, newX2, newY2);
		_path_endfigure(newPath, TRUE);
		_path_close(newPath);
		shape->path = newPath;
		_path_getbounds(newPath, &shape->boundingRect);
	}

	// 更新选中区域
	_prototype_board_update_selected_region(hObj);
}

/// <summary>
/// 重置路径绘制状态
/// </summary>
void _prototype_board_reset_path_drawing(HEXOBJ hObj)
{
	// 释放现有的路径点数组
	LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);
	if (pathPoints != 0)
	{
		free(pathPoints);
	}

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED, 0);
}

/// <summary>
/// 添加路径点到数组
/// </summary>
void _prototype_board_path_add_point(HEXOBJ hObj, FLOAT worldX, FLOAT worldY)
{
	BOOL pathClosed = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED);
	BOOL pathDrawing = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING);
	// 如果路径已闭合，重置状态开始新的路径
	if (pathClosed)
	{
		_prototype_board_reset_path_drawing(hObj);
		pathDrawing = FALSE;
		pathClosed = FALSE;
	}

	LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);
	INT pointsCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT);

	// 检查是否要闭合路径
	if (pointsCount >= 2)
	{
		// 检查是否靠近起始点（5px范围内）
		POINT startPoint = pathPoints[0];
		FLOAT distance = sqrt(pow(worldX - startPoint.x, 2) + pow(worldY - startPoint.y, 2));

		if (distance <= 5.0f)
		{
			// 闭合路径
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED, 1);

			// 创建路径形状
			_prototype_board_create_path_shape(hObj);

			// 清除临时点
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y, 0);

			// 但保持绘制模式，允许用户点击开始新的路径
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING, 0);
			return;
		}
	}

	// 添加新点
	if (pointsCount == 0)
	{
		// 第一个点
		pathPoints = (LPPOINT)malloc(sizeof(POINT));
		pathPoints[0].x = worldX;
		pathPoints[0].y = worldY;
		pointsCount = 1;
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING, 1);
	}
	else
	{
		// 后续点
		pathPoints = (LPPOINT)realloc(pathPoints, (pointsCount + 1) * sizeof(POINT));
		pathPoints[pointsCount].x = worldX;
		pathPoints[pointsCount].y = worldY;
		pointsCount++;
	}

	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS, (LONG_PTR)pathPoints);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT, pointsCount);

	// 清除临时点（会在鼠标移动时重新设置）
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y, 0);

	Ex_ObjInvalidateRect(hObj, 0);
}

/// <summary>
/// 移除路径最后一个点
/// </summary>
void _prototype_board_path_remove_last_point(HEXOBJ hObj)
{
	INT pointsCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT);

	if (pointsCount > 0)
	{
		LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);

		if (pointsCount == 1)
		{
			// 只有一个点，完全重置
			_prototype_board_reset_path_drawing(hObj);
			// 同时重置鼠标状态
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_X, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD_START_Y, 0);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_HOLD, 0);
		}
		else
		{
			// 移除最后一个点
			pointsCount--;
			pathPoints = (LPPOINT)realloc(pathPoints, pointsCount * sizeof(POINT));

			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS, (LONG_PTR)pathPoints);
			Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT, pointsCount);
		}
	}
}

/// <summary>
/// 创建路径形状
/// </summary>
void _prototype_board_create_path_shape(HEXOBJ hObj)
{
	LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);
	INT pointsCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT);
	BOOL pathClosed = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED);
	if (pointsCount < 2) return; // 至少需要2个点

	// 创建形状结构
	auto ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)malloc(sizeof(EX_PROTOTYPEBOARD_SHAPE));
	ptrShape->type = ShapeType::SHAPE_PATH;
	ptrShape->pathClosed = pathClosed;
	ptrShape->pathPointsCount = pointsCount;

	// 复制路径点数据
	ptrShape->pathPoints = (LPPOINT)malloc(pointsCount * sizeof(POINT));
	if (pathPoints != 0 && ptrShape->pathPoints != 0)
	{
		memcpy(ptrShape->pathPoints, pathPoints, pointsCount * sizeof(POINT));
	}

	// 创建路径
	HEXPATH path;
	_path_create(1, &path);
	_path_open(path);

	// 添加路径点
	if (pointsCount > 0)
	{
		_path_beginfigure2(path, pathPoints[0].x, pathPoints[0].y);

		for (INT i = 1; i < pointsCount; i++)
		{
			_path_addline(path, pathPoints[i - 1].x, pathPoints[i - 1].y,
				pathPoints[i].x, pathPoints[i].y);
		}

		// 如果是闭合路径，连接到起始点
		if (pathClosed)
		{
			_path_addline(path, pathPoints[pointsCount - 1].x, pathPoints[pointsCount - 1].y,
				pathPoints[0].x, pathPoints[0].y);
		}

		_path_endfigure(path, TRUE);
	}
	_path_close(path);


	EX_RECTF rc;
	_path_getbounds(path, &rc);
	ptrShape->path = path;
	ptrShape->boundingRect = rc;
	ptrShape->shapeLevel = _prototype_board_getshapecount(hObj);
	ptrShape->bSelect = FALSE;
	// 初始化其他字段
	ptrShape->currentLeft = rc.left;
	ptrShape->currentTop = rc.top;
	ptrShape->currentRight = rc.right;
	ptrShape->currentBottom = rc.bottom;
	// 取出图层
	auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	size_t layerValue = 0;
	memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
	EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

	// 形状添加到图层
	ptrLayer->shapes = realloc(ptrLayer->shapes, (ptrLayer->shapesCount + 1) * sizeof(size_t));
	size_t shapeValue = (size_t)ptrShape;
	memmove((LPVOID)((size_t)ptrLayer->shapes + ptrLayer->shapesCount * sizeof(size_t)), &shapeValue, sizeof(size_t));
	ptrLayer->shapesCount = ptrLayer->shapesCount + 1;
}

/// <summary>
/// 绘制路径预览
/// </summary>
void _prototype_board_draw_path_preview(HEXOBJ hObj, HEXCANVAS hCanvas, float scaleFactor, float canvasOffsetX, float canvasOffsetY, float scale, float scaleinit)
{
	LPPOINT pathPoints = (LPPOINT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS);
	INT pointsCount = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT);
	FLOAT tempX = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X);
	FLOAT tempY = (FLOAT)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y);
	BOOL pathClosed = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_CLOSED);
	BOOL pathDrawing = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_PATH_DRAWING);
	if (pointsCount > 0 && pathDrawing && !pathClosed)
	{
		// 获取当前图层的颜色
		auto layerIndex = Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYER_INDEX);
		auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

		auto hBrush = _brush_create(ptrLayer->color);
		auto hBrushTemp = _brush_create(ExARGB(255, 0, 0, 255)); // 红色用于临时线段

		HEXMATRIX mx = _matrix_create();
		_matrix_translate(mx, canvasOffsetX, canvasOffsetY);
		_matrix_scale(mx, (scale / scaleinit), (scale / scaleinit));
		_canvas_settransform(hCanvas, mx);

		// 绘制已确定的线段
		for (INT i = 0; i < pointsCount - 1; i++)
		{
			_canvas_drawline(hCanvas, hBrush,
				pathPoints[i].x, pathPoints[i].y,
				pathPoints[i + 1].x, pathPoints[i + 1].y,
				2, 0);
		}

		// 绘制临时线段（从最后一个点到鼠标位置）
		if (tempX != 0 || tempY != 0)
		{
			_canvas_drawline(hCanvas, hBrushTemp,
				pathPoints[pointsCount - 1].x, pathPoints[pointsCount - 1].y,
				tempX, tempY,
				2, 0); // 虚线表示临时线段

			// 检查是否靠近起始点（绘制闭合提示圆）
			if (pointsCount >= 2)
			{
				POINT startPoint = pathPoints[0];
				FLOAT distance = sqrt(pow(tempX - startPoint.x, 2) + pow(tempY - startPoint.y, 2));

				if (distance <= 5.0f)
				{
					// 绘制闭合提示圆
					auto hBrushCircle = _brush_create(ExARGB(255, 0, 0, 200)); // 红色半透明
					_canvas_drawellipse(hCanvas, hBrushCircle,
						startPoint.x, startPoint.y,
						5, 5, 2, 0);
					_brush_destroy(hBrushCircle);
				}
			}
		}
		// 绘制点标记
		for (INT i = 0; i < pointsCount; i++)
		{
			_canvas_drawellipse(hCanvas, hBrush,
				pathPoints[i].x, pathPoints[i].y,
				4, 4, 1, 0);
		}
		_canvas_settransform(hCanvas, 0);
		_matrix_destroy(mx);
		_brush_destroy(hBrush);
		_brush_destroy(hBrushTemp);
	}
}

/// <summary>
/// 清除所有选择状态
/// </summary>
void _prototype_board_clear_selections(HEXOBJ hObj)
{
	// 清除选中区域
	HEXRGN oldhRgn = (HEXRGN)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN);
	if (oldhRgn != 0)
	{
		_rgn_destroy(oldhRgn);
		Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_SELECT_RGN, 0);
	}

	// 清除所有形状的选中状态
	auto arr = (EX_PROTOTYPEBOARD_LAYER_ARRAY*)Ex_ObjGetLong(hObj, PROTOTYPEBOARD_LONG_LAYERS);
	for (int layerIndex = 0; layerIndex < arr->layersCount; layerIndex++)
	{
		size_t layerValue = 0;
		memmove(&layerValue, (LPVOID)((size_t)arr->layers + layerIndex * sizeof(size_t)), sizeof(size_t));
		EX_PROTOTYPEBOARD_LAYER* ptrLayer = (EX_PROTOTYPEBOARD_LAYER*)layerValue;

		for (int shapeIndex = 0; shapeIndex < ptrLayer->shapesCount; shapeIndex++)
		{
			size_t shapeValue = 0;
			memmove(&shapeValue, (LPVOID)((size_t)ptrLayer->shapes + shapeIndex * sizeof(size_t)), sizeof(size_t));
			EX_PROTOTYPEBOARD_SHAPE* ptrShape = (EX_PROTOTYPEBOARD_SHAPE*)shapeValue;
			ptrShape->bSelect = FALSE;
		}
	}

	// 清除选中图形指针数组
	_prototype_board_free_selected_shapes(hObj);

	// 清除调整大小相关状态
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_MODE, 0);
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_RESIZE_CORNER, -1);

	// 清除拖动相关状态
	Ex_ObjSetLong(hObj, PROTOTYPEBOARD_LONG_DRAGGING_SELECTED, 0);

	// 重绘画布
	Ex_ObjInvalidateRect(hObj, 0);
}