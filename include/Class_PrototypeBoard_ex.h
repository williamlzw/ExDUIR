#pragma once

const std::vector<EXARGB> PROTOTYPEBOARD_LAYER_COLORS = { ExARGB(35, 102, 255, 255) };

enum ShapeType {
	SHAPE_RECTANGLE,
	SHAPE_ELLIPSE,
	SHAPE_LINE,
	SHAPE_TEXT,
	SHAPE_IMAGE,
	SHAPE_PATH
} ;

// 原型画板图层数组结构
struct EX_PROTOTYPEBOARD_LAYER_ARRAY
{
	/// <summary>
	/// 图层个数
	/// </summary>
	DWORD layersCount;
	LPVOID layers;//保存有layersCount个EX_PROTOTYPEBOARD_LAYER*指针
};

/// <summary>
/// 原型画板图层信息结构
/// </summary>
struct EX_PROTOTYPEBOARD_LAYER
{
	/// <summary>
	/// 形状个数
	/// </summary>
	DWORD shapesCount;
	/// <summary>
	/// 图层颜色
	/// </summary>
	EXARGB color;
	/// <summary>
	/// 图层索引
	/// </summary>
	DWORD index;
	/// <summary>
	/// 保存有shapesCount个EX_PROTOTYPEBOARD_SHAPE*指针
	/// </summary>
	LPVOID shapes; //
};

/// <summary>
/// 原型画板形状信息结构
/// </summary>
struct EX_PROTOTYPEBOARD_SHAPE
{
	/// <summary>
	/// 形状类型
	/// </summary>
	ShapeType type;
	/// <summary>
	/// 形状层次
	/// </summary>
	INT shapeLevel;
	/// <summary>
	/// 形状路径
	/// </summary>
	HEXPATH path;
	/// <summary>
	/// 包围矩形
	/// </summary>
	EX_RECTF boundingRect;
	/// <summary>
	/// 是否被选中
	/// </summary>
	BOOL bSelect;
	/// <summary>
	/// 实时坐标左边（拖动时需要）
	/// </summary>
	FLOAT currentLeft;
	/// <summary>
	/// 实时坐标顶边
	/// </summary>
	FLOAT currentTop;
	/// <summary>
	/// 实时坐标右边
	/// </summary>
	FLOAT currentRight;
	/// <summary>
	/// 实时坐标底边
	/// </summary>
	FLOAT currentBottom;
	/// <summary>
	/// 椭圆实时中心横坐标
	/// </summary>
	FLOAT currentCenterX;
	/// <summary>
	/// 椭圆实时中心纵坐标
	/// </summary>
	FLOAT currentCenterY;
	/// <summary>
	/// 椭圆实时半径X
	/// </summary>
	FLOAT currentRadiusX;
	/// <summary>
	/// 椭圆实时半径Y
	/// </summary>
	FLOAT currentRadiusY;
	/// <summary>
	/// 直线实时起点X1
	/// </summary>
	FLOAT currentX1;
	/// <summary>
	/// 直线实时起点Y1
	/// </summary>
	FLOAT currentY1;
	/// <summary>
	/// 直线实时终点X2
	/// </summary>
	FLOAT currentX2;
	/// <summary>
	/// 直线实时终点Y2
	/// </summary>
	FLOAT currentY2;
	/// <summary>
	/// 图片句柄（如果有）
	/// </summary>
	HEXIMAGE imageHandle;
	/// <summary>
	/// 图片原始宽度
	/// </summary>
	INT imageWidth;
	/// <summary>
	/// 图片原始高度
	/// </summary>
	INT imageHeight;
	/// <summary>
	/// 是否保持图片原始宽高比
	/// </summary>
	BOOL keepAspectRatio;
	/// <summary>
	/// 文本内容
	/// </summary>
	WCHAR* textContent;
	/// <summary>
	/// 字体句柄
	/// </summary>
	HEXFONT textFont;
	/// <summary>
	/// 文本颜色
	/// </summary>
	EXARGB textColor;
	/// <summary>
	/// 文本格式（对齐方式等）
	/// </summary>
	DWORD textFormat;
	/// <summary>
	/// 文本大小（备用）
	/// </summary>
	INT textSize;
	/// <summary>
	/// 路径点数组（仅对PATH类型有效）
	/// </summary>
	LPPOINT pathPoints;
	/// <summary>
	/// 路径点数量（仅对PATH类型有效）
	/// </summary>
	INT pathPointsCount;
	/// <summary>
	/// 路径是否闭合（仅对PATH类型有效）
	/// </summary>
	BOOL pathClosed;
};

