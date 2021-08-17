#pragma once
#include "ExDUIR_Struct.h"

/*
* @fnType ExDirectUI
* @brief _brush_create 画刷创建
* @param 1  argb                    [EXARGB]
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_create(EXARGB argb);

/*
* @fnType ExDirectUI
* @brief _brush_createfromcanvas 画刷创建自画布
* @param 1  hCanvas                 [HEXCANVAS]
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_createfromcanvas(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _brush_createfromcanvas 画刷创建自画布2
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  alpha                   [DWORD]
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_createfromcanvas2(HEXCANVAS hCanvas, DWORD alpha);

/*
* @fnType ExDirectUI
* @brief _brush_createfromimg 画刷创建自图片句柄
* @param 1  hImg                    [HEXIMAGE]
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_createfromimg(HEXIMAGE hImg);

/*
* @fnType ExDirectUI
* @brief _brush_createlinear 创建线性渐变画刷
* @param 1  xStart                      [FLOAT]               起点X
* @param 2  yStart                      [FLOAT]               起点Y
* @param 3  xEnd						[FLOAT]               终点X
* @param 4  yEnd						[FLOAT]               终点Y
* @param 5  crBegin						[EXARGB]              起点颜色ARGB
* @param 6  crEnd						[EXARGB]              终点颜色ARGB
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_createlinear(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, EXARGB crBegin, EXARGB crEnd);

/*
* @fnType ExDirectUI
* @brief _brush_createlinear_ex 创建线性渐变画刷2
* @param 1  xStart                    [FLOAT]               起点X
* @param 2  yStart                    [FLOAT]               起点Y
* @param 3  xEnd                      [FLOAT]               终点X
* @param 4  yEnd                      [FLOAT]               终点Y
* @param 5  arrStopPts				  [INT*]                停止点指针,{位置(0-100),颜色(ARGB),位置,颜色,...}
* @param 6  cStopPts                  [INT]                 停止点个数
* @return [HEXBRUSH]返回画刷句柄
*/
HEXBRUSH _brush_createlinear_ex(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd, const INT* arrStopPts, INT cStopPts);

/*
* @fnType ExDirectUI
* @brief _brush_destroy 画刷销毁
* @param 1  hBrush                  [HEXBRUSH]
* @return [BOOL]返回是否成功
*/
BOOL _brush_destroy(HEXBRUSH hBrush);

/*
* @fnType ExDirectUI
* @brief _brush_setcolor 画刷置颜色
* @param 1  hBrush                  [HEXBRUSH]
* @param 2  argb                    [EXARGB]
* @return [EXARGB]返回旧颜色
*/
EXARGB _brush_setcolor(HEXBRUSH hBrush, EXARGB argb);

/*
* @fnType ExDirectUI
* @brief _brush_settransform 画刷置矩阵
* @param 1  hBrush                  [HEXBRUSH]
* @param 2  matrix                  [HEXMATRIX]
* @return [void]
*/
void _brush_settransform(HEXBRUSH hBrush, HEXMATRIX matrix);

/*
* @fnType ExDirectUI
* @brief _canvas_begindraw 画布开始绘制
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_begindraw(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_blur 画布模糊
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  fDeviation              [FLOAT]
* @param 3  lprc                    [RECT*]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_blur(HEXCANVAS hCanvas, FLOAT fDeviation, RECT* lprc);

/*
* @fnType ExDirectUI
* @brief _canvas_calctextsize 画布计算文本尺寸
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hFont                   [HEXFONT]
* @param 3  lpwzText                [LPCWSTR]
* @param 4  dwLen                   [LONG_PTR]
* @param 5  dwDTFormat              [INT]
* @param 6  lParam                  [LPARAM]
* @param 7  layoutWidth             [FLOAT]
* @param 8  layoutHeight            [FLOAT]
* @param 9  lpWidth                 [FLOAT*]              小数型 取指针_通用型
* @param 10 lpHeight                [FLOAT*]              小数型 取指针_通用型
* @return [BOOL]返回是否成功
*/
BOOL _canvas_calctextsize(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, LPARAM lParam, FLOAT layoutWidth, FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight);

/*
* @fnType ExDirectUI
* @brief _canvas_clear 画布清除背景
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  Color                   [EXARGB]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_clear(HEXCANVAS hCanvas, EXARGB Color);

/*
* @fnType ExDirectUI
* @brief _canvas_cliprect 画布置剪辑区
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  left                    [INT]
* @param 3  top                     [INT]
* @param 4  right                   [INT]
* @param 5  bottom                  [INT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_cliprect(HEXCANVAS hCanvas, INT left, INT top, INT right, INT bottom);

/*
* @fnType ExDirectUI
* @brief _canvas_createfromexdui 画布创建自引擎句柄
* @param 1  hExDui                  [HEXDUI]
* @param 2  width                   [INT]
* @param 3  height                  [INT]
* @param 4  dwFlags                 [INT]               CVF_
* @return [HEXCANVAS]返回画布句柄
*/
HEXCANVAS _canvas_createfromexdui(HEXDUI hExDui, INT width, INT height, INT dwFlags);

/*
* @fnType ExDirectUI
* @brief _canvas_createfromobj 创建画布自组件句柄。
* @param 1  hObj                    [HEXOBJ]
* @param 2  uWidth                  [INT]
* @param 3  uHeight                 [INT]
* @param 4  dwFlags                 [DWORD]               常量CVF_
* @return [HEXCANVAS]成功返回画布句柄，失败返回0
*/
HEXCANVAS _canvas_createfromobj(HEXOBJ hObj, INT uWidth, INT uHeight, INT dwFlags);

/*
* @fnType ExDirectUI
* @brief _canvas_destroy 销毁画布
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_destroy(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_drawcanvas 画布画画布
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  sCanvas                 [HEXCANVAS]
* @param 3  dstLeft                 [INT]
* @param 4  dstTop                  [INT]
* @param 5  dstRight                [INT]
* @param 6  dstBottom               [INT]
* @param 7  srcLeft                 [INT]
* @param 8  srcTop                  [INT]
* @param 9  dwAlpha                 [DWORD]
* @param 10 dwCompositeMode         [DWORD]                 #CV_COMPOSITE_MODE_
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawcanvas(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight, INT dstBottom, INT srcLeft, INT srcTop, DWORD dwAlpha, DWORD dwCompositeMode);

/*
* @fnType ExDirectUI
* @brief _canvas_drawellipse 画布画椭圆
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  x                       [FLOAT]
* @param 4  y                       [FLOAT]
* @param 5  radiusX                 [FLOAT]
* @param 6  radiusY                 [FLOAT]
* @param 7  strokeWidth             [FLOAT]
* @param 8  strokeStyle             [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle);

/*
* @fnType ExDirectUI
* @brief _canvas_drawimage 画布画图片
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hImage                  [HEXIMAGE]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  alpha                   [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawimage(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, DWORD alpha);

/*
* @fnType ExDirectUI
* @brief _canvas_drawimagefromgrid 画布画九宫矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hImage                  [HEXIMAGE]
* @param 3  dstLeft                 [FLOAT]
* @param 4  dstTop                  [FLOAT]
* @param 5  dstRight                [FLOAT]
* @param 6  dstBottom               [FLOAT]
* @param 7  srcLeft                 [FLOAT]
* @param 8  srcTop                  [FLOAT]
* @param 9  srcRight                [FLOAT]
* @param 10 srcBottom               [FLOAT]
* @param 11 gridPaddingLeft         [FLOAT]
* @param 12 gridPaddingTop          [FLOAT]
* @param 13 gridPaddingRight        [FLOAT]
* @param 14 gridPaddingBottom       [FLOAT]
* @param 15 dwFlags                 [INT]               #BIF_
* @param 16 dwAlpha                 [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawimagefromgrid(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, FLOAT gridPaddingLeft, FLOAT gridPaddingTop, FLOAT gridPaddingRight,
	FLOAT gridPaddingBottom, INT dwFlags, DWORD dwAlpha);

/*
* @fnType ExDirectUI
* @brief _canvas_drawimagerect 画布画图像矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hImage                  [HEXIMAGE]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  right                   [FLOAT]
* @param 6  bottom                  [FLOAT]
* @param 7  alpha                   [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawimagerect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left, FLOAT Top, FLOAT Right, FLOAT Bottom, DWORD alpha);

/*
* @fnType ExDirectUI
* @brief _canvas_drawimagerectrect 画布画图像缩放到
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hImage                  [HEXIMAGE]
* @param 3  dstLeft                 [FLOAT]
* @param 4  dstTop                  [FLOAT]
* @param 5  dstRight                [FLOAT]
* @param 6  dstBottom               [FLOAT]
* @param 7  srcLeft                 [FLOAT]
* @param 8  srcTop                  [FLOAT]
* @param 9  srcRight                [FLOAT]
* @param 10 srcBottom               [FLOAT]
* @param 11 alpha                   [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawimagerectrect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	FLOAT srcLeft, FLOAT srcTop, FLOAT srcRight, FLOAT srcBottom, DWORD alpha);

/*
* @fnType ExDirectUI
* @brief _canvas_drawline 画布画直线
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  X1                      [FLOAT]
* @param 4  Y1                      [FLOAT]
* @param 5  X2                      [FLOAT]
* @param 6  Y2                      [FLOAT]
* @param 7  strokeWidth             [FLOAT]
* @param 8  strokeStyle             [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawline(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT X1, FLOAT Y1, FLOAT X2, FLOAT Y2, FLOAT strokeWidth, DWORD strokeStyle);

/*
* @fnType ExDirectUI
* @brief _canvas_drawpath 画布画路径
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hPath                   [HEXPATH]
* @param 3  hBrush                  [HEXBRUSH]
* @param 4  strokeWidth             [FLOAT]
* @param 5  strokeStyle             [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush, FLOAT strokeWidth, DWORD strokeStyle);

/*
* @fnType ExDirectUI
* @brief _canvas_drawrect 画布画矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  right                   [FLOAT]
* @param 6  bottom                  [FLOAT]
* @param 7  strokeWidth             [FLOAT]
* @param 8  strokeStyle             [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT strokeWidth, DWORD strokeStyle);

/*
* @fnType ExDirectUI
* @brief _canvas_drawroundedrect 画布画圆角矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  right                   [FLOAT]
* @param 6  bottom                  [FLOAT]
* @param 7  radiusX                 [FLOAT]			横向圆角度
* @param 8  radiusY                 [FLOAT]			纵向圆角度
* @param 9  strokeWidth             [FLOAT]			画刷宽度
* @param 10 strokeStyle             [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth, DWORD strokeStyle);

/*
* @fnType ExDirectUI
* @brief _canvas_drawtext 画布画文本
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hFont                   [HEXFONT]             如果为0则使用默认字体句柄
* @param 3  crText                  [EXARGB]
* @param 4  lpwzText                [LPCWSTR]
* @param 5  dwLen                   [LONG_PTR]              -1则自动计算尺寸(必须是指向空字符串的指针)
* @param 6  dwDTFormat              [INT]               #DT_
* @param 7  left                    [FLOAT]
* @param 8  top                     [FLOAT]
* @param 9  right                   [FLOAT]
* @param 10 bottom                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawtext(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/*
* @fnType ExDirectUI
* @brief _canvas_drawtext2 画布画文本
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hFont                   [HEXFONT]             如果为0则使用默认字体句柄
* @param 3  hBrush                  [HEXBRUSH]
* @param 4  lpwzText                [LPCWSTR]
* @param 5  dwLen                   [LONG_PTR]              -1则自动计算尺寸(必须是指向空字符串的指针)
* @param 6  dwDTFormat              [INT]               #DT_
* @param 7  left                    [FLOAT]
* @param 8  top                     [FLOAT]
* @param 9  right                   [FLOAT]
* @param 10 bottom                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawtext2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBrush, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/*
* @fnType ExDirectUI
* @brief _canvas_drawtextex 画布画文本Ex
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hFont                   [HEXFONT]
* @param 3  crText                  [EXARGB]
* @param 4  lpwzText                [LPCWSTR]
* @param 5  dwLen                   [LONG_PTR]              -1则自动计算尺寸(必须是指向空字符串的指针)
* @param 6  dwDTFormat              [INT]
* @param 7  left                    [FLOAT]
* @param 8  top                     [FLOAT]
* @param 9  right                   [FLOAT]
* @param 10 bottom                  [FLOAT]
* @param 11 iGlowsize               [INT]
* @param 12 crShadow                [EXARGB]
* @param 13 lParam                  [LPARAM]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_drawtextex(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText, LPCWSTR lpwzText, LONG_PTR dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, INT iGlowsize, EXARGB crShadom, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief _canvas_enddraw 画布结束绘制
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_enddraw(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_fillellipse 画布填充椭圆
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  x                       [FLOAT]
* @param 4  y                       [FLOAT]
* @param 5  radiusX                 [FLOAT]
* @param 6  radiusY                 [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY);

/*
* @fnType ExDirectUI
* @brief _canvas_fillpath 画布填充路径
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hPath                   [HEXPATH]
* @param 3  hBrush                  [HEXBRUSH]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush);

/*
* @fnType ExDirectUI
* @brief _canvas_fillrect 画布填充矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  right                   [FLOAT]
* @param 6  bottom                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/*
* @fnType ExDirectUI
* @brief _canvas_fillregion 画布填充区域
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hRgn                    [HEXRGN]
* @param 3  hBrush                  [HEXBRUSH]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush);

/*
* @fnType ExDirectUI
* @brief _canvas_fillroundedimage 画布填充圆角图片
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  Width                   [FLOAT]
* @param 6  Height                  [FLOAT]
* @param 7  RadiuX                  [FLOAT]
* @param 8  RadiuY                  [FLOAT]
* @param 9  shadowNum               [INT*]		透明度数组
* @param 10	number					[INT]		透明度数量
* @param 11 crShadow                [EXARGB]]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillroundedimage(HEXCANVAS hCanvas, HEXIMAGE hImg, FLOAT left, FLOAT top, FLOAT Width, FLOAT Height, FLOAT RadiuX, FLOAT RadiuY, INT* shadowNum, INT number, EXARGB crShadow);

/*
* @fnType ExDirectUI
* @brief _canvas_fillroundedrect 画布填充圆角矩形
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  hBrush                  [HEXBRUSH]
* @param 3  left                    [FLOAT]
* @param 4  top                     [FLOAT]
* @param 5  right                   [FLOAT]
* @param 6  bottom                  [FLOAT]
* @param 7  radiusX                 [FLOAT]
* @param 8  radiusY                 [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_fillroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY);

/*
* @fnType ExDirectUI
* @brief _canvas_flush 画布刷新
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_flush(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_getcontext 获取画布信息
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  nType                   [INT]                 画布信息类型 CVC_DX常量
* @return [LPVOID]
*/
LPVOID _canvas_getcontext(HEXCANVAS hCanvas, INT nType);