// 原型画板属性_画布偏移_横向
#define PROTOTYPEBOARD_LONG_OFFSET_CANVAS_X 0
// 原型画板属性_画布偏移_纵向
#define PROTOTYPEBOARD_LONG_OFFSET_CANVAS_Y 1
// 原型画板属性_画布宽度
#define PROTOTYPEBOARD_LONG_CANVAS_WIDTH 2
// 原型画板属性_画布高度
#define PROTOTYPEBOARD_LONG_CANVAS_HEIGHT 3
// 原型画板属性_缩放比例
#define PROTOTYPEBOARD_LONG_SCALE_FACTOR 4
// 原型画板属性_初始缩放比例_用于画布缩放
#define PROTOTYPEBOARD_LONG_SCALE_FACTOR_INIT 5
// 原型画板属性_拖动模式_PROTOTYPEBOARD_MODE_
#define PROTOTYPEBOARD_LONG_MODE 6
// 原型画板属性_按住状态_1按住_0释放
#define PROTOTYPEBOARD_LONG_HOLD 7
// 原型画板属性_按住起始横坐标
#define PROTOTYPEBOARD_LONG_HOLD_START_X 8
// 原型画板属性_按住起始纵坐标
#define PROTOTYPEBOARD_LONG_HOLD_START_Y 9
// 原型画板属性_按住移动横坐标
#define PROTOTYPEBOARD_LONG_HOLD_X 10
// 原型画板属性_按住移动纵坐标
#define PROTOTYPEBOARD_LONG_HOLD_Y 11
// 原型画板属性_按住偏移横坐标_拖动画布使用
#define PROTOTYPEBOARD_LONG_HOLD_OFFSET_X 12
// 原型画板属性_按住偏移纵坐标_拖动画布使用
#define PROTOTYPEBOARD_LONG_HOLD_OFFSET_Y 13
// 原型画板属性_绘制类型_-1无绘制_0直线_1矩形_2椭圆_3文本_4图像_5路径_6选中区域
#define PROTOTYPEBOARD_LONG_DRAW_TYPE 14
// 原型画板属性_图层信息指针
#define PROTOTYPEBOARD_LONG_LAYERS 15
// 原型画板属性_当前图层索引
#define PROTOTYPEBOARD_LONG_LAYER_INDEX 16
// 原型画板属性_临时形状画布
#define PROTOTYPEBOARD_LONG_CANVAS_TEMP 17
// 原型画板属性_拉框区域画布
#define PROTOTYPEBOARD_LONG_CANVAS_SELECT 18
// 原型画板属性_选中区域画布
#define PROTOTYPEBOARD_LONG_CANVAS_CHOICE 19
// 原型画板属性_背景形状画布
#define PROTOTYPEBOARD_LONG_CANVAS_BACKGROUND 20
// 原型画板属性_点燃图层索引
#define PROTOTYPEBOARD_LONG_HOVER_LAYER_INDEX 21
// 原型画板属性_点燃形状索引
#define PROTOTYPEBOARD_LONG_HOVER_SHAPE_INDEX 22
// 原型画板属性_选中图层索引
#define PROTOTYPEBOARD_LONG_SELECT_LAYER_INDEX 23
// 原型画板属性_选中形状索引
#define PROTOTYPEBOARD_LONG_SELECT_SHAPE_INDEX 24
// 原型画板属性_选中区域句柄
#define PROTOTYPEBOARD_LONG_SELECT_RGN 25
// 原型画板属性_拖动选中图形标志
#define PROTOTYPEBOARD_LONG_DRAGGING_SELECTED  26
// 原型画板属性_拖动选中图形横坐标
#define PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_X 27
// 原型画板属性_拖动选中图形纵坐标
#define PROTOTYPEBOARD_LONG_LAST_DRAG_WORLD_Y 28
// 原型画板属性_拖动选中图形包围盒中心X
#define PROTOTYPEBOARD_LONG_SELECTED_CENTER_X 29
// 原型画板属性_拖动选中图形包围盒中心Y
#define PROTOTYPEBOARD_LONG_SELECTED_CENTER_Y 30
// 原型画板属性_拖动选中图形偏移X
#define PROTOTYPEBOARD_LONG_SELECTED_OFFSET_X 31
// 原型画板属性_拖动选中图形偏移Y
#define PROTOTYPEBOARD_LONG_SELECTED_OFFSET_Y 32
// 原型画板属性_拖动选中图形个数
#define PROTOTYPEBOARD_LONG_SELECTED_SHAPES_COUNT 33
// 原型画板属性_存储选中图形的指针数组
#define PROTOTYPEBOARD_LONG_SELECTED_SHAPES 34
// 原型画板属性_调整大小模式_0否_1是
#define PROTOTYPEBOARD_LONG_RESIZE_MODE 35
// 原型画板属性_调整大小角点_CORNER_NONE_-1_CORNER_TOP_LEFT_0_CORNER_TOP_RIGHT_1_CORNER_BOTTOM_RIGHT_2_CORNER_BOTTOM_LEFT_3
#define PROTOTYPEBOARD_LONG_RESIZE_CORNER 36
// 原型画板属性_调整大小起始横坐标
#define PROTOTYPEBOARD_LONG_RESIZE_START_X 37
// 原型画板属性_调整大小起始纵坐标
#define PROTOTYPEBOARD_LONG_RESIZE_START_Y 38
// 原型画板属性_调整大小起始左边
#define PROTOTYPEBOARD_LONG_RESIZE_START_LEFT 39
// 原型画板属性_调整大小起始顶边
#define PROTOTYPEBOARD_LONG_RESIZE_START_TOP 40
// 原型画板属性_调整大小起始右边
#define PROTOTYPEBOARD_LONG_RESIZE_START_RIGHT 41
// 原型画板属性_调整大小起始底边
#define PROTOTYPEBOARD_LONG_RESIZE_START_BOTTOM 42
// 原型画板属性_调整大小起始中心X
#define PROTOTYPEBOARD_LONG_RESIZE_START_CENTERX 43
// 原型画板属性_调整大小起始中心Y
#define PROTOTYPEBOARD_LONG_RESIZE_START_CENTERY 44
// 原型画板属性_调整大小起始半径X
#define PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSX 45
// 原型画板属性_调整大小起始半径Y
#define PROTOTYPEBOARD_LONG_RESIZE_START_RADIUSY 46
// 原型画板属性_调整大小起始直线起点X
#define PROTOTYPEBOARD_LONG_RESIZE_START_X1 47
// 原型画板属性_调整大小起始直线起点Y
#define PROTOTYPEBOARD_LONG_RESIZE_START_Y1 48
// 原型画板属性_调整大小起始直线终点X
#define PROTOTYPEBOARD_LONG_RESIZE_START_X2 49
// 原型画板属性_调整大小起始直线终点Y
#define PROTOTYPEBOARD_LONG_RESIZE_START_Y2 50
// 原型画板属性_路径绘制点数组
#define PROTOTYPEBOARD_LONG_PATH_POINTS 51
// 原型画板属性_路径绘制点个数
#define PROTOTYPEBOARD_LONG_PATH_POINTS_COUNT 52
// 原型画板属性_临时路径终点
#define PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_X 53
#define PROTOTYPEBOARD_LONG_PATH_TEMP_POINT_Y 54
// 原型画板属性_路径绘制中标志
#define PROTOTYPEBOARD_LONG_PATH_DRAWING 55
// 原型画板属性_路径是否闭合
#define PROTOTYPEBOARD_LONG_PATH_CLOSED 56
// 原型画板属性_按住起始世界坐标横坐标
#define PROTOTYPEBOARD_LONG_HOLD_START_WORLD_X 57
// 原型画板属性_按住起始世界坐标纵坐标
#define PROTOTYPEBOARD_LONG_HOLD_START_WORLD_Y 58