/*
* @fnType ExDirectUI
* @brief _canvas_getdc 画布取DC
* @param 1  hCanvas                 [HEXCANVAS]
* @return [HDC]返回HDC
*/
HDC _canvas_getdc(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_getsize 画布取尺寸
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  width                   [INT*]
* @param 3  height                  [INT*]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_getsize(HEXCANVAS hCanvas, INT* width, INT* height);

/*
* @fnType ExDirectUI
* @brief _canvas_releasedc 画布释放DC
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_releasedc(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_resetclip 画布重置剪辑区
* @param 1  hCanvas                 [HEXCANVAS]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_resetclip(HEXCANVAS hCanvas);

/*
* @fnType ExDirectUI
* @brief _canvas_resize 重新设置尺寸。
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  width                   [INT]
* @param 3  height                  [INT]
* @return [BOOL]返回是否成功
*/
BOOL _canvas_resize(HEXCANVAS hCanvas, INT width, INT height);

/*
* @fnType ExDirectUI
* @brief _canvas_rotate_hue 画布旋转色相
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  fAngle                  [FLOAT]               0-360
* @return [BOOL]返回是否成功
*/
BOOL _canvas_rotate_hue(HEXCANVAS hCanvas, FLOAT fAngle);

/*
* @fnType ExDirectUI
* @brief _canvas_setantialias 设置画布图形抗锯齿
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  antialias               [BOOL]                是否抗锯齿
* @return [BOOL]返回是否成功
*/
BOOL _canvas_setantialias(HEXCANVAS hCanvas, BOOL antialias);

/*
* @fnType ExDirectUI
* @brief _canvas_setimageantialias 设置画布图像抗锯齿,等效于同时设置文本和图形抗锯齿
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  antialias               [BOOL]                是否抗锯齿
* @return [BOOL]返回是否成功
*/
BOOL _canvas_setimageantialias(HEXCANVAS hCanvas, BOOL antialias);

/*
* @fnType ExDirectUI
* @brief _canvas_settextantialiasmode 设置画布文本抗锯齿模式
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  antialias               [BOOL]                 是否抗锯齿
* @return [BOOL]返回是否成功
*/
BOOL _canvas_settextantialiasmode(HEXCANVAS hCanvas, BOOL antialias);

/*
* @fnType ExDirectUI
* @brief _canvas_settransform 画布置矩阵
* @param 1  hCanvas                 [HEXCANVAS]
* @param 2  pMatrix                 [HEXMATRIX]           0.则重置
* @return [BOOL]返回是否成功
*/
BOOL _canvas_settransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix);

/*
* @fnType ExDirectUI
* @brief _easing_create 缓动创建
* @param 1  dwType                  [DWORD]               常量ET_
* @param 2  pEasingContext          [LPVOID]
* @param 3  dwMode                  [DWORD]               常量ES_的组合
* @param 4  pContext                [LONG_PTR]			  描述表,可以是值或回调地址
* @param 5  nTotalTime              [INT]                 ms
* @param 6  nInterval               [INT]                 ms
* @param 7  nState                  [DWORD]               常量EES_
* @param 8  nStart                  [INT]
* @param 9  nStop                   [INT]
* @param 10 param1                  [LONG_PTR]
* @param 11 param2                  [LONG_PTR]
* @param 12 param3                  [LONG_PTR]
* @param 13 param4                  [LONG_PTR]
* @return [HEXEASING]返回缓动句柄
*/
HEXEASING _easing_create(DWORD dwType, LPVOID pEasingContext, DWORD dwMode, LONG_PTR pContext, INT nTotalTime, INT nInterval, DWORD nState, INT nStart, INT nStop, LONG_PTR param1, LONG_PTR param2, LONG_PTR param3, LONG_PTR param4);

/*
* @fnType ExDirectUI
* @brief _easing_getstate 缓动取状态
* @param 1  pEasing                 [HEXEASING]
* @return [DWORD]返回状态
*/
DWORD _easing_getstate(HEXEASING pEasing);

/*
* @fnType ExDirectUI
* @brief _easing_setstate 缓动置状态
* @param 1  pEasing                 [HEXEASING]
* @param 2  nState                  [DWORD]               EES_
* @return [BOOL]返回是否成功
*/
BOOL _easing_setstate(HEXEASING pEasing, DWORD nState);

/*
* @fnType ExDirectUI
* @brief _font_create 创建默认字体
* @return [HEXFONT]返回字体句柄
*/
HEXFONT _font_create();

/*
* @fnType ExDirectUI
* @brief _font_createfromfamily 创建字体自字体族
* @param 1  lpwzFontFace            [LPCWSTR]
* @param 2  dwFontSize              [INT]
* @param 3  dwFontStyle             [DWORD]
* @return [HEXFONT]返回字体句柄
*/
HEXFONT _font_createfromfamily(LPCWSTR lpwzFontFace, INT dwFontSize, DWORD dwFontStyle);

/*
* @fnType ExDirectUI
* @brief _font_createfromlogfont 创建字体自逻辑字体
* @param 1  lpLogfont               [LPVOID]
* @return [HEXFONT]返回字体句柄
*/
HEXFONT _font_createfromlogfont(LOGFONTW* lpLogfont);

/*
* @fnType ExDirectUI
* @brief _font_destroy 字体销毁
* @param 1  hFont                   [HEXFONT*]
* @return [BOOL]返回是否成功
*/
BOOL _font_destroy(HEXFONT hFont);

/*
* @fnType ExDirectUI
* @brief _font_getcontext 取描述表
* @param 1  hFont                   [HEXFONT]
* @return [LPVOID]
*/
LPVOID _font_getcontext(HEXFONT hFont);

/*
* @fnType ExDirectUI
* @brief _font_getlogfont 获取逻辑字体
* @param 1  hFont                   [HEXFONT]
* @param 2  lpLogFont               [LOGFONTW*]		返回逻辑字体
* @return [BOOL]返回是否成功
*/
BOOL _font_getlogfont(HEXFONT hFont, LOGFONTW* lpLogFont);

/*
* @fnType ExDirectUI
* @brief _img_changecolor 图像改颜色
* @param 1  hImg                    [HEXIMAGE]            图像句柄
* @param 2  argb                    [EXARGB]              新颜色
* @return [BOOL]返回是否成功
*/
BOOL _img_changecolor(HEXIMAGE hImg, EXARGB argb);

/*
* @fnType ExDirectUI
* @brief _img_clip 裁剪图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  left                    [INT]
* @param 3  top                     [INT]
* @param 4  width                   [INT]
* @param 5  height                  [INT]
* @param 6  phImg                   [HEXIMAGE*]           返回新图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_clip(HEXIMAGE hImg, INT left, INT top, INT width, INT height, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_copy 复制图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  phImg                  [HEXIMAGE*]           返回新图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_copy(HEXIMAGE hImg, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_copyrect 复制部分图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  x                       [INT]
* @param 3  y                       [INT]
* @param 4  width                   [INT]
* @param 5  height                  [INT]
* @param 6  phImg                  [HEXIMAGE*]           返回新图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_copyrect(HEXIMAGE hImg, INT x, INT y, INT width, INT height, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_create 创建图像
* @param 1  width                   [INT]                 图像宽度
* @param 2  height                  [INT]                 图像高度
* @param 3  phImg                   [HEXIMAGE*]           返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_create(INT width, INT height, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfromfile 图像创建自文件
* @param 1  wzFilename              [LPCWSTR]			 文件路径
* @param 2  phImg                   [HEXIMAGE*]          返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfromfile(LPCWSTR lpwzFilename, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfromhbitmap 创建自位图句柄
* @param 1  hBitmap                 [LPVOID]
* @param 2  hPalette                [LPVOID]
* @param 3  fPreAlpha               [BOOL]                是否预乘透明通道
* @param 4  phImg                    [HEXIMAGE*]           返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfromhbitmap(LPVOID hBitmap, LPVOID hPalette, BOOL fPreAlpha, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfromhicon 图像创建自图标句柄
* @param 1  hIcon                   [HICON]
* @param 2  phImg                    [HEXIMAGE*]           返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfromhicon(HICON hIcon, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfrommemory 图像创建自内存。
* @param 1  lpData                  [LPVOID]              图像指针
* @param 2  dwLen                   [size_t]              图像长度
* @param 3  phImg                   [HEXIMAGE*]           返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfrommemory(LPVOID lpData, size_t dwLen, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfromres 图像创建自资源包
* @param 1  hRes                    [HEXRES]
* @param 2  atomPath                [EXATOM]			路径
* @param 3  phImg                    [HEXIMAGE*]         返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfromres(HEXRES hRes, EXATOM atomPath, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_createfromstream 创建自字节流
*
* @param 1  lpStream                [LPVOID]
* @param 2  phImg                    [HEXIMAGE*]           返回图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_createfromstream(LPVOID lpStream, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_destroy 销毁图像
*
* @param 1  hImg                    [HEXIMAGE]
* @return [BOOL]返回是否成功
*/
BOOL _img_destroy(HEXIMAGE hImg);

/*
* @fnType ExDirectUI
* @brief _img_getframecount 取图像帧数
* @param 1  hImg                    [HEXIMAGE]
* @param 2  nFrameCount             [INT*]                返回帧数.
* @return [BOOL]返回是否成功
*/
BOOL _img_getframecount(HEXIMAGE hImage, INT* nFrameCount);

/*
* @fnType ExDirectUI
* @brief _img_getframedelay 取图像帧延时
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  lpDelayAry              [INT*]                 指针指向图像帧延时数组。
* @param 3  nFrames                 [INT]                 返回总帧数
* @return [BOOL]返回是否成功
*/
BOOL _img_getframedelay(HEXIMAGE hImg, INT* lpDelayAry, INT nFrames);

/*
* @fnType ExDirectUI
* @brief _img_getpixel 获取点像素
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  x                       [INT]
* @param 3  y                       [INT]
* @param 4  retPixel                [EXARGB*]             返回ARGB颜色
* @return [BOOL]返回是否成功
*/
BOOL _img_getpixel(HEXIMAGE hImg, INT x, INT y, EXARGB* retPixel);

/*
* @fnType ExDirectUI
* @brief _img_getsize 获取图像尺寸
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  lpWidth                 [INT*]
* @param 3  lpHeight                [INT*]
* @return [BOOL]返回是否成功
*/
BOOL _img_getsize(HEXIMAGE hImg, INT* lpWidth, INT* lpHeight);

/*
* @fnType ExDirectUI
* @brief _img_height 取图像高度
*
* @param 1  hImg                    [HEXIMAGE]
* @return [INT]返回高度
*/
INT _img_height(HEXIMAGE hImg);

/*
* @fnType ExDirectUI
* @brief _img_lock 锁定图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  lpRectL                 [RECT*]               锁定矩形指针，如果为0，则锁定整张图像。
* @param 3  flags                   [DWORD]               READ:1 / WRITE:2 / READ&WRITE:3
* @param 4  PixelFormat             [INT]                 参考:https://msdn.microsoft.com/en-us/library/ms534412(v=vs.85).aspx
* @param 5  lpLockedBitmapData      [EX_BITMAPDATA*]      BITMAPDATA
* @return [BOOL]返回是否成功
*/
BOOL _img_lock(HEXIMAGE hImg, RECT* lpRectL, DWORD flags, INT PixelFormat, EX_BITMAPDATA* lpLockedBitmapData);

/*
* @fnType ExDirectUI
* @brief _img_rotateflip 旋转翻转图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  rfType                  [INT]                 参考:https://msdn.microsoft.com/en-us/library/windows/desktop/ms534171(v=vs.85).aspx
* @param 3  phImg                   [HEXIMAGE*]           返回新图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_rotateflip(HEXIMAGE hImg, INT rfType, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_savetofile 图像保存到文件
* @param 1  hImg                    [HEXIMAGE]            图像句柄
* @param 2  wzFileName              [LPCWSTR]             保存的文件名
* @return [BOOL]返回是否成功
*/
BOOL _img_savetofile(HEXIMAGE hImg, LPCWSTR wzFileName);

/*
* @fnType ExDirectUI
* @brief _img_savetomemory 图像保存到内存缓冲区
* @param 1  hImg                    [HEXIMAGE]            图像句柄
* @param 2  lpBuffer                [LPVOID]              填充保存的缓冲区(0则只是返回长度)
* @return [size_t]返回长度
*/
size_t _img_savetomemory(HEXIMAGE hImg, LPVOID lpBuffer);

/*
* @fnType ExDirectUI
* @brief _img_clip 缩放图像
* @param 1  hImg                    [HEXIMAGE]
* @param 2  width                   [INT]
* @param 3  height                  [INT]
* @param 4  phImg                   [HEXIMAGE*]           返回新图像指针
* @return [BOOL]返回是否成功
*/
BOOL _img_scale(HEXIMAGE hImg, INT width, INT height, HEXIMAGE* phImg);

/*
* @fnType ExDirectUI
* @brief _img_selectactiveframe 设置当前活动帧
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  nIndex                  [INT]
* @return [BOOL]返回是否成功
*/
BOOL _img_selectactiveframe(HEXIMAGE hImg, INT nIndex);

/*
* @fnType ExDirectUI
* @brief _img_setpixel 设置点像素
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  x                       [INT]
* @param 3  y                       [INT]
* @param 4  color                   [EXARGB]              argb
* @return [BOOL]返回是否成功
*/
BOOL _img_setpixel(HEXIMAGE hImg, INT x, INT y, EXARGB color);

/*
* @fnType ExDirectUI
* @brief _img_unlock 解锁图像
*
* @param 1  hImg                    [HEXIMAGE]
* @param 2  lpLockedBitmapData      [EX_BITMAPDATA*]
* @return [BOOL]返回是否成功
*/
BOOL _img_unlock(HEXIMAGE hImg, EX_BITMAPDATA* lpLockedBitmapData);

/*
* @fnType ExDirectUI
* @brief _img_width 取图像宽度
*
* @param 1  hImg                    [HEXIMAGE]
* @return [INT]返回宽度
*/
INT _img_width(HEXIMAGE hImg);

/*
* @fnType ExDirectUI
* @brief _imglist_add 添加图片从数据指针
* @param 1  hImageList              [INT]                 图片组句柄
* @param 2  lpImage                 [LPVOID]              图像数据指针
* @param 3  cbImage                 [size_t]              图像数据长度
* @param 4  nIndexInsert            [size_t]              插入位置(0为最后)
* @return [size_t]返回索引
*/
size_t _imglist_add(HEXIMAGELIST hImageList, LPVOID pImg, size_t dwBytes, size_t nIndex);

/*
* @fnType ExDirectUI
* @brief _imglist_addimage 添加图片从图片句柄
* @param 1  hImageList              [HEXIMAGELIST]                 图片组句柄
* @param 2  hImage                  [HEXIMAGE]            图像对象句柄
* @param 3  nIndexInsert            [size_t]                 插入位置(0为最后)
* @return [size_t]返回索引
*/
size_t _imglist_addimage(HEXIMAGELIST hImageList, HEXIMAGE hImg, size_t nIndex);

/*
* @fnType ExDirectUI
* @brief _imglist_count 获取图片组图片数量
* @param 1  hImageList              [HEXIMAGELIST]           图片组句柄
* @return [INT]返回图片组图片数量
*/
INT _imglist_count(HEXIMAGELIST hImageList);

/*
* @fnType ExDirectUI
* @brief _imglist_create 创建图片组
* @param 1  nWidth                  [INT]                 宽度
* @param 2  nHeight                 [INT]                 高度
* @return [HEXIMAGELIST]返回图片组句柄
*/
HEXIMAGELIST _imglist_create(INT width, INT height);

/*
* @fnType ExDirectUI
* @brief _imglist_del 删除图片
* @param 1  hImageList              [HEXIMAGELIST]           图片组句柄
* @param 2  nIndex                  [size_t]                 图片索引
* @return [BOOL]返回是否成功
*/
BOOL _imglist_del(HEXIMAGELIST hImageList, size_t nIndex);

/*
* @fnType ExDirectUI
* @brief _imglist_destroy 销毁图片组
* @param 1  hImageList              [HEXIMAGELIST]                 图片组句柄
* @return [BOOL]返回是否成功
*/
BOOL _imglist_destroy(HEXIMAGELIST hImageList);

/*
* @fnType ExDirectUI
* @brief _imglist_draw 绘制图片
* @param 1  hImageList              [HEXIMAGELIST]        图片组句柄
* @param 2  nIndex                  [size_t]              图片索引
* @param 3  hCanvas                 [HEXCANVAS]           画布句柄
* @param 4  nLeft                   [INT]                 左边
* @param 5  nTop                    [INT]                 顶边
* @param 6  nRight                  [INT]                 右边
* @param 7  nBottom                 [INT]                 底边
* @param 8  nAlpha                  [DWORD]               透明度0-255
* @return [BOOL]返回是否成功
*/
BOOL _imglist_draw(HEXIMAGELIST hImageList, size_t nIndex, HEXCANVAS hCanvas, INT nLeft, INT nTop, INT nRight, INT nBottom, DWORD nAlpha);

/*
* @fnType ExDirectUI
* @brief _imglist_get 获取图片句柄
* @param 1  hImageList              [HEXIMAGELIST]           图片组句柄
* @param 2  nIndex                  [size_t]                 图片索引
* @return [HEXIMAGE]返回图片句柄
*/
HEXIMAGE _imglist_get(HEXIMAGELIST hImageList, size_t nIndex);

/*
* @fnType ExDirectUI
* @brief _imglist_set 设置图片
* @param 1  hImageList              [HEXIMAGELIST]        图片组句柄
* @param 2  nIndex                  [size_t]              图片索引
* @param 3  lpImage                 [LPVOID]              图片数据指针
* @param 4  cbImage                 [size_t]              图片数据长度
* @return [BOOL]返回是否成功
*/
BOOL _imglist_set(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg, size_t dwBytes);

/*
* @fnType ExDirectUI
* @brief _imglist_setimage 设置图片
* @param 1  hImageList              [HEXIMAGELIST]        图片组句柄
* @param 2  nIndex                  [size_t]              图片索引
* @param 3  hImage                  [HEXIMAGE]            图像对象句柄
* @return [BOOL]返回是否成功
*/
BOOL _imglist_setimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg);

/*
* @fnType ExDirectUI
* @brief _imglist_size 获取图片组图片尺寸
* @param 1  hImageList              [HEXIMAGELIST]         图片组句柄
* @param 2  pWidth                  [INT*]                 返回图片宽度
* @param 3  pHeight                 [INT*]                 返回图片高度
* @return [BOOL]返回是否成功
*/
BOOL _imglist_size(HEXIMAGELIST hImageList, INT* pWidth, INT* pHeight);

/*
* @fnType ExDirectUI
* @brief _layout_absolute_lock 绝对布局按当前位置锁定
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObjChild               [HEXOBJ]
* @param 3  tLeft                   [INT]                 #ELCP_ABSOLUTE_XXX_TYPE 下同
* @param 4  tTop                    [INT]
* @param 5  tRight                  [INT]
* @param 6  tBottom                 [INT]
* @param 7  tWidth                  [INT]
* @param 8  tHeight                 [INT]
* @return [BOOL]返回是否成功
*/
BOOL _layout_absolute_lock(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT tLeft, INT tTop, INT tRight, INT tBottom, INT tWidth, INT tHeight);

/*
* @fnType ExDirectUI
* @brief _layout_absolute_setedge 绝对布局置边界信息
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObjChild               [HEXOBJ]
* @param 3  dwEdge                  [INT]               #ELCP_ABSOLUTE_XXX
* @param 4  dwType                  [INT]
* @param 5  nValue                  [size_t]
* @return [BOOL]返回是否成功
*/
BOOL _layout_absolute_setedge(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT dwEdge, INT dwType, size_t nValue);

/*
* @fnType ExDirectUI
* @brief _layout_addchild 布局添加组件
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL _layout_addchild(HEXLAYOUT hLayout, HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief _layout_addchildren 布局加入所有组件,已被加入的不会重复添加(系统按钮不加入)
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  fDesc                   [BOOL]                是否倒序
* @param 3  dwObjClassATOM          [EXATOM]              0或空为所有
* @param 4  nCount                  [INT*]                加入的个数
* @return [BOOL]返回是否成功
*/
BOOL _layout_addchildren(HEXLAYOUT hLayout, BOOL fDesc, EXATOM dwObjClassATOM, INT* nCount);

/*
* @fnType ExDirectUI
* @brief _layout_create	创建布局
* @param 1  nType                   [INT]                 #ELT_ 布局类型
* @param 2  hObjBind                [EXHANDLE]            所绑定的HOBJ或HEXDUI
* @return [HEXLAYOUT]返回布局句柄
*/
HEXLAYOUT _layout_create(INT nType, EXHANDLE hObjBind);

/*
* @fnType ExDirectUI
* @brief _layout_deletechild 布局删除组件
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL _layout_deletechild(HEXLAYOUT hLayout, HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief _layout_deletechildren 布局删除所有组件
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  dwObjClassATOM          [EXATOM]              0或空为所有
* @return [BOOL]返回是否成功
*/
BOOL _layout_deletechildren(HEXLAYOUT hLayout, EXATOM dwObjClassATOM);

/*
* @fnType ExDirectUI
* @brief _layout_destroy 销毁布局
* @param 1  hLayout                 [HEXLAYOUT]
* @return [BOOL]返回是否成功
*/
BOOL _layout_destroy(HEXLAYOUT hLayout);

/*
* @fnType ExDirectUI
* @brief _layout_enableupdate 布局置是否允许更新
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  fUpdateable             [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL _layout_enableupdate(HEXLAYOUT hLayout, BOOL fUpdateable);

/*
* @fnType ExDirectUI
* @brief _layout_getchildprop 布局取子属性
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObj                    [HEXOBJ]
* @param 3  dwPropID                [INT]
* @param 4  pvValue                 [size_t*]
* @return [BOOL]返回是否成功
*/
BOOL _layout_getchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID, size_t* pvValue);

/*
* @fnType ExDirectUI
* @brief _layout_getchildproplist 布局取子属性列表
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObj                    [HEXOBJ]
* @param 3  lpProps                 [LPVOID*]                不释放
* @return [BOOL]返回是否成功
*/
BOOL _layout_getchildproplist(HEXLAYOUT hLayout, HEXOBJ hObj, LPVOID* lpProps);

/*
* @fnType ExDirectUI
* @brief _layout_getprop 布局取属性
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  dwPropID                [INT]
* @return [size_t]返回属性
*/
size_t _layout_getprop(HEXLAYOUT hLayout, INT dwPropID);

/*
* @fnType ExDirectUI
* @brief _layout_getproplist 布局取属性列表
* @param 1  hLayout                 [HEXLAYOUT]
* @return [LPVOID]
*/
LPVOID _layout_getproplist(HEXLAYOUT hLayout);

/*
* @fnType ExDirectUI
* @brief _layout_gettype 布局取类型
* @param 1  hLayout                 [HEXLAYOUT]
* @return [INT]返回布局类型
*/
INT _layout_gettype(HEXLAYOUT hLayout);

/*
* @fnType ExDirectUI
* @brief _layout_notify 布局分发通知
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  nEvent                  [INT]
* @param 3  wParam                  [WPARAM]
* @param 4  lParam                  [LPARAM]
* @return [LRESULT]返回结果
*/
LRESULT _layout_notify(HEXLAYOUT hLayout, INT nEvent, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief _layout_setchildprop 布局置子属性
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  hObj                    [HEXOBJ]
* @param 3  dwPropID                [INT]
* @param 4  pvValue                 [size_t]
* @return [BOOL]返回是否成功
*/
BOOL _layout_setchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID, size_t pvValue);

/*
* @fnType ExDirectUI
* @brief _layout_setprop 布局置属性
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  dwPropID                [INT]
* @param 3  pvValue                 [size_t]
* @return [BOOL]返回是否成功
*/
BOOL _layout_setprop(HEXLAYOUT hLayout, INT dwPropID, size_t pvValue);

/*
* @fnType ExDirectUI
* @brief _layout_table_setinfo 布局置表格信息
* @details 无
*
* @param 1  hLayout                 [HEXLAYOUT]
* @param 2  aRowHeight              [INT*]
* @param 3  cRows                   [INT]
* @param 4  aCellWidth              [INT*]
* @param 5  cCells                  [INT]
* @return [BOOL]返回是否成功
*/
BOOL _layout_table_setinfo(HEXLAYOUT hLayout, INT* aRowHeight, INT cRows, INT* aCellWidth, INT cCells);

/*
* @fnType ExDirectUI
* @brief _layout_update 布局更新
* @param 1  hLayout                 [HEXLAYOUT]
* @return [BOOL]返回是否成功
*/
BOOL _layout_update(HEXLAYOUT hLayout);

/*
* @fnType ExDirectUI
* @brief _matrix_create 创建矩阵
* @return [HEXMATRIX]返回矩阵句柄
*/
HEXMATRIX _matrix_create();

/*
* @fnType ExDirectUI
* @brief _matrix_destroy 销毁矩阵
* @param 1  pMatrix                 [HEXMATRIX]
* @return [BOOL]返回是否成功
*/
BOOL _matrix_destroy(HEXMATRIX pMatrix);

/*
* @fnType ExDirectUI
* @brief _matrix_reset 重置矩阵
* @param 1  pMatrix                 [HEXMATRIX]
* @return [BOOL]返回是否成功
*/
BOOL _matrix_reset(HEXMATRIX pMatrix);