// 控制点大小
#define RESIZE_HANDLE_SIZE 10

// 控制点位置枚举
enum ResizeCorner {
	CORNER_NONE = -1,
	CORNER_TOP_LEFT = 0,
	CORNER_TOP_RIGHT = 1,
	CORNER_BOTTOM_RIGHT = 2,
	CORNER_BOTTOM_LEFT = 3
};

void _prototype_board_register();
LRESULT CALLBACK _prototype_board_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _prototype_board_paint(HEXOBJ hObj);
void _prototype_board_paint_mark(EX_PAINTSTRUCT ps, float scaleFactor, float canvasOffsetX, float canvasOffsetY);
void _prototype_board_paint_background(EX_PAINTSTRUCT ps, float scaleFactor, float canvasWidth, float canvasHeight, float canvasOffsetX, float canvasOffsetY);
void _prototype_board_update_selectcanvas(HEXOBJ hObj);
void _prototype_board_update_choicecanvas(HEXOBJ hObj);
void _prototype_board_update_tempcanvas(HEXOBJ hObj);
void _prototype_board_update_bkgcanvas(HEXOBJ hObj);
void _prototype_board_addrecttoshape(HEXOBJ hObj, int left, int top, int right, int bottom);
void _prototype_board_addlinetoshape(HEXOBJ hObj, int x1, int y1, int x2, int y2);
void _prototype_board_addellipsetoshape(HEXOBJ hObj, int x, int y, int radiusX, int radiusY);
void _prototype_board_addimagetoshape(HEXOBJ hObj, int left, int top, int right, int bottom);
void _prototype_board_calc_scale(HEXOBJ hObj, int width, int height, float scale = 1.0f);
void _prototype_board_calc_size(HEXOBJ hObj, int width, int height);
void _prototype_board_leftbuttonup(HEXOBJ hObj, LPARAM lParam);
void _prototype_board_leftbuttondown(HEXOBJ hObj, LPARAM lParam);
void _prototype_board_mousemove(HEXOBJ hObj, LPARAM lParam);
void _prototype_board_calc_selectregion(HEXOBJ hObj, int left, int top, int right, int bottom);