/*
* @fnType ExDirectUI
* @brief _matrix_rotate 矩阵旋转
* @param 1  pMatrix                 [HEXMATRIX]
* @param 2  fAngle                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _matrix_rotate(HEXMATRIX pMatrix, FLOAT fAngle);

/*
* @fnType ExDirectUI
* @brief _matrix_scale 矩阵缩放
* @param 1  pMatrix                 [HEXMATRIX]
* @param 2  scaleX                  [FLOAT]
* @param 3  scaleY                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _matrix_scale(HEXMATRIX pMatrix, FLOAT scaleX, FLOAT scaleY);

/*
* @fnType ExDirectUI
* @brief _matrix_translate 矩阵转置
* @param 1  pMatrix                 [HEXMATRIX]
* @param 2  offsetX                 [FLOAT]
* @param 3  offsetY                 [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _matrix_translate(HEXMATRIX pMatrix, FLOAT offsetX, FLOAT offsetY);

/*
* @fnType ExDirectUI
* @brief _path_addarc 添加弧
* @param 1  hPath                   [HEXPATH]
* @param 2  x1                      [FLOAT]               起始坐标X
* @param 3  y1                      [FLOAT]               起始坐标Y
* @param 4  x2                      [FLOAT]               终点坐标X
* @param 5  y2                      [FLOAT]               终点坐标Y
* @param 6  radiusX                 [FLOAT]               弧中心坐标X
* @param 7  radiusY                 [FLOAT]               弧中心坐标Y
* @param 8  fClockwise              [BOOL]                是否顺时针
* @return [BOOL]返回是否成功
*/
BOOL _path_addarc(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT radiusX, FLOAT radiusY, BOOL fClockwise);

/*
* @fnType ExDirectUI
* @brief _path_addarc2 添加弧 v2
* @param 1  hPath                   [HEXPATH]
* @param 2  x                       [FLOAT]
* @param 3  y                       [FLOAT]
* @param 4  width                   [FLOAT]               宽度
* @param 5  height                  [FLOAT]               高度
* @param 6  startAngle              [FLOAT]               开始角度
* @param 7  sweepAngle              [FLOAT]               扫描角度
* @return [BOOL]返回是否成功
*/
BOOL _path_addarc2(HEXPATH hPath, FLOAT x, FLOAT y, FLOAT width, FLOAT height, FLOAT startAngle, FLOAT sweepAngle);

/*
* @fnType ExDirectUI
* @brief _path_addarc3 添加弧V3
* @param 1  hPath                   [HEXPATH]
* @param 2  x                       [FLOAT]               终点坐标X
* @param 3  y                       [FLOAT]               终点坐标Y
* @param 4  radiusX                 [FLOAT]               弧中心坐标X
* @param 5  radiusY                 [FLOAT]               弧中心坐标Y
* @param 6  startAngle              [FLOAT]               起始弧度
* @param 7  sweepAngle              [FLOAT]               扫描弧度
* @param 8  fClockwise              [BOOL]                是否顺时针
* @param 9  barcSize                [BOOL]                是否大于180°
* @return [BOOL]返回是否成功
*/
BOOL _path_addarc3(HEXPATH hPath, FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY, FLOAT startAngle, FLOAT sweepAngle, BOOL fClockwise, BOOL barcSize);