void _prototype_board_initparam(HEXOBJ hObj);
void _prototype_board_uninitparam(HEXOBJ hObj);
void _prototype_board_set_canvaswidth(HEXOBJ hObj, float width);
float _prototype_board_get_canvaswidth(HEXOBJ hObj);
void _prototype_board_set_canvasheight(HEXOBJ hObj, float height);
float _prototype_board_get_canvasheight(HEXOBJ hObj);
void _prototype_board_set_canvasoffsetx(HEXOBJ hObj, float offsetX);
float _prototype_board_get_canvasoffsetx(HEXOBJ hObj);
void _prototype_board_set_canvasoffsety(HEXOBJ hObj, float offsetY);
float _prototype_board_get_canvasoffsety(HEXOBJ hObj);
float _prototype_board_get_scalefactor(HEXOBJ hObj);
void _prototype_board_set_scalefactor(HEXOBJ hObj, float scaleFactor);
void _prototype_board_set_scalefactorinit(HEXOBJ hObj, float scaleFactor);
float _prototype_board_get_scalefactorinit(HEXOBJ hObj);
void _prototype_board_set_cursor(HEXOBJ hObj, int value);
INT _prototype_board_getshapecount(HEXOBJ hObj);
void _prototype_board_update_selected_region(HEXOBJ hObj);
void _prototype_board_free_selected_shapes(HEXOBJ hObj);
void _prototype_board_collect_selected_shapes(HEXOBJ hObj);
void _prototype_board_update_selected_shapes_position(HEXOBJ hObj, FLOAT mouseWorldX, FLOAT mouseWorldY);
void _prototype_board_set_image_to_selected(HEXOBJ hObj, HEXIMAGE hImage, BOOL keepAspectRatio);
void _prototype_board_addtexttoshape(HEXOBJ hObj, int left, int top, int right, int bottom);
void _prototype_board_set_text_to_selected(HEXOBJ hObj, HEXFONT hFont, LPCWSTR textContent);

ResizeCorner _prototype_board_hittest_resize_handle(HEXOBJ hObj, FLOAT worldX, FLOAT worldY, EX_PROTOTYPEBOARD_SHAPE* shape);
EX_PROTOTYPEBOARD_SHAPE* _prototype_board_get_single_selected_shape(HEXOBJ hObj);
void _prototype_board_resize_shape(HEXOBJ hObj, EX_PROTOTYPEBOARD_SHAPE* shape, INT corner, FLOAT deltaX, FLOAT deltaY);
void _prototype_board_create_path_shape(HEXOBJ hObj);
void _prototype_board_path_remove_last_point(HEXOBJ hObj);
void _prototype_board_path_add_point(HEXOBJ hObj, FLOAT worldX, FLOAT worldY);
void _prototype_board_reset_path_drawing(HEXOBJ hObj);
void _prototype_board_draw_path_preview(HEXOBJ hObj, HEXCANVAS hCanvas, float scaleFactor, float canvasOffsetX, float canvasOffsetY, float scale, float scaleinit);
void _prototype_board_clear_selections(HEXOBJ hObj);