/*
* @fnType ExDirectUI
* @brief _path_addbezier 路径添加贝塞尔曲线
* @param 1  hPath                   [HEXPATH]
* @param 2  x1                      [FLOAT]
* @param 3  y1                      [FLOAT]
* @param 4  x2                      [FLOAT]
* @param 5  y2                      [FLOAT]
* @param 6  x3                      [FLOAT]
* @param 7  y3                      [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_addbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2, FLOAT x3, FLOAT y3);

/*
* @fnType ExDirectUI
* @brief _path_addline 路径添加直线
* @param 1  hPath                   [HEXPATH]
* @param 2  x1                      [FLOAT]
* @param 3  y1                      [FLOAT]
* @param 4  x2                      [FLOAT]
* @param 5  y2                      [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_addline(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);

/*
* @fnType ExDirectUI
* @brief _path_addquadraticbezier 路径添加二次方贝塞尔曲线
* @param 1  hPath                   [HEXPATH]
* @param 2  x1                      [FLOAT]
* @param 3  y1                      [FLOAT]
* @param 4  x2                      [FLOAT]
* @param 5  y2                      [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_addquadraticbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);

/*
* @fnType ExDirectUI
* @brief _path_addrect 路径添加矩形
* @param 1  hPath                   [HEXPATH]
* @param 2  left                    [FLOAT]
* @param 3  top                     [FLOAT]
* @param 4  right                   [FLOAT]
* @param 5  bottom                  [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_addrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/*
* @fnType ExDirectUI
* @brief _path_addroundedrect 添加圆角矩形
* @param 1  hPath                   [HEXPATH]
* @param 2  left                    [FLOAT]
* @param 3  top                     [FLOAT]
* @param 4  right                   [FLOAT]
* @param 5  bottom                  [FLOAT]
* @param 6  radiusTopLeft           [FLOAT]
* @param 7  radiusTopRight          [FLOAT]
* @param 8  radiusBottomLeft        [FLOAT]
* @param 9  radiusBottomRight       [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_addroundedrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusTopLeft, FLOAT radiusTopRight, FLOAT radiusBottomLeft, FLOAT radiusBottomRight);

/*
* @fnType ExDirectUI
* @brief _path_beginfigure 路径开始新图形
* @param 1  hPath                   [HEXPATH]
* @return [BOOL]返回是否成功
*/
BOOL _path_beginfigure(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _path_beginfigure2  路径开始新图形2,可设置起始点
* @param 1  hPath                   [HEXPATH]
* @param 2  x                       [FLOAT]               起始坐标
* @param 3  y                       [FLOAT]               起始坐标
* @return [BOOL]返回是否成功
*/
BOOL _path_beginfigure2(HEXPATH hPath, FLOAT x, FLOAT y);

/*
* @fnType ExDirectUI
* @brief _path_beginfigure3  路径开始新图形3,可设置起始点和类型
* @param 1  hPath                   [HEXPATH]
* @param 2  x                       [FLOAT]               起始坐标
* @param 3  y                       [FLOAT]               起始坐标
* @param 4  figureBegin				[INT]				  常量PBF_
* @return [BOOL]返回是否成功
*/
BOOL _path_beginfigure3(HEXPATH hPath, FLOAT x, FLOAT y, INT figureBegin);

/*
* @fnType ExDirectUI
* @brief _path_close 关闭路径
* @param 1  hPath                   [HEXPATH]
* @return [BOOL]返回是否成功
*/
BOOL _path_close(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _path_create 创建路径
* @param 1  dwFlags                 [DWORD]               EPF_
* @param 2  hPath                   [HEXPATH*]
* @return [BOOL]返回是否成功
*/
BOOL _path_create(DWORD dwFlags, HEXPATH* hPath);

/*
* @fnType ExDirectUI
* @brief _path_destroy 销毁路径
* @param 1  hPath                   [HEXPATH]
* @return [BOOL]返回是否成功
*/
BOOL _path_destroy(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _path_endfigure 路径结束当前图形
* @param 1  hPath                   [HEXPATH]
* @param 2  fCloseFigure            [BOOL]                是否需要闭合图形
* @return [BOOL]返回是否成功
*/
BOOL _path_endfigure(HEXPATH hPath, BOOL fCloseFigure);

/*
* @fnType ExDirectUI
* @brief _path_getbounds 取路径边界矩形
* @param 1  hPath                   [HEXPATH]
* @param 2  lpBounds                [RECTF*]              RECTF
* @return [BOOL]返回是否成功
*/
BOOL _path_getbounds(HEXPATH hPath, RECTF* lpBounds);

/*
* @fnType ExDirectUI
* @brief _path_hittest 测试坐标是否在可见路径内
* @param 1  hPath                   [HEXPATH]
* @param 2  x                       [FLOAT]
* @param 3  y                       [FLOAT]
* @return [BOOL]返回是否成功
*/
BOOL _path_hittest(HEXPATH hPath, FLOAT x, FLOAT y);

/*
* @fnType ExDirectUI
* @brief _path_open 打开路径
* @param 1  hPath                   [HEXPATH]
* @return [BOOL]返回是否成功
*/
BOOL _path_open(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _path_reset 重置路径
* @param 1  hPath                   [HEXPATH]
* @return [BOOL]返回是否成功
*/
BOOL _path_reset(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _rgn_combine 区域合并
* @param 1  hRgnSrc                 [HEXRGN]
* @param 2  hRgnDst                 [HEXRGN]
* @param 3  nCombineMode            [INT]                 #RGN_COMBINE_
* @param 4  dstOffsetX              [INT]
* @param 5  dstOffsetY              [INT]
* @return [HEXRGN]返回合并区域句柄
*/
HEXRGN _rgn_combine(HEXRGN hRgnSrc, HEXRGN hRgnDst, INT nCombineMode, INT dstOffsetX, INT dstOffsetY);

/*
* @fnType ExDirectUI
* @brief _rgn_createfrompath 区域创建自路径
* @param 1  hPath                    [HEXPATH]
* @return [HEXRGN]返回区域句柄
*/
HEXRGN _rgn_createfrompath(HEXPATH hPath);

/*
* @fnType ExDirectUI
* @brief _rgn_createfromrect 区域创建自矩形
* @param 1  left                    [FLOAT]
* @param 2  top                     [FLOAT]
* @param 3  right                   [FLOAT]
* @param 4  bottom                  [FLOAT]
* @return [HEXRGN]返回区域句柄
*/
HEXRGN _rgn_createfromrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/*
* @fnType ExDirectUI
* @brief _rgn_createfromroundrect 区域创建自圆角矩形
* @param 1  left                    [FLOAT]
* @param 2  top                     [FLOAT]
* @param 3  right                   [FLOAT]
* @param 4  bottom                  [FLOAT]
* @param 5  radiusX                 [FLOAT]
* @param 6  radiusY                 [FLOAT]
* @return [HEXRGN]返回区域句柄
*/
HEXRGN _rgn_createfromroundrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, FLOAT radiusX, FLOAT radiusY);

/*
* @fnType ExDirectUI
* @brief _rgn_destroy 区域销毁
* @param 1  hRgn                    [HEXRGN]
* @return [BOOL]返回是否成功
*/
BOOL _rgn_destroy(HEXRGN hRgn);

/*
* @fnType ExDirectUI
* @brief _rgn_hittest 区域命中测试
* @param 1  hRgn                    [HEXRGN]
* @param 2  x                       [FLOAT]
* @param 3  y                       [FLOAT]
* @return [BOOL]返回是否命中区域
*/
BOOL _rgn_hittest(HEXRGN hRgn, FLOAT x, FLOAT y);

/*
* @fnType ExDirectUI
* @brief Ex_AllocBuffer 申请内存
* @param 1  dwSize                   [size_t]
* @return [LPVOID]返回内存地址
*/
LPVOID Ex_AllocBuffer(size_t dwSize);

/*
* @fnType ExDirectUI
* @brief Ex_Atom 获取字符串唯一原子号
* @param 1  lptstring               [LPCWSTR]
* @return [EXATOM]返回原子号
*/
EXATOM Ex_Atom(LPCWSTR lptstring);

/*
* @fnType ExDirectUI
* @brief Ex_CefBrowserInitialize 初始化CEF浏览框路径,创建前使用
* @param 1  libPath               [LPCWSTR] 库文件夹路径,绝对路径,不为0时会写入环境变量PATH
* @param 2  singleProcess         [BOOL] 是否单进程模式
* @param 3  cachePath             [LPCWSTR] 临时文件保存路径,绝对路径,0为内存模式
* @param 4  userAgent             [LPCWSTR] 用户设备
* @param 5  debuggingPort         [INT] 调试端口
* @param 6  BeforeCommandLine     [CefPROC] 可为0,初始化传入的命令行参数void(CALLBACK* CefPROC)(void* command_line);
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjCefBrowserInitialize(LPCWSTR libPath, BOOL singleProcess, LPCWSTR cachePath, LPCWSTR userAgent, INT debuggingPort, CefPROC lpBeforeCommandLine);

/*
* @fnType ExDirectUI
* @brief Ex_DUIBindWindow 绑定窗口,
* @param 1  hWnd                    [HWND]                窗口句柄
* @param 2  hTheme                  [HEXTHEME]            主题句柄.(值可为0)
* @param 3  dwStyle                 [DWORD]               相关常量:#EWS_
* @return [HEXDUI]成功返回引擎句柄,失败返回0.
*/
HEXDUI Ex_DUIBindWindow(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle);

/*
* @fnType ExDirectUI
* @brief Ex_DUIBindWindowEx 绑定窗口
* @param 1  hWnd                    [HWND]                窗口句柄
* @param 2  hTheme                  [HEXTHEME]            主题句柄.(值可为0)
* @param 3  dwStyle                 [DWORD]               相关常量:#EWS_
* @param 4  lParam                  [LPARAM]              附加参数.(值可为0)
* @param 5  lpfnMsgProc             [WinMsgPROC]          (LRESULT)MsgProc(HWND hWnd,HEXDUI	hExDui,INT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult).
* @return [HEXDUI]成功返回引擎句柄,失败返回0
*/
HEXDUI Ex_DUIBindWindowEx(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle, LPARAM lParam, WinMsgPROC lpfnMsgProc);

/*
* @fnType ExDirectUI
* @brief Ex_DUIFromWindow 从窗口句柄获取引擎句柄
* @param 1  hWnd                    [HWND]
* @return [HEXDUI]返回引擎句柄
*/
HEXDUI Ex_DUIFromWindow(HWND hWnd);

/*
* @fnType ExDirectUI
* @brief Ex_DUIGetClientRect 获取客户区矩形
* @param 1  hExDui                  [HEXDUI]              引擎句柄
* @param 2  lpClientRect            [RECT*]               矩形指针
* @return [BOOL]返回是否成功
*/
BOOL Ex_DUIGetClientRect(HEXDUI hExDui, RECT* lpClientRect);

/*
* @fnType ExDirectUI
* @brief Ex_DUIGetLong 获取引擎数值
* @param 1  hExDui                  [HEXDUI]              引擎句柄
* @param 2  nIndex                  [INT]                 相关常量: EWL_
* @return [size_t]返回引擎数值
*/
size_t Ex_DUIGetLong(HEXDUI hExDui, INT nIndex);

/*
* @fnType ExDirectUI
* @brief Ex_DUIGetObjFromPoint 获取鼠标所在窗口组件句柄
* @param 1  handle                  [EXHANDLE]            0[坐标所在窗口]/窗口句柄/引擎句柄/组件句柄
* @param 2  x                       [INT]                 handle:0相对屏幕/其他相对窗口
* @param 3  y                       [INT]
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_DUIGetObjFromPoint(EXHANDLE handle, INT x, INT y);

/*
* @fnType ExDirectUI
* @brief Ex_DUISetLong 设置引擎数值
* @param 1  hExDui                  [HEXDUI]
* @param 2  nIndex                  [INT]                 #EWL_
* @param 3  dwNewlong               [size_t]
* @return [size_t]返回旧数值
*/
size_t Ex_DUISetLong(HEXDUI hExDui, INT nIndex, size_t dwNewlong);

/*
* @fnType ExDirectUI
* @brief Ex_DUIShowWindow 显示窗口
* @param 1  hExDui                  [HEXDUI]              引擎句柄
* @param 2  nCmdShow                [INT]                 相关常量:#SW_
* @param 3  dwTimer                 [INT]                 动画时间间隔.(ms)
* @param 4  dwFrames                [INT]                 动画总帧数.
* @param 5  dwFlags                 [INT]                 动画标记.
* @return [BOOL]返回是否成功
*/
BOOL Ex_DUIShowWindow(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags);

/*
* @fnType ExDirectUI
* @brief Ex_DUIShowWindowEx 显示窗口Ex
* @param 1  hExDui                  [HEXDUI]              引擎句柄
* @param 2  nCmdShow                [INT]                 相关常量:#SW_
* @param 3  dwTimer                 [INT]                 动画时间间隔.(ms)
* @param 4  dwFrames                [INT]                 动画总帧数.
* @param 5  dwFlags                 [INT]                 动画标记.#EXA_
* @param 6  uEasing                 [INT]                 缓动类型常量#ET_
* @param 7  wParam                  [WPARAM]              参数1
* @param 8  lParam                  [LPARAM]              参数2
* @return [BOOL]返回是否成功
*/
BOOL Ex_DUIShowWindowEx(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames, INT dwFlags, INT uEasing, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_DUITrayIconPopup 弹出托盘图标
* @param 1  hExDui                  [HEXDUI]              引擎句柄
* @param 2  lpwzInfo                [LPCWSTR]             弹出文本内容
* @param 3  lpwzInfoTitle           [LPCWSTR]             弹出标题文本
* @param 4  dwInfoFlags             [INT]                 相关常量 #NIIF_
* @return [BOOL]返回是否成功
*/
BOOL Ex_DUITrayIconPopup(HEXDUI hExDui, LPCWSTR lpwzInfo, LPCWSTR lpwzInfoTitle, INT dwInfoFlags);

/*
* @fnType ExDirectUI
* @brief Ex_DUITrayIconSet 设置托盘图标
* @param 1  hExdui                  [HEXDUI]                引擎句柄
* @param 2  hIcon                   [HICON]                 图标句柄
* @param 3  lpwzTips                [LPCWSTR]               提示文本指针
* @return [BOOL]返回是否成功
*/
BOOL Ex_DUITrayIconSet(HEXDUI hExDui, HICON hIcon, LPCWSTR lpwzTips);

/*
* @fnType ExDirectUI
* @brief Ex_FreeBuffer 释放内存
* @param 1  lpBuffer                  [LPVOID]			内存地址
* @return [BOOL]返回是否成功
*/
BOOL Ex_FreeBuffer(LPVOID lpBuffer);

/*
* @fnType ExDirectUI
* @brief Ex_GetLastError 获取最后错误代码.相关常量 :#ERROR_EX_
* @return [INT]返回最后错误
*/
INT Ex_GetLastError();

/*
* @fnType ExDirectUI
* @brief Ex_Init 初始化引擎
* @param 1  hInstance               [HINSTANCE]           (值可为0)
* @param 2  dwGlobalFlags           [DWORD]               相关常量:#EXGF_ .(值可为0)
* @param 3  hDefaultCursor          [HCURSOR]             默认鼠标指针.(值可为0)
* @param 4  lpszDefaultClassName    [LPCWSTR]             默认窗口类名.(值可为0)
* @param 5  lpDefaultTheme          [LPVOID]              默认主题包指针.
* @param 6  dwDefaultThemeLen       [size_t]              默认主题包长度.
* @param 7  lpDefaultI18N           [LPVOID]              默认语言包指针.(值可为0)
* @param 8  dwDefaultI18NLen        [size_t]              默认语言包长度.(值可为0)
* @return [BOOL]返回是否成功
*/
BOOL Ex_Init(HINSTANCE hInstance, DWORD dwGlobalFlags, HCURSOR hDefaultCursor, LPCWSTR lpszDefaultClassName,
	LPVOID lpDefaultTheme, size_t dwDefaultThemeLen, LPVOID lpDefaultI18N, size_t dwDefaultI18NLen);

/*
* @fnType ExDirectUI
* @brief Ex_IsDxRender 是否使用D2D渲染
*
* @return [BOOL]
*/
BOOL Ex_IsDxRender();

/*
* @fnType ExDirectUI
* @brief Ex_LoadBitMapFromMemory 加载位图对象自内存
* @param 1  lpData                  [LPVOID]              图像数据指针
* @param 2  dwLen                   [size_t]              图像数据长度
* @param 3  retBitMap                   [HBITMAP*]        返回位图句柄
* @return [BOOL]返回是否成功
*/
BOOL Ex_LoadBitMapFromMemory(LPVOID lpData, size_t dwLen, HBITMAP* retBitMap);

/*
* @fnType ExDirectUI
* @brief Ex_LoadImageFromMemory 加载图像对象自内存
* @param 1  lpData                  [LPVOID]              图像数据指针
* @param 2  dwLen                   [size_t]              图像数据长度
* @param 3  uType                   [INT]                 图像类型 IMAGE_
* @param 4  nIndex                  [INT]                 索引
* @return [LPVOID]返回位图句柄或图标句柄
*/
LPVOID Ex_LoadImageFromMemory(LPVOID lpData, size_t dwLen, INT uType, INT nIndex);

/*
* @fnType ExDirectUI
* @brief Ex_MessageBox
* @details 弹出信息框
*
* @param 1  handle                  [size_t]            组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认信息框弹出)
* @param 2  lpText                  [LPCWSTR]             消息框内容
* @param 3  lpCaption               [LPCWSTR]             消息框标题
* @param 4  uType                   [INT]                 相关常量 #MB_
* @param 5  dwFlags                 [INT]               相关常量 #EMBF_
* @return [INT]
*/
INT Ex_MessageBox(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, INT dwFlags);

/*
* @fnType ExDirectUI
* @brief Ex_MessageBoxEx 弹出信息框Ex
* @param 1  handle                  [size_t]            组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认信息框弹出)
* @param 2  lpText                  [LPCWSTR]             消息框内容
* @param 3  lpCaption               [LPCWSTR]             消息框标题
* @param 4  uType                   [INT]                 相关常量 #MB_
* @param 5  lpCheckBox              [LPCWSTR]             检查框标题(消息框左下角显示一个检查框组件).(如果该窗口未使用引擎渲染,则该值无效)
* @param 6  lpCheckBoxChecked       [BOOL*]               返回检查框选中状态.(如果该窗口未使用引擎渲染,则该值无效)
* @param 7  dwMilliseconds          [INT]                 消息框延迟关闭时间，单位：毫秒。如果该值不为0,且超时后(即用户未操作)，消息框自动关闭，返回32000。
* @param 8  dwFlags                 [INT]               相关常量 #EMBF_
* @param 9  lpfnMsgProc             [MsgPROC]           (BOOL)MsgProc(hWnd,hExDui,uMsg,wParam,lParam,lpResult).
* @return [INT]
*/
INT Ex_MessageBoxEx(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType, LPCWSTR lpCheckBox, BOOL* lpCheckBoxChecked, INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc);

/*
* @fnType ExDirectUI
* @brief Ex_ObjBeginPaint 组件开始绘制
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpPS                    [EX_PAINTSTRUCT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjBeginPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS);

/*
* @fnType ExDirectUI
* @brief Ex_ObjCallProc 调用过程
* @param 1  lpPrevObjProc           [ClsPROC]
* @param 2  hWnd                    [HWND]
* @param 3  hObj                    [HEXOBJ]
* @param 4  uMsg                    [INT]
* @param 5  wParam                  [WPARAM]
* @param 6  lParam                  [LPARAM]
* @return [LRESULT]返回结果
*/
LRESULT Ex_ObjCallProc(ClsPROC lpPrevObjProc, HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjCheckDropFormat 查询拖曳信息格式
* @param 1  hObj					[HEXOBJ]
* @param 2  pDataObject             [LPVOID]
* @param 3  dwFormat                [DWORD]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjCheckDropFormat(HEXOBJ hObj, LPVOID pDataObject, DWORD dwFormat);

/*
* @fnType ExDirectUI
* @brief Ex_ObjClientToScreen 客户区坐标到屏幕坐标
* @param 1  hObj                    [HEXOBJ]
* @param 2  x                       [INT*]
* @param 3  y                       [INT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjClientToScreen(HEXOBJ hObj, INT* x, INT* y);

/*
* @fnType ExDirectUI
* @brief Ex_ObjClientToWindow 客户区坐标到窗口坐标
* @param 1  hObj                    [HEXOBJ]
* @param 2  x                       [INT*]
* @param 3  y                       [INT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjClientToWindow(HEXOBJ hObj, INT* x, INT* y);

/*
* @fnType ExDirectUI
* @brief Ex_ObjCreate 创建组件.
* @param 1  lptszClassName          [LPCWSTR]             组件类名
* @param 2  lptszObjTitle           [LPCWSTR]             组件标题
* @param 3  dwStyle                 [INT]                 组件风格 相关常量 EOS_
* @param 4  x                       [INT]                 左边
* @param 5  y                       [INT]                 顶边
* @param 6  width                   [INT]                 宽度
* @param 7  height                  [INT]                 高度
* @param 8  hParent                 [EXHANDLE]            父组件句柄
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjCreate(LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent);

/*
* @fnType ExDirectUI
* @brief Ex_ObjCreateEx 创建组件Ex
* @param 1  dwStyleEx               [DWORD]               组件扩展风格 相关常量 EOS_EX_
* @param 2  lptszClassName          [LPCWSTR]             组件类名
* @param 3  lptszObjTitle           [LPCWSTR]             组件标题
* @param 4  dwStyle                 [INT]                 组件风格 相关常量 EOS_
* @param 5  x                       [INT]                 左边
* @param 6  y                       [INT]                 顶边
* @param 7  width                   [INT]                 宽度
* @param 8  height                  [INT]                 高度
* @param 9  hParent                 [EXHANDLE]            父组件句柄
* @param 10 nID                     [INT]				  组件ID
* @param 11 dwTextFormat            [INT]                 相关常量 DT_
* @param 12 lParam                  [LPARAM]              附加参数
* @param 13 hTheme                  [HEXTHEME]            主题句柄.0为默认
* @param 14 lpfnMsgProc             [MsgPROC]             LRESULT CALLBACK MsgProc(HWND hWnd,HEXOBJ hObj,INT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult).
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjCreateEx(INT dwStyleEx, LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y, INT width, INT height, EXHANDLE hParent, INT nID, INT dwTextFormat, LPARAM lParam, HEXTHEME hTheme, MsgPROC lpfnMsgProc);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDefProc 组件默认过程
* @param 1  hWnd                    [HWND]
* @param 2  hObj                    [HEXOBJ]
* @param 3  uMsg                    [INT]
* @param 4  wParam                  [WPARAM]
* @param 5  lParam                  [LPARAM]
* @return [LRESULT]返回结果
*/
LRESULT Ex_ObjDefProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDestroy 销毁组件
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjDestroy(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDisableTranslateSpaceAndEnterToClick 设置控件是否禁止转换空格和回车为单击事件
* @param 1  hObj                    [HEXOBJ]
* @param 2  fDisable                [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjDisableTranslateSpaceAndEnterToClick(HEXOBJ hObj, BOOL fDisable);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDispatchMessage 分发消息
* @param 1  hObj                    [HEXOBJ]
* @param 2  uMsg                    [INT]
* @param 3  wParam                  [WPARAM]
* @param 4  lParam                  [LPARAM]
* @return [LRESULT]返回结果
*/
size_t Ex_ObjDispatchMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDispatchNotify 分发事件
* @param 1  hObj                    [HEXOBJ]
* @param 2  nCode                   [INT]
* @param 3  wParam                  [WPARAM]
* @param 4  lParam                  [LPARAM]
* @return [size_t]返回结果
*/
size_t Ex_ObjDispatchNotify(HEXOBJ hObj, INT nCode, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjDrawBackgroundProc 默认绘制背景过程
* @param 1  hObj                    [HEXOBJ]
* @param 2  hCanvas                 [HEXCANVAS]
* @param 3  lprcPaint               [RECT*]
* @return [BOOL]
*/
BOOL Ex_ObjDrawBackgroundProc(HEXOBJ hObj, HEXCANVAS hCanvas, RECT* lprcPaint);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnable 设置组件可用状态.
* @param 1  hObj                    [HEXOBJ]              组件句柄
* @param 2  fEnable                 [BOOL]                是否可用
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEnable(HEXOBJ hObj, BOOL fEnable);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnableEventBubble 设置该控件是否启用事件冒泡
* @param 1  hObj                    [HEXOBJ]
* @param 2  fEnable                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEnableEventBubble(HEXOBJ hObj, BOOL fEnable);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnableIME 是否允许启用输入法
* @param 1  hObj                    [HEXOBJ]
* @param 2  fEnable                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEnableIME(HEXOBJ hObj, BOOL fEnable);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnablePaintingMsg 设置控件是否启用绘画中消息
* @param 1  hObj                    [HEXOBJ]
* @param 2  fEnable                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEnablePaintingMsg(HEXOBJ hObj, BOOL bEnable);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEndPaint 组件结束绘制
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpPS                    [EX_PAINTSTRUCT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEndPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnumChild 枚举子组件.
* @param 1  hObjParent              [EXHANDLE]            父组件的句柄。从根部枚举则为引擎句柄。
* @param 2  lpEnumFunc              [LPVOID]                 callback(hObj,lParam).返回1继续枚举,返回0停止枚举.
* @param 3  lParam                  [LPARAM]              附带参数
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjEnumChild(EXHANDLE hObjParent, LPVOID lpEnumFunc, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEnumProps 枚举属性表
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpfnCbk                 [LPVOID]          回调LRESULT enum(HEXOBJ hObj,size_t dwKey,size_t dwValue,size_t param)
* @param 3  param                   [size_t]
* @return [INT]返回个数.
*/
INT Ex_ObjEnumProps(HEXOBJ hObj, EnumPropsPROC lpfnCbk, size_t param);

/*
* @fnType ExDirectUI
* @brief Ex_ObjFind 查找组件.
* @param 1  hObjParent              [EXHANDLE]            父组件句柄。从根部查找则为引擎句柄。
* @param 2  hObjChildAfter          [HEXOBJ]              由此组件开始查找（不包含该组件）。如果为0，则从第一个组件开始查找。
* @param 3  lpClassName             [LPCWSTR]             欲查找的组件类名指针/Ex_ATOM()
* @param 4  lpTitle                 [LPCWSTR]             欲查找的组件标题
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjFind(EXHANDLE hObjParent, HEXOBJ hObjChildAfter, LPCWSTR lpClassName, LPCWSTR lpTitle);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetBackgroundImage 获取组件背景信息
* @param 1  handle                  [EXHANDLE]
* @param 2  lpBackgroundImage       [EX_BACKGROUNDIMAGEINFO]            相关结构 EX_BACKGROUNDIMAGEINFO
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetBackgroundImage(EXHANDLE handle, EX_BACKGROUNDIMAGEINFO* lpBackgroundImage);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetClassInfo 获取组件类信息
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpClassInfo             [EX_CLASSINFO*]                 相关结构 EX_CLASSINFO
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetClassInfo(HEXOBJ hObj, EX_CLASSINFO* lpClassInfo);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetClassInfoEx 通过类名/类ATOM获取类信息
* @param 1  wzClassName             [INT]                 wzClassName/AtomClassName
* @param 2  lpClassInfo             [INT]                 相关结构 EX_CLASSINFO
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetClassInfoEx(LPCWSTR lptszClassName, EX_CLASSINFO* lpClassInfo);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetClientRect 获取组件客户区矩形
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpRect                  [RECT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetClientRect(HEXOBJ hObj, RECT* lpRect);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetColor 获取组件相关颜色
* @param 1  hObj                    [HEXOBJ]
* @param 2  nIndex                  [INT]                 相关常量 COLOR_EX_
* @return [EXARGB]返回颜色
*/
EXARGB Ex_ObjGetColor(HEXOBJ hObj, INT nIndex);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetDropString 查询拖曳文本内容
* @param 1  hObj					[HEXOBJ]
* @param 2  pDataObject             [LPVOID]
* @param 3  lpwzBuffer              [LPWSTR]	//接收内容的文本指针
* @param 4  cchMaxLength            [INT]		//接收内容的文本指针尺寸
* @return [INT]返回字符数
*/
INT Ex_ObjGetDropString(HEXOBJ hObj, LPVOID pDataObject, LPWSTR lpwzBuffer, INT cchMaxLength);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetFocus 获取焦点组件
* @param 1  hExDuiOrhObj            [EXHANDLE]
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetFont 获取字体。用户不应该销毁该字体对象
* @param 1  hObj                    [HEXOBJ]
* @return [HEXFONT]返回字体句柄
*/
HEXFONT Ex_ObjGetFont(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetFromID 获取组件句柄自ID
* @param 1  hExDuiOrhObj            [EXHANDLE]            如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索.
* @param 2  nID                     [INT]
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, INT nID);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetFromName 获取组件句柄自名称
* @param 1  hExDuiOrhObj            [EXHANDLE]            如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索.
* @param 2  lpwzName                [LPCWSTR]
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, LPCWSTR lpName);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetFromNodeID 获取组件句柄自节点ID
* @param 1  hExDuiOrhObj            [EXHANDLE]            如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索.
* @param 2  nNodeID                [INT]
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjGetFromNodeID(EXHANDLE hExDUIOrObj, INT nNodeID);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetLong 获取组件数值
* @param 1  hObj                    [HEXOBJ]
* @param 2  nIndex                  [INT]                 相关常量 #EOL_
* @return [LONG_PTR]返回组件数值
*/
LONG_PTR Ex_ObjGetLong(HEXOBJ hObj, INT nIndex);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetObj 返回与指定组件有特定关系（如Z序或所有者）的组件句柄。
* @param 1  hObj                    [HEXOBJ]
* @param 2  nCmd                    [INT]                 相关常量 #GW_
* @return [HEXOBJ]返回组件句柄
*/
HEXOBJ Ex_ObjGetObj(HEXOBJ hObj, INT nCmd);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetParent 获取父组件
* @param 1  hObj                    [HEXOBJ]
* @return [EXHANDLE]返回父组件句柄
*/
EXHANDLE Ex_ObjGetParent(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetParentEx 获取父组件和EXDUI句柄
* @param 1  hObj                    [HEXOBJ]
* @param 2  phExDUI                 [HEXDUI*]
* @return [EXHANDLE]返回父组件句柄
*/
EXHANDLE Ex_ObjGetParentEx(HEXOBJ hObj, HEXDUI* phExDUI);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetProp 获取属性
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwKey                   [size_t]
* @return [size_t]返回属性
*/
size_t Ex_ObjGetProp(HEXOBJ hObj, size_t dwKey);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetRect 获取组件矩形
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpRect                  [RECT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetRect(HEXOBJ hObj, RECT* lpRect);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetRectEx 取组件矩形
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpRect                  [RECT*]
* @param 3  nType                   [INT] 0组件相对位置矩形 1组件客户区矩形 2组件窗口矩形 3组件脏区域矩形 4组件文本矩形
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetRectEx(HEXOBJ hObj, RECT* lpRect, INT nType);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetText 获取组件文本
* @param 1  hObj                    [HEXOBJ]              组件句柄.
* @param 2  lpString                [LPCWSTR]             缓冲区指针.
* @param 3  nMaxCount               [size_t]              缓冲区长度.
* @return [size_t]返回已拷贝字符长度
*/
size_t Ex_ObjGetText(HEXOBJ hObj, LPCWSTR lpString, size_t nMaxCount);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetTextLength 获取组件文本长度
* @param 1  hObj                    [HEXOBJ]
* @return [size_t]返回文本长度
*/
size_t Ex_ObjGetTextLength(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetTextRect 获取组件文本矩形
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpRect                  [RECT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjGetTextRect(HEXOBJ hObj, RECT* lpRect);

/*
* @fnType ExDirectUI
* @brief Ex_ObjGetUIState 获取组件状态
* @param 1  hObj                    [HEXOBJ]
* @return [INT]返回组件状态
*/
INT Ex_ObjGetUIState(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjHandleEvent 挂接组件事件回调
* @param 1  hObj                    [HEXOBJ]
* @param 2  nEvent                  [INT]                 #NM_
* @param 3  pfnCallback             [EventHandlerPROC]   (LRESULT) Handler(HEXOBJ hObj,INT nID,INT nCode,WPARAM wParam,LPARAM lParam) 返回0继续分发事件,返回1则停止继续分发事件
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjHandleEvent(HEXOBJ hObj, INT nEvent, EventHandlerPROC pfnCallback);

/*
* @fnType ExDirectUI
* @brief Ex_ObjInitPropList 初始化属性列表,要注意每次初始化都会清空之前存储的内容,若存储的是指针需要自己先行释放
* @param 1  hObj                    [HEXOBJ]
* @param 2  nPropCount              [INT]                 -1为哈希表模式,0为不存储属性,正数为属性个数(每个4字节)
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjInitPropList(HEXOBJ hObj, INT nPropCount);

/*
* @fnType ExDirectUI
* @brief Ex_ObjInvalidateRect 设置重画区域
* @param 1  hObj                    [HEXOBJ]
* @param 2  lprcRedraw              [RECT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjInvalidateRect(HEXOBJ hObj, RECT* lprcRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjIsEnable 组件是否可用
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否可用
*/
BOOL Ex_ObjIsEnable(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjIsValidate 是否有效的组件
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否有效
*/
BOOL Ex_ObjIsValidate(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjIsVisible 组件是否可视.
* @param 1  hObj                    [HEXOBJ]              组件句柄
* @return [BOOL]返回是否可视
*/
BOOL Ex_ObjIsVisible(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjKillFocus 销毁组件焦点
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjKillFocus(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjKillTimer 销毁组件时钟
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjKillTimer(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjLayoutClear 清空对象布局信息
* @param 1  handle                  [EXHANDLE]
* @param 2  bChildren               [BOOL]                是否清空所有子组件,否则为当前组件.
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjLayoutClear(EXHANDLE handle, BOOL bChildren);

/*
* @fnType ExDirectUI
* @brief Ex_ObjLayoutGet 获取布局对象句柄
* @param 1  handle                  [EXHANDLE]
* @return [HEXLAYOUT]返回布局句柄
*/
HEXLAYOUT Ex_ObjLayoutGet(EXHANDLE handle);

/*
* @fnType ExDirectUI
* @brief Ex_ObjLayoutSet 设置布局对象句柄
* @param 1  handle                  [EXHANDLE]
* @param 2  hLayout                 [HEXLAYOUT]
* @param 3  fUpdate                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjLayoutSet(EXHANDLE handle, HEXLAYOUT hLayout, BOOL fUpdate);

/*
* @fnType ExDirectUI
* @brief Ex_ObjLayoutUpdate 更新对象布局
* @param 1  handle                  [EXHANDLE]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjLayoutUpdate(EXHANDLE handle);

/*
* @fnType ExDirectUI
* @brief Ex_ObjMove 移动组件
* @param 1  hObj                    [HEXOBJ]
* @param 2  x                       [INT]
* @param 3  y                       [INT]
* @param 4  width                   [INT]
* @param 5  height                  [INT]
* @param 6  bRepaint                [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjMove(HEXOBJ hObj, INT x, INT y, INT width, INT height, BOOL bRepaint);

/*
* @fnType ExDirectUI
* @brief Ex_ObjPointTransform 坐标转换
* @param 1  hObjSrc                 [HEXOBJ]
* @param 2  hObjDst                 [HEXOBJ]                 0为所属窗口
* @param 3  ptX                     [INT*]                in/out
* @param 4  ptY                     [INT*]                in/out
* @return [BOOL] 返回是否成功
*/
BOOL Ex_ObjPointTransform(HEXOBJ hObjSrc, HEXOBJ hObjDst, INT* ptX, INT* ptY);

/*
* @fnType ExDirectUI
* @brief Ex_ObjPostMessage 投递消息
* @param 1  hObj                    [HEXOBJ]
* @param 2  uMsg                    [INT]
* @param 3  wParam                  [WPARAM]
* @param 4  lParam                  [LPARAM]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjPostMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjRegister 注册组件
* @param 1  lptszClassName          [LPCWSTR]           组件类名.最大允许长度:MAX_CLASS_NAME_LEN
* @param 2  dwStyle                 [INT]               组件默认风格    EOS_
* @param 3  dwStyleEx               [INT]               组件默认扩展风格 EOS_EX
* @param 4  dwTextFormat            [INT]               相关常量 DT_
* @param 5  cbObjExtra              [DWORD]             组件额外分配字节数(值可为0)
* @param 6  hCursor                 [HCURSOR]           组件默认鼠标指针句柄(值可为0)
* @param 7  dwFlags                 [INT]               画布标志 #ECVF_(值可为0)
* @param 8  pfnObjProc              [ClsPROC]			组件默认回调 LRESULT CALLBACK ClsPROC(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
* @return [EXATOM] 成功返回组件类名原子,失败返回0
*/
EXATOM Ex_ObjRegister(LPCWSTR lptszClassName, INT dwStyle, INT dwStyleEx, INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor, INT dwFlags, ClsPROC pfnObjProc);

/*
* @fnType ExDirectUI
* @brief Ex_ObjRemoveProp 移除属性
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwKey                   [size_t]
* @return [size_t]返回旧属性
*/
size_t Ex_ObjRemoveProp(HEXOBJ hObj, size_t dwKey);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollEnable 禁用/启用滚动条
* @param 1  hObj                    [HEXOBJ]
* @param 2  wSB                     [INT]                 支持SB_BOTH
* @param 3  wArrows                 [INT]                 相关常量 ESB_
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjScrollEnable(HEXOBJ hObj, INT wSB, INT wArrows);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollGetControl 获取滚动条句柄
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]                 相关常量 #SB_
* @return [HEXOBJ]返回滚动条句柄
*/
HEXOBJ Ex_ObjScrollGetControl(HEXOBJ hObj, INT nBar);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollGetInfo 获取滚动条信息
* @param 1  hObj                    [HEXOBJ]
* @param 2  fnBar                   [INT]
* @param 3  lpnMin                  [INT*]
* @param 4  lpnMax                  [INT*]
* @param 5  lpnPos                  [INT*]
* @param 6  lpnTrackPos             [INT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjScrollGetInfo(HEXOBJ hObj, INT nBar, INT* lpnMin, INT* lpnMax, INT* lpnPos, INT* lpnTrackPos);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollGetPos
* @details 获取滚动条位置
*
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]
* @return [INT]返回滚动条位置
*/
INT Ex_ObjScrollGetPos(HEXOBJ hObj, INT nBar);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollGetRange
* @details 获取滚动条范围
*
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]
* @param 3  lpnMinPos               [INT*]
* @param 4  lpnMaxPos               [INT*]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjScrollGetRange(HEXOBJ hObj, INT nBar, INT* lpnMinPos, INT* lpnMaxPos);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollGetTrackPos 获取滚动条拖动位置
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]
* @return [INT]返回拖动位置
*/
INT Ex_ObjScrollGetTrackPos(HEXOBJ hObj, INT nBar);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollSetInfo 设置滚动条信息
* @param 1  hObj                    [HEXOBJ]
* @param 2  fnBar                   [INT]                 SB_
* @param 3  fMask                   [INT]                 SIF_
* @param 4  nMin                    [INT]
* @param 5  nMax                    [INT]
* @param 6  nPage                   [INT]
* @param 7  nPos                    [INT]
* @param 8  fRedraw                 [BOOL]
* @return [INT]返回位置
*/
INT Ex_ObjScrollSetInfo(HEXOBJ hObj, INT nBar, INT Mask, INT nMin, INT nMax, INT nPage, INT nPos, BOOL bRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollSetPos 设置滚动条位置
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]                 SB_
* @param 3  nPos                    [INT]
* @param 4  bRedraw                 [BOOL]
* @return [INT]返回原位置
*/
INT Ex_ObjScrollSetPos(HEXOBJ hObj, INT nBar, INT nPos, BOOL bRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollSetRange 设置滚动条范围
* @param 1  hObj                    [HEXOBJ]
* @param 2  nBar                    [INT]                 SB_
* @param 3  nMin                    [INT]
* @param 4  nMax                    [INT]
* @param 5  bRedraw                 [BOOL]
* @return [INT]返回位置
*/
INT Ex_ObjScrollSetRange(HEXOBJ hObj, INT nBar, INT nMin, INT nMax, BOOL bRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjScrollShow 显示/隐藏滚动条
* @param 1  hObj                    [HEXOBJ]
* @param 2  wBar                    [INT]                 支持SB_BOTH
* @param 3  fShow                   [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjScrollShow(HEXOBJ hObj, INT wBar, BOOL fShow);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSendMessage 组件发送消息
* @param 1  hObj                    [HEXOBJ]
* @param 2  uMsg                    [INT]
* @param 3  wParam                  [WPARAM]
* @param 4  lParam                  [LPARAM]
* @return [size_t]返回结果
*/
size_t Ex_ObjSendMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetBackgroundImage 设置组件背景信息
* @param 1  handle                  [EXHANDLE]            引擎句柄/组件句柄
* @param 2  lpImage                 [LPVOID]              图片指针
* @param 3  dwImageLen              [size_t]              图片长度  0 为销毁背景 或调用 Ex_ObjDestroyBackground 销毁
* @param 4  X                       [INT]                 偏移X
* @param 5  Y                       [INT]                 偏移Y
* @param 6  dwRepeat                [DWORD]               相关常量 BIR_
* @param 7  lpGird                  [RECT*]              九宫矩形指针 (值可为0)
* @param 8  dwFlags                 [INT]               相关常量 BIF_
* @param 9  dwAlpha                 [DWORD]               透明度(0-255)
* @param 10 fUpdate                 [BOOL]                是否立即刷新
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetBackgroundImage(EXHANDLE handle, LPVOID lpImage, size_t dwImageLen, INT x, INT y, DWORD dwRepeat, RECT* lpGrid, INT dwFlags, DWORD dwAlpha, BOOL fUpdate);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetBackgroundPlayState 设置组件背景图片播放状态.
* @param 1  handle                  [EXHANDLE]            引擎句柄/组件句柄
* @param 2  fPlayFrames             [BOOL]                是否播放.
* @param 3  fResetFrame             [BOOL]                是否重置当前帧为首帧.
* @param 4  fUpdate                 [BOOL]                是否更新背景.
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetBackgroundPlayState(EXHANDLE handle, BOOL fPlayFrames, BOOL fResetFrame, BOOL fUpdate);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetBlur 设置组件模糊度
* @param 1  hObj                    [HEXOBJ]
* @param 2  fDeviation              [FLOAT]               模糊度
* @param 6  bRedraw                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetBlur(HEXOBJ hObj, FLOAT fDeviation, BOOL bRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetColor 设置组件相关颜色
* @param 1  hObj                    [HEXOBJ]
* @param 2  nIndex                  [INT]                 相关常量 COLOR_EX_
* @param 3  dwColor                 [EXARGB]
* @param 4  fRedraw                 [BOOL]                是否重画
* @return [EXARGB]返回修改前相关颜色
*/
EXARGB Ex_ObjSetColor(HEXOBJ hObj, INT nIndex, EXARGB dwColor, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEditSetSelCharFormat 设置编辑框选中行字符格式
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwMask                  [INT]                 相关常量 CFM_
* @param 3  crText                  [EXARGB]			  文本颜色,可空
* @param 4  wzFontFace              [LPCWSTR]             字体名称,可空
* @param 5  fontSize                [DWORD]               字体尺寸,可空
* @param 6  yOffset                 [INT]                 字体垂直偏移,可空
* @param 7  bBold                   [BOOL]                是否加粗,可空
* @param 8  bItalic                 [BOOL]                是否倾斜,可空
* @param 9  bUnderLine              [BOOL]                是否下划线,可空
* @param 10  bStrikeOut             [BOOL]                是否删除线,可空
* @param 11  bLink                  [BOOL]                是否超链接,可空
* @return [size_t]返回结果
*/
size_t Ex_ObjEditSetSelCharFormat(HEXOBJ hObj, INT dwMask, EXARGB crText = NULL, LPCWSTR wzFontFace = NULL, DWORD fontSize = NULL, INT yOffset = NULL, BOOL bBold = FALSE, BOOL bItalic = FALSE, BOOL bUnderLine = FALSE, BOOL bStrikeOut = FALSE, BOOL bLink = FALSE);

/*
* @fnType ExDirectUI
* @brief Ex_ObjEditSetSelParFormat 设置编辑框选中行段落格式
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwMask                  [INT]                 相关常量 PFM_
* @param 3  wNumbering              [WORD]			      编号类型,可空 PFN_常量
* @param 4  dxStartIndent           [INT]                 首行缩进,可空
* @param 5  dxRightIndent           [INT]                 右侧缩进,可空
* @param 6  dxOffset                [INT]                 非首行缩进,可空
* @param 7  wAlignment              [INT]                 段落对齐方式,可空 PFA_常量
* @return [size_t]返回结果
*/
size_t Ex_ObjEditSetSelParFormat(HEXOBJ hObj, DWORD dwMask, WORD wNumbering = NULL, INT dxStartIndent = NULL, INT dxRightIndent = NULL, INT dxOffset = NULL, WORD wAlignment = NULL);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetFocus 设置组件焦点
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetFocus(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetFont 设置组件文本字体
* @param 1  hObj                    [HEXOBJ]
* @param 2  hFont                   [HEXFONT]
* @param 3  fRedraw                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetFont(HEXOBJ hObj, HEXFONT hFont, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetFontFromFamily 从字体名称设置组件文本字体
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpszFontfamily          [LPCWSTR]             -1 为默认字体
* @param 3  dwFontsize              [INT]               -1 为默认尺寸
* @param 4  dwFontstyle             [INT]               -1 为默认风格
* @param 5  fRedraw                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetFontFromFamily(HEXOBJ hObj, LPCWSTR lpszFontfamily, INT dwFontsize, INT dwFontstyle, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetIMEState 设置窗口的输入法状态
* @param 1  hObjOrExDui             [EXHANDLE]
* @param 2  fOpen                   [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetIMEState(EXHANDLE hObjOrExDui, BOOL fOpen);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetLong 设置组件数值
* @param 1  hObj                    [HEXOBJ]
* @param 2  nIndex                  [INT]                 #EOL_
* @param 3  dwNewLong               [LONG_PTR]
* @return [LONG_PTR]返回旧数值
*/
LONG_PTR Ex_ObjSetLong(HEXOBJ hObj, INT nIndex, LONG_PTR dwNewLong);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetPadding 设置组件偏移矩形
* @param 1  hObj                    [HEXOBJ]
* @param 2  nPaddingType            [INT]                 EOPT_TEXT=0
* @param 3  left                    [INT]
* @param 4  top                     [INT]
* @param 5  right                   [INT]
* @param 6  bottom                  [INT]
* @param 7  fRedraw                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetPadding(HEXOBJ hObj, INT nPaddingType, INT left, INT top, INT right, INT bottom, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetParent 组件置父
* @param 1  hObj                    [HEXOBJ]              被置父的子控件
* @param 2  hParent                 [EXHANDLE]            新的父控件/皮肤/窗口 句柄
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetParent(HEXOBJ hObj, EXHANDLE hParent);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetPos 设置组件位置
* @param 1  hObj                    [HEXOBJ]
* @param 2  hObjInsertAfter         [HEXOBJ]
* @param 3  x                       [INT]
* @param 4  y                       [INT]
* @param 5  width                   [INT]
* @param 6  height                  [INT]
* @param 7  flags                   [INT]               相关常量 #SWP_
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetPos(HEXOBJ hObj, HEXOBJ hObjInsertAfter, INT x, INT y, INT width, INT height, INT flags);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetProp 设置组件属性
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwKey                   [size_t]
* @param 3  dwValue                 [size_t]
* @return [size_t]返回旧属性
*/
size_t Ex_ObjSetProp(HEXOBJ hObj, size_t dwKey, size_t dwValue);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetRadius 设置组件圆角度
* @param 1  hObj                    [HEXOBJ]
* @param 2  topleft                 [FLOAT]               左上角
* @param 3  topright                [FLOAT]               右上角
* @param 4  bottomright             [FLOAT]               右下角
* @param 5  bottomleft              [FLOAT]               左下角
* @param 6  fUpdate                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetRadius(HEXOBJ hObj, FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL fUpdate);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetRedraw 设置组件是否可以重画.如果组件扩展风格存在EOS_EX_COMPOSITED,则该函数无效.
* @param 1  hObj                    [HEXOBJ]              组件句柄
* @param 2  fCanbeRedraw            [BOOL]                是否可重画
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetRedraw(HEXOBJ hObj, BOOL fCanbeRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetText 设置组件文本.
* @param 1  hObj                    [HEXOBJ]              组件句柄.
* @param 2  lpString                [LPCWSTR]             指向一个空结束的字符串的指针.
* @param 3  fRedraw                 [BOOL]                是否重画组件
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetText(HEXOBJ hObj, LPCWSTR lpString, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetTextFormat 设置组件文本格式.
* @param 1  hObj                    [HEXOBJ]              组件句柄.
* @param 2  dwTextFormat            [DWORD]               DT_
* @param 3  fRedraw                 [BOOL]                是否重画组件
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetTextFormat(HEXOBJ hObj, DWORD dwTextFormat, BOOL bRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetTimer
* @details 设置组件时钟
*
* @param 1  hObj                    [HEXOBJ]
* @param 2  uElapse                 [INT]单位毫秒
* @return [INT]返回索引
*/
INT Ex_ObjSetTimer(HEXOBJ hObj, INT uElapse);

/*
* @fnType ExDirectUI
* @brief Ex_ObjSetUIState
* @details 设置组件状态,用于更新组件界面显示状态
*
* @param 1  hObj                    [HEXOBJ]
* @param 2  dwState                 [DWORD]
* @param 3  fRemove                 [BOOL]
* @param 4  lprcRedraw              [RECT*]
* @param 5  fRedraw                 [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjSetUIState(HEXOBJ hObj, DWORD dwState, BOOL fRemove, RECT* lprcRedraw, BOOL fRedraw);

/*
* @fnType ExDirectUI
* @brief Ex_ObjShow 设置组件可视状态
* @param 1  hObj                    [HEXOBJ]
* @param 2  fShow                   [BOOL]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjShow(HEXOBJ hObj, BOOL fShow);

/*
* @fnType ExDirectUI
* @brief Ex_ObjTooltipsPop 弹出或关闭提示文本
* @param 1  hObj                    [HEXOBJ]              组件句柄.
* @param 2  lpString                [LPCWSTR]             提示文本.该值为0则关闭提示文本
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjTooltipsPop(HEXOBJ hObj, LPCWSTR lpText);

/*
* @fnType ExDirectUI
* @brief Ex_ObjTooltipsPopEx 弹出或关闭提示文本Ex
* @param 1  hObj                    [HEXOBJ]              组件句柄.
* @param 2  lpTitle                 [LPCWSTR]             标题内容
* @param 3  lpText                  [LPCWSTR]             提示文本.
* @param 4  x                       [INT]                 屏幕坐标.(-1:默认)
* @param 5  y                       [INT]                 屏幕坐标.(-1:默认)
* @param 6  dwTime                  [INT]                 单位:毫秒.(-1:默认,0:始终显示)
* @param 7  nIcon                   [INT]                 #气泡图标_
* @param 8  fShow                   [BOOL]                是否立即显示
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjTooltipsPopEx(HEXOBJ hObj, LPCWSTR lpTitle, LPCWSTR lpText, INT x, INT y, INT dwTime, INT nIcon, BOOL fShow);

/*
* @fnType ExDirectUI
* @brief Ex_ObjTooltipsSetText 设置提示文本
* @param 1  hObj                    [HEXOBJ]
* @param 2  lpString                [LPCWSTR]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjTooltipsSetText(HEXOBJ hObj, LPCWSTR lpString);

/*
* @fnType ExDirectUI
* @brief Ex_ObjUpdate 立即更新组件
* @param 1  hObj                    [HEXOBJ]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ObjUpdate(HEXOBJ hObj);

/*
* @fnType ExDirectUI
* @brief Ex_ReadFile 读文件
* @param 1  filePath                [LPCWSTR]	文件路径
* @param 2  retData                 [std::vector<CHAR>*] 返回数据
* @return [BOOL]返回是否成功
*/
BOOL Ex_ReadFile(LPCWSTR filePath, std::vector<CHAR>* retData);

/*
* @fnType ExDirectUI
* @brief Ex_ReadResSource 读取RC资源
* @param 1  lpname                [WORD]				资源ID
* @param 2  lpType                [LPCWSTR]				资源类型
* @param 3  retData               [std::vector<CHAR>*]   返回数据
* @return [BOOL]返回是否成功
*/
BOOL Ex_ReadResSource(WORD lpname, LPCWSTR lpType, std::vector<CHAR>* retData);

/*
* @fnType ExDirectUI
* @brief Ex_ResFree 释放资源
* @param 1  hRes                    [HEXRES]
* @return [void] 无
*/
void Ex_ResFree(HEXRES hRes);

/*
* @fnType ExDirectUI
* @brief Ex_ResGetFile
* @details 获取资源文件
*
* @param 1  hRes                    [HEXRES]
* @param 2  lpwzPath                [LPCWSTR]
* @param 3  lpFile                  [LPVOID*]                [out]文件数据指针.用户不应该释放该内存.
* @param 4  dwFileLen               [size_t*]                [out]文件尺寸.
* @return [BOOL]返回是否成功
*/
BOOL Ex_ResGetFile(HEXRES hRes, LPCWSTR lpwzPath, LPVOID* lpFile, size_t* dwFileLen);

/*
* @fnType ExDirectUI
* @brief Ex_ResGetFileFromAtom 获取资源文件从路径原子
* @param 1  hRes                    [HEXRES]
* @param 2  atomPath                [EXATOM]	路径原子
* @param 3  lpFile                  [LPVOID*]      [out]文件数据指针.用户不应该释放该内存.
* @param 4  dwFileLen               [size_t*]      [out]文件尺寸.
* @return [BOOL]返回是否成功
*/
BOOL Ex_ResGetFileFromAtom(HEXRES hRes, EXATOM atomPath, LPVOID* lpFile, size_t* dwFileLen);

/*
* @fnType ExDirectUI
* @brief Ex_ResLoadFromFile 从文件加载资源
* @param 1  lpwzFile                [LPCWSTR]
* @return [HEXRES]返回资源包句柄
*/
HEXRES Ex_ResLoadFromFile(LPCWSTR lptszFile);

/*
* @fnType ExDirectUI
* @brief Ex_ResLoadFromMemory
* @details 从内存加载资源
*
* @param 1  lpData                  [LPVOID]
* @param 2  dwDataLen               [size_t]
* @return [HEXRES]返回资源包句柄
*/
HEXRES Ex_ResLoadFromMemory(LPVOID lpData, size_t dwDataLen);

/*
* @fnType ExDirectUI
* @brief Ex_Scale 取DPI缩放值
* @param 1  n                       [FLOAT]
* @return [FLOAT]返回缩放值
*/
FLOAT Ex_Scale(FLOAT n);

/*
* @fnType ExDirectUI
* @brief Ex_SetLastError 设置最后错误代码.相关常量 :#ERROR_EX_
* @param 1  nError                  [INT]
* @return [void] 无
*/
void Ex_SetLastError(INT nError);

/*
* @fnType ExDirectUI
* @brief Ex_Sleep 暂停
* @param 1  us                      [INT]
* @return [void] 无
*/
void Ex_Sleep(INT us);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeDrawControl 绘制主题数据
* @param 1  hTheme                  [HEXTHEME]            主题句柄
* @param 2  hCanvas                 [HEXCANVAS]           画板句柄
* @param 3  dstLeft                 [FLOAT]               目标左边
* @param 4  dstTop                  [FLOAT]               目标顶边
* @param 5  dstRight                [FLOAT]               目标右边
* @param 6  dstBottom               [FLOAT]               目标底边
* @param 7  atomClass               [EXATOM]
* @param 8  atomSrcRect             [EXATOM]
* @param 9  dwAlpha                 [DWORD]               透明度(0-255)
* @return [BOOL]返回是否成功
*/
BOOL Ex_ThemeDrawControl(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	EXATOM atomClass, EXATOM atomSrcRect, DWORD dwAlpha);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeDrawControlEx 绘制主题数据Ex
* @param 1  hTheme                  [HEXTHEME]            主题句柄
* @param 2  hCanvas                 [HEXCANVAS]           画板句柄
* @param 3  dstLeft                 [FLOAT]               目标左边
* @param 4  dstTop                  [FLOAT]               目标顶边
* @param 5  dstRight                [FLOAT]               目标右边
* @param 6  dstBottom               [FLOAT]               目标底边
* @param 7  atomClass               [EXATOM]
* @param 8  atomSrcRect             [EXATOM]
* @param 9  atomBackgroundRepeat    [EXATOM]
* @param 10 atomBackgroundPositon   [EXATOM]
* @param 11 atomBackgroundGrid      [EXATOM]
* @param 12 atomBackgroundFlags     [EXATOM]
* @param 13 dwAlpha                 [DWORD]               透明度(0-255)
* @return [BOOL]返回是否成功
*/
BOOL Ex_ThemeDrawControlEx(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
	EXATOM atomClass, EXATOM atomSrcRect, EXATOM atomBackgroundRepeat, EXATOM atomBackgroundPositon, EXATOM atomBackgroundGrid, EXATOM atomBackgroundFlags, DWORD dwAlpha);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeFree 释放主题
* @param 1  hTheme                  [HEXTHEME]
* @return [BOOL]返回是否成功
*/
BOOL Ex_ThemeFree(HEXTHEME hTheme);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeGetColor 获取主题相关颜色值
* @param 1  hTheme                  [HEXTHEME]            主题句柄
* @param 2  nIndex                  [INT]                 #COLOR_EX_
* @return [EXARGB]返回颜色
*/
EXARGB Ex_ThemeGetColor(HEXTHEME hTheme, INT nIndex);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeGetValuePtr 获取组件属性值指针.
* @param 1  hTheme                  [HEXTHEME]            主题句柄
* @param 2  atomClass               [EXATOM]
* @param 3  atomProp                [EXATOM]
* @return [LPVOID]返回属性值指针
*/
LPVOID Ex_ThemeGetValuePtr(HEXTHEME hTheme, EXATOM atomClass, EXATOM atomProp);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeLoadFromFile 加载主题包自文件
* @param 1  lptszFile               [LPCWSTR]
* @param 2  lpKey                   [LPVOID]
* @param 3  dwKeyLen                [size_t]
* @param 4  bDefault                [BOOL]
* @return [HEXTHEME]返回主题包指针
*/
HEXTHEME Ex_ThemeLoadFromFile(LPCWSTR lptszFile, LPVOID lpKey, size_t dwKeyLen, BOOL bDefault);

/*
* @fnType ExDirectUI
* @brief Ex_ThemeLoadFromMemory  加载主题包自内存
* @param 1  lpData                  [LPVOID]
* @param 2  dwDataLen               [size_t]
* @param 3  lpKey                   [LPVOID]
* @param 4  dwKeyLen                [size_t]
* @param 5  bDefault                [BOOL]
* @return [HEXTHEME]返回主题包指针
*/
HEXTHEME Ex_ThemeLoadFromMemory(LPVOID lpData, size_t dwDataLen, LPVOID lpKey, size_t dwKeyLen, BOOL bDefault);

/*
* @fnType ExDirectUI
* @brief Ex_TrackPopupMenu 弹出菜单
* @param 1  hMenu                   [HMENU]               菜单句柄
* @param 2  uFlags                  [DWORD]               相关常量 TPM_
* @param 3  x                       [INT]                 弹出坐标X(屏幕坐标)
* @param 4  y                       [INT]                 弹出坐标Y(屏幕坐标)
* @param 5  nReserved               [size_t]              0.备用
* @param 6  handle                  [EXHANDLE]            组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认菜单弹出)
* @param 7  lpRc                    [RECT*]               0.忽略
* @param 8  pfnCallback             [MsgPROC]           (LRESULT)MsgProc(HWND hWnd,size_t hExDui,INT uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult).
* @param 9  dwFlags                 [DWORD]               相关常量 EMNF_
* @return [BOOL]返回是否成功
*/
BOOL Ex_TrackPopupMenu(HMENU hMenu, DWORD uFlags, INT x, INT y, size_t nReserved, EXHANDLE handle, RECT* lpRC, MsgPROC pfnCallback, DWORD dwFlags);

/*
* @fnType ExDirectUI
* @brief Ex_UnInit 反初始化引擎
* @return [void] 无
*/
void Ex_UnInit();

/*
* @fnType ExDirectUI
* @brief Ex_WndCenterFrom 窗口居中
* @param 1  hWnd                    [HWND]                预居中的原始窗口
* @param 2  hWndFrom                [HWND]                 预居中的目标窗口,如果为0则为屏幕居中.
* @param 3  bFullScreen             [BOOL]                是否全屏模式
* @return [void] 无
*/
void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, BOOL bFullScreen);

/*
* @fnType ExDirectUI
* @brief Ex_WndCreate 创建窗口
* @param 1  hWndParent              [HWND]			父窗口句柄
* @param 2  lpwzClassName           [LPCWSTR]		窗口类名
* @param 3  lpwzWindowName          [LPCWSTR]		窗口标题
* @param 4  x                       [INT]			横坐标
* @param 5  y                       [INT]			纵坐标
* @param 6  Width                   [INT]			宽度
* @param 7  Height                  [INT]			高度
* @param 8  dwStyle                 [INT]			窗口风格
* @param 9  dwStyleEx               [INT]			窗口扩展风格
* @return [HWND]返回窗口句柄
*/
HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName, LPCWSTR lpwzWindowName, INT x, INT y, INT width, INT height, INT dwStyle, INT dwStyleEx);

/*
* @fnType ExDirectUI
* @brief Ex_WndMsgLoop 窗口消息循环
* @return [WPARAM]返回msg.wParam
*/
WPARAM Ex_WndMsgLoop();

/*
* @fnType ExDirectUI
* @brief Ex_WndRegisterClass 注册窗口类
* @param 1  lpwzClassName           [LPCWSTR]	窗口类名
* @param 2  hIcon                   [HICON]		窗口大图标
* @param 3  hIconsm                 [HICON]		窗口小图标
* @param 4  hCursor                 [HCURSOR]	窗口鼠标句柄
* @return [WORD]返回窗口类原子
*/
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm, HCURSOR hCursor);

/*
* @fnType ExDirectUI
* @brief __get 取指针size_t值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @return [size_t]返回值
*/
LONG_PTR __get(LPVOID lpAddr, LONG_PTR offset);

/*
* @fnType ExDirectUI
* @brief __get_int 取指针INT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @return [INT]返回值
*/
INT __get_int(LPVOID lpAddr, LONG_PTR offset);

/*
* @fnType ExDirectUI
* @brief __get_float 取指针FLOAT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @return [FLOAT]返回值
*/
FLOAT __get_float(LPVOID lpAddr, LONG_PTR offset);

/*
* @fnType ExDirectUI
* @brief __get_float 取指针SHORT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @return [SHORT]返回值
*/
SHORT __get_short(LPVOID lpAddr, LONG_PTR offset);

/*
* @fnType ExDirectUI
* @brief __set 置指针size_t值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		值
* @return [size_t]返回旧值
*/
LONG_PTR __set(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/*
* @fnType ExDirectUI
* @brief __set_float 置指针FLOAT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value					  [FLOAT]		值
* @return [FLOAT]返回旧值
*/
FLOAT __set_float(LPVOID lpAddr, LONG_PTR offset, FLOAT value);

/*
* @fnType ExDirectUI
* @brief __set_int 置指针INT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [INT]		值
* @return [INT]返回旧值
*/
INT __set_int(LPVOID lpAddr, LONG_PTR offset, INT value);

/*
* @fnType ExDirectUI
* @brief __set_short 置指针SHORT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [INT]		值
* @return [SHORT]返回旧值
*/
SHORT __set_short(LPVOID lpAddr, LONG_PTR offset, SHORT value);

/*
* @fnType ExDirectUI
* @brief __query 指针位查询size_t值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		查询值
* @return [BOOL]返回是否存在
*/
BOOL __query(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/*
* @fnType ExDirectUI
* @brief __query_int 指针位查询INT值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [INT]		查询值
* @return [BOOL]返回是否存在
*/
BOOL __query_int(LPVOID lpAddr, LONG_PTR offset, INT value);

/*
* @fnType ExDirectUI
* @brief __del 指针位删除size_t值
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		删除值
* @return [void]
*/
void __del(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/*
* @fnType ExDirectUI
* @brief __add 指针位添加size_t值(位或)
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		添加值
* @return [void]
*/
void __add(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/*
* @fnType ExDirectUI
* @brief __addn 指针位添加size_t值(相加)
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		添加值
* @return [void]
*/
void __addn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/*
* @fnType ExDirectUI
* @brief __addn 指针位减少size_t值(相减)
* @param 1  lpAddr                    [LPVOID]		指针
* @param 2  offset                    [size_t]		偏移
* @param 3  value                     [size_t]		减少值
* @return [void]
*/
void __subn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);