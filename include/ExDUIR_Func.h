#pragma once
#include "ExDUIR_Struct.h"

/// <summary>
/// 画刷创建
/// </summary>
/// <param name="argb"></param>
/// <returns></returns>
HEXBRUSH _brush_create(EXARGB argb);

/// <summary>
/// 画刷创建自画布
/// </summary>
/// <param name="hCanvas"></param>
/// <returns></returns>
HEXBRUSH _brush_createfromcanvas(HEXCANVAS hCanvas);

/// <summary>
/// 画刷创建自画布2
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="alpha"></param>
/// <returns></returns>
HEXBRUSH _brush_createfromcanvas2(HEXCANVAS hCanvas, DWORD alpha);

/// <summary>
/// 画刷创建自图片句柄
/// </summary>
/// <param name="hImg"></param>
/// <returns></returns>
HEXBRUSH _brush_createfromimg(HEXIMAGE hImg);

/// <summary>
/// 创建线性渐变画刷
/// </summary>
/// <param name="xStart"></param>
/// <param name="yStart"></param>
/// <param name="xEnd"></param>
/// <param name="yEnd"></param>
/// <param name="crBegin">起点颜色ARGB</param>
/// <param name="crEnd">终点颜色ARGB</param>
/// <returns></returns>
HEXBRUSH _brush_createlinear(FLOAT xStart, FLOAT yStart, FLOAT xEnd, FLOAT yEnd,
                             EXARGB crBegin, EXARGB crEnd);

/// <summary>
/// 创建线性渐变画刷2
/// </summary>
/// <param name="xStart">起点X</param>
/// <param name="yStart">起点Y</param>
/// <param name="xEnd">终点X</param>
/// <param name="yEnd">终点Y</param>
/// <param name="arrPts">位置数组(取值0-1.0),两个</param>
/// <param name="arrColors">ARGB颜色数组,两个</param>
/// <returns></returns>
HEXBRUSH _brush_createlinear_ex(FLOAT xStart, FLOAT yStart, FLOAT xEnd,
                                FLOAT yEnd, FLOAT* arrPts, INT* arrColors);

/// <summary>
/// 画刷销毁
/// </summary>
/// <param name="hBrush"></param>
/// <returns></returns>
BOOL _brush_destroy(HEXBRUSH hBrush);

/// <summary>
/// 画刷置颜色
/// </summary>
/// <param name="hBrush"></param>
/// <param name="argb"></param>
/// <returns>返回旧颜色</returns>
EXARGB _brush_setcolor(HEXBRUSH hBrush, EXARGB argb);

/// <summary>
/// 画刷置矩阵
/// </summary>
/// <param name="hBrush"></param>
/// <param name="matrix"></param>
void _brush_settransform(HEXBRUSH hBrush, HEXMATRIX matrix);

/// <summary>
/// 画布应用效果器
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hEffect">效果器指针</param>
void _canvas_applyeffect(HEXCANVAS hCanvas, HEXEFFECT hEffect);

/// <summary>
/// 画布开始绘制
/// </summary>
/// <param name="hCanvas"></param>
/// <returns></returns>
BOOL _canvas_begindraw(HEXCANVAS hCanvas);

/// <summary>
/// 画布模糊
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="fDeviation"></param>
/// <param name="lprc"></param>
/// <returns></returns>
BOOL _canvas_blur(HEXCANVAS hCanvas, FLOAT fDeviation, RECT* lprc);

/// <summary>
/// 画布计算文本尺寸
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hFont"></param>
/// <param name="lpwzText"></param>
/// <param name="dwLen"></param>
/// <param name="dwDTFormat"></param>
/// <param name="layoutWidth"></param>
/// <param name="layoutHeight"></param>
/// <param name="lpWidth"></param>
/// <param name="lpHeight"></param>
/// <returns></returns>
BOOL _canvas_calctextsize(HEXCANVAS hCanvas, HEXFONT hFont, LPCWSTR lpwzText,
                          INT dwLen, INT dwDTFormat, FLOAT layoutWidth,
                          FLOAT layoutHeight, FLOAT* lpWidth, FLOAT* lpHeight);

/// <summary>
/// 画布清除背景
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="Color"></param>
/// <returns></returns>
BOOL _canvas_clear(HEXCANVAS hCanvas, EXARGB Color);

/// <summary>
/// 画布置剪辑区
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
BOOL _canvas_cliprect(HEXCANVAS hCanvas, INT left, INT top, INT right,
                      INT bottom);

/// <summary>
/// 画布创建自引擎句柄
/// </summary>
/// <param name="hExDui"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="dwFlags">画布标识常量CANVAS_FLAG_</param>
/// <returns></returns>
HEXCANVAS _canvas_createfromexdui(HEXDUI hExDui, INT width, INT height,
                                  INT dwFlags);

/// <summary>
/// 画布创建自组件句柄
/// </summary>
/// <param name="hObj"></param>
/// <param name="uWidth"></param>
/// <param name="uHeight"></param>
/// <param name="dwFlags">画布标识常量CANVAS_FLAG_</param>
/// <returns></returns>
HEXCANVAS _canvas_createfromobj(HEXOBJ hObj, INT uWidth, INT uHeight,
                                INT dwFlags);

/// <summary>
/// 创建独立画布
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="dwFlags">画布标识常量CANVAS_FLAG_</param>
/// <returns></returns>
HEXCANVAS _canvas_createindependent(INT width, INT height, INT dwFlags);

/// <summary>
/// 画布销毁
/// </summary>
/// <param name="hCanvas"></param>
/// <returns></returns>
BOOL _canvas_destroy(HEXCANVAS hCanvas);

/// <summary>
/// 画布画画布
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="sCanvas"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="srcLeft"></param>
/// <param name="srcTop"></param>
/// <param name="dwAlpha"></param>
/// <param name="dwCompositeMode">#CANVAS_COMPOSITE_MODE_</param>
/// <returns></returns>
BOOL _canvas_drawcanvas(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft,
                        INT dstTop, INT dstRight, INT dstBottom, INT srcLeft,
                        INT srcTop, DWORD dwAlpha, DWORD dwCompositeMode);

/// <summary>
/// 画布画曲线
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="points">坐标点</param>
/// <param name="count">点个数</param>
/// <param name="tension">张力系数 (0,1] 大于0小于等于1</param>
/// <param name="strokeWidth"></param>
/// <param name="strokeStyle"></param>
void _canvas_drawcurve(HEXCANVAS hCanvas, HEXBRUSH hBrush, const POINTF* points,
                       INT count, FLOAT tension, FLOAT strokeWidth,
                       DWORD strokeStyle);

/// <summary>
/// 画布画椭圆
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="radiusX"></param>
/// <param name="radiusY"></param>
/// <param name="strokeWidth"></param>
/// <param name="strokeStyle"></param>
/// <returns></returns>
BOOL _canvas_drawellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y,
                         FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth,
                         DWORD strokeStyle);

/// <summary>
/// 画布画图片
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hImage"></param>
/// <param name="Left"></param>
/// <param name="Top"></param>
/// <param name="alpha"></param>
/// <returns></returns>
BOOL _canvas_drawimage(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left,
                       FLOAT Top, DWORD alpha);

/// <summary>
/// 画布画九宫矩形
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hImage"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="srcLeft"></param>
/// <param name="srcTop"></param>
/// <param name="srcRight"></param>
/// <param name="srcBottom"></param>
/// <param name="gridPaddingLeft"></param>
/// <param name="gridPaddingTop"></param>
/// <param name="gridPaddingRight"></param>
/// <param name="gridPaddingBottom"></param>
/// <param name="dwFlags">#BACKGROUND_FLAG_</param>
/// <param name="dwAlpha"></param>
/// <returns></returns>
BOOL _canvas_drawimagefromgrid(HEXCANVAS hCanvas, HEXIMAGE hImage,
                               FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight,
                               FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
                               FLOAT srcRight, FLOAT srcBottom,
                               FLOAT gridPaddingLeft, FLOAT gridPaddingTop,
                               FLOAT gridPaddingRight, FLOAT gridPaddingBottom,
                               INT dwFlags, DWORD dwAlpha);

/// <summary>
/// 画布画图像矩形
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hImage"></param>
/// <param name="Left"></param>
/// <param name="Top"></param>
/// <param name="Right"></param>
/// <param name="Bottom"></param>
/// <param name="alpha"></param>
/// <returns></returns>
BOOL _canvas_drawimagerect(HEXCANVAS hCanvas, HEXIMAGE hImage, FLOAT Left,
                           FLOAT Top, FLOAT Right, FLOAT Bottom, DWORD alpha);

/// <summary>
/// 画布画图像缩放到
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hImage"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="srcLeft"></param>
/// <param name="srcTop"></param>
/// <param name="srcRight"></param>
/// <param name="srcBottom"></param>
/// <param name="alpha"></param>
/// <returns></returns>
BOOL _canvas_drawimagerectrect(HEXCANVAS hCanvas, HEXIMAGE hImage,
                               FLOAT dstLeft, FLOAT dstTop, FLOAT dstRight,
                               FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
                               FLOAT srcRight, FLOAT srcBottom, DWORD alpha);

/// <summary>
/// 画布画直线
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="X1"></param>
/// <param name="Y1"></param>
/// <param name="X2"></param>
/// <param name="Y2"></param>
/// <param name="strokeWidth"></param>
/// <param name="strokeStyle">
///	0无分隔符的实线
/// 1短划线，后跟长度相等的间隙
/// 2一个点，后跟一个较长的间距
/// 3一个短划线，后跟一个间隙，后跟一个点，然后是另一个间隙
/// 4破折号，后跟一个间隙，后跟一个点，后跟另一个间隙，后跟另一个点，然后是另一个间距
/// 5短划线模式由浮点值数组指定</param>
/// <returns></returns>
BOOL _canvas_drawline(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT X1, FLOAT Y1,
                      FLOAT X2, FLOAT Y2, FLOAT strokeWidth, DWORD strokeStyle);

/// <summary>
/// 画布画路径
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hPath">路径句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="strokeWidth">线条宽度</param>
/// <param name="strokeStyle">线条中间样式类型，0实线</param>
/// <param name="lineCap">线条两端样式默认1。 1直角,2圆角</param>
/// <returns></returns>
BOOL _canvas_drawpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush,
                      FLOAT strokeWidth, DWORD strokeStyle, DWORD lineCap = 1U);

/// <summary>
/// 画布画二次方贝塞尔曲线
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="points">坐标点，必须是3个</param>
/// <param name="count">坐标点个数，必须是3个</param>
/// <param name="strokeWidth"></param>
/// <param name="strokeStyle"></param>
void _canvas_drawquadraticbezier(HEXCANVAS hCanvas, HEXBRUSH hBrush,
                                 const POINTF* points, INT count,
                                 FLOAT strokeWidth, DWORD strokeStyle);

/// <summary>
/// 画布画区域
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="HEXRGN">区域句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="strokeWidth">线条宽度</param>
/// <param name="strokeStyle">线条中间样式类型，0实线</param>
/// <returns></returns>
BOOL _canvas_drawregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush,
                        FLOAT strokeWidth, DWORD strokeStyle);

/// <summary>
/// 画布画多边形
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="left">绘制多边形外接椭圆的左边</param>
/// <param name="top">绘制多边形外接椭圆的顶边</param>
/// <param name="right">绘制多边形外接椭圆的右边</param>
/// <param name="bottom">绘制多边形外接椭圆的底边</param>
/// <param name="NumberOfEdges">边数</param>
/// <param name="Angle">角度</param>
/// <param name="strokeWidth">绘制的线宽</param>
/// <param name="strokeStyle">绘制的线型</param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawpolygon(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left,
                         FLOAT top, FLOAT right, FLOAT bottom,
                         UINT numberOfEdges, FLOAT angle, FLOAT strokeWidth,
                         UINT strokeStyle);

/// <summary>
/// 画布画矩形
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="strokeWidth"></param>
/// <param name="strokeStyle"></param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top,
                      FLOAT right, FLOAT bottom, FLOAT strokeWidth,
                      DWORD strokeStyle);

/// <summary>
/// 画布画圆角矩形
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="radiusX">横向圆角度</param>
/// <param name="radiusY">纵向圆角度</param>
/// <param name="strokeWidth">画刷宽度</param>
/// <param name="strokeStyle"></param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left,
                             FLOAT top, FLOAT right, FLOAT bottom,
                             FLOAT radiusX, FLOAT radiusY, FLOAT strokeWidth,
                             DWORD strokeStyle);

/// <summary>
/// 画布画阴影
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="fLeft">阴影包围左边</param>
/// <param name="fTop">阴影包围顶边</param>
/// <param name="fRight">阴影包围右边</param>
/// <param name="fBottom">阴影包围底边</param>
/// <param name="fShadowSize">阴影尺寸</param>
/// <param name="crShadow">阴影颜色</param>
/// <param name="radiusTopLeft">左上角圆角度</param>
/// <param name="radiusTopRight">右上角圆角度</param>
/// <param name="radiusBottomLeft">左下角圆角度</param>
/// <param name="radiusBottomRight">右下角圆角度</param>
/// <param name="OffsetX">阴影横向偏移</param>
/// <param name="OffsetY">阴影纵向偏移</param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawshadow(HEXCANVAS hCanvas, FLOAT fLeft, FLOAT fTop,
                        FLOAT fRight, FLOAT fBottom, FLOAT fShadowSize,
                        EXARGB crShadow, FLOAT radiusTopLeft,
                        FLOAT radiusTopRight, FLOAT radiusBottomLeft,
                        FLOAT radiusBottomRight, FLOAT OffsetX, FLOAT OffsetY);

/// <summary>
/// 画布画文本
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hFont">如果为0则使用默认字体句柄</param>
/// <param name="crText"></param>
/// <param name="lpwzText"></param>
/// <param name="dwLen">-1则自动计算尺寸(必须是指向空字符串的指针)</param>
/// <param name="dwDTFormat">#DT_</param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawtext(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText,
                      LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, FLOAT left,
                      FLOAT top, FLOAT right, FLOAT bottom);

/// <summary>
/// 画布画文本2
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hFont">如果为0则使用默认字体句柄</param>
/// <param name="hBrush"></param>
/// <param name="lpwzText"></param>
/// <param name="dwLen">-1则自动计算尺寸(必须是指向空字符串的指针)</param>
/// <param name="dwDTFormat">#DT_</param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawtext2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBrush,
                       LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, FLOAT left,
                       FLOAT top, FLOAT right, FLOAT bottom);

/// <summary>
/// 画布画文本Ex
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hFont">字体句柄</param>
/// <param name="crText">颜色</param>
/// <param name="lpwzText">内容</param>
/// <param name="dwLen">长度</param>
/// <param name="dwDTFormat">文本格式</param>
/// <param name="left">目标左边</param>
/// <param name="top">目标顶边</param>
/// <param name="right">目标右边</param>
/// <param name="bottom">目标底边</param>
/// <returns>返回是否成功</returns>
BOOL _canvas_drawtextex(HEXCANVAS hCanvas, HEXFONT hFont, EXARGB crText,
                        LPCWSTR lpwzText, INT dwLen, INT dwDTFormat, FLOAT left,
                        FLOAT top, FLOAT right, FLOAT bottom);

/// <summary>
/// 画布_画svg句柄
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hSvg">svg句柄</param>
/// <param name="Left">左边</param>
/// <param name="Top">顶边</param>
/// <param
/// name="width">宽度(将与SVG的ViewBox属性的width比值进行缩放,取宽高最小缩放比缩放),为0不缩放</param>
/// <param
/// name="height">高度(将与SVG的ViewBox属性的height比值进行缩放,取宽高最小缩放比缩放),为0不缩放</param>
/// <returns></returns>
BOOL _canvas_drawsvg(HEXCANVAS hCanvas, HEXSVG hSvg, FLOAT Left, FLOAT Top, FLOAT width, FLOAT height);

/// <summary>
/// 同_canvas_drawhSvg, width和height为画布大小
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hSvg"></param>
/// <param name="Left"></param>
/// <param name="Top"></param>
/// <returns></returns>
BOOL _canvas_drawsvg2(HEXCANVAS hCanvas, HEXSVG hSvg, FLOAT Left, FLOAT Top);

/// <summary>
/// 画布结束绘制
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <returns></returns>
BOOL _canvas_enddraw(HEXCANVAS hCanvas);

/// <summary>
/// 画布_拷贝画布到目标画布(替换目标画布内容)
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="sCanvas"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="srcLeft"></param>
/// <param name="srcTop"></param>
/// <returns></returns>
BOOL _canvas_bitblt(HEXCANVAS hCanvas, HEXCANVAS sCanvas, INT dstLeft, INT dstTop, INT dstRight,
    INT dstBottom, INT srcLeft, INT srcTop);

/// <summary>
/// 画布_透明混合(画布内容混合)
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="sCanvas"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="srcLeft"></param>
/// <param name="srcTop"></param>
/// <param name="srcRight"></param>
/// <param name="srcBottom"></param>
/// <param name="alpha"></param>
/// <returns></returns>
BOOL _canvas_alphablend(HEXCANVAS hCanvas, HEXCANVAS sCanvas, FLOAT dstLeft, FLOAT dstTop,
    FLOAT dstRight, FLOAT dstBottom, FLOAT srcLeft, FLOAT srcTop,
    FLOAT srcRight, FLOAT srcBottom, INT alpha);

/// <summary>
/// 画布填充椭圆
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="x">圆心横坐标</param>
/// <param name="y">圆心纵坐标</param>
/// <param name="radiusX">横向半径</param>
/// <param name="radiusY">纵向半径</param>
/// <returns></returns>
BOOL _canvas_fillellipse(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT x, FLOAT y,
                         FLOAT radiusX, FLOAT radiusY);

/// <summary>
/// 画布填充路径
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hPath">路径句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <returns></returns>
BOOL _canvas_fillpath(HEXCANVAS hCanvas, HEXPATH hPath, HEXBRUSH hBrush);

/// <summary>
/// 在画布上填充一个多边形
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="left">填充多边形外接椭圆的坐标</param>
/// <param name="top">填充多边形外接椭圆的坐标</param>
/// <param name="right">填充多边形外接椭圆的坐标</param>
/// <param name="bottom">填充多边形外接椭圆的坐标</param>
/// <param name="NumberOfEdges">边数</param>
/// <param name="Angle">角度</param>
/// <returns></returns>
BOOL _canvas_fillpolygon(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left,
                         FLOAT top, FLOAT right, FLOAT bottom,
                         UINT numberOfEdges, FLOAT angle);

/// <summary>
/// 画布填充矩形
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <param name="left">矩形左边</param>
/// <param name="top">矩形顶边</param>
/// <param name="right">矩形右边</param>
/// <param name="bottom">矩形底边</param>
/// <returns></returns>
BOOL _canvas_fillrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left, FLOAT top,
                      FLOAT right, FLOAT bottom);

/// <summary>
/// 画布填充区域
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hRgn">区域句柄</param>
/// <param name="hBrush">画刷句柄</param>
/// <returns></returns>
BOOL _canvas_fillregion(HEXCANVAS hCanvas, HEXRGN hRgn, HEXBRUSH hBrush);

/// <summary>
/// 画布填充圆角图片
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="hImg">图像句柄</param>
/// <param name="left">目标左边</param>
/// <param name="top">目标顶边</param>
/// <param name="width">目标宽度</param>
/// <param name="height">目标高度</param>
/// <param name="radiusX">横向圆角度</param>
/// <param name="radiusY">纵向圆角度</param>
/// <param name="shadowNum">透明度数组</param>
/// <param name="number">透明度数量</param>
/// <param name="crShadow">阴影颜色</param>
/// <returns></returns>
BOOL _canvas_fillroundedimage(HEXCANVAS hCanvas, HEXIMAGE hImg, FLOAT left,
                              FLOAT top, FLOAT width, FLOAT height,
                              FLOAT radiusX, FLOAT radiusY, INT* shadowNum,
                              INT number, EXARGB crShadow);

/// <summary>
/// 画布填充圆角矩形
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="hBrush"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="radiusX"></param>
/// <param name="radiusY"></param>
/// <returns></returns>
BOOL _canvas_fillroundedrect(HEXCANVAS hCanvas, HEXBRUSH hBrush, FLOAT left,
                             FLOAT top, FLOAT right, FLOAT bottom,
                             FLOAT radiusX, FLOAT radiusY);

/// <summary>
/// 画布刷新
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <returns></returns>
BOOL _canvas_flush(HEXCANVAS hCanvas);

/// <summary>
/// 获取画布信息
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="nType">画布信息类型 CVC_DX常量</param>
/// <returns></returns>
LPVOID _canvas_getcontext(HEXCANVAS hCanvas, INT nType);

/// <summary>
/// 画布取DC
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <returns></returns>
HDC _canvas_getdc(HEXCANVAS hCanvas);

/// <summary>
/// 画布取尺寸
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="width">返回宽度</param>
/// <param name="height">返回高度</param>
/// <returns></returns>
BOOL _canvas_getsize(HEXCANVAS hCanvas, INT* width, INT* height);

/// <summary>
/// 画布取矩阵
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="pMatrix">返回矩阵</param>
/// <returns></returns>
BOOL _canvas_gettransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix);

/// <summary>
/// 画布释放DC
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <returns></returns>
BOOL _canvas_releasedc(HEXCANVAS hCanvas);

/// <summary>
/// 画布重置剪辑区
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <returns></returns>
BOOL _canvas_resetclip(HEXCANVAS hCanvas);

/// <summary>
/// 画布重新设置尺寸
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="width">新宽度</param>
/// <param name="height">新高度</param>
/// <returns></returns>
BOOL _canvas_resize(HEXCANVAS hCanvas, INT width, INT height);

/// <summary>
/// 画布旋转色相
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="fAngle">旋转角度 0-360</param>
/// <returns></returns>
BOOL _canvas_rotate_hue(HEXCANVAS hCanvas, FLOAT fAngle);

/// <summary>
/// 画布设置图形抗锯齿
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="antialias">是否抗锯齿</param>
/// <returns></returns>
BOOL _canvas_setantialias(HEXCANVAS hCanvas, BOOL antialias);

/// <summary>
/// 画布设置图像抗锯齿,等效于同时设置文本和图形抗锯齿
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="antialias">是否抗锯齿</param>
/// <returns></returns>
BOOL _canvas_setimageantialias(HEXCANVAS hCanvas, BOOL antialias);

/// <summary>
/// 画布设置文本抗锯齿
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="antialias">是否抗锯齿</param>
/// <returns></returns>
BOOL _canvas_settextantialiasmode(HEXCANVAS hCanvas, BOOL antialias);

/// <summary>
/// 画布置矩阵
/// </summary>
/// <param name="hCanvas">画布句柄</param>
/// <param name="pMatrix">矩阵句柄 0.则重置</param>
/// <returns></returns>
BOOL _canvas_settransform(HEXCANVAS hCanvas, HEXMATRIX pMatrix);

/// <summary>
/// 缓动创建
/// </summary>
/// <param name="dwType">缓动类型 常量EASING_TYPE_</param>
/// <param
/// name="pEasingContext">缓动类型为EASING_TYPE_CUSTOM时，值为自定义回调函数(nProcess,nStart,nStop,nCurrent*,pEasingContext)</param>
/// <param name="dwMode">缓动模式 常量EASING_MODE_的组合</param>
/// <param name="pContext">自定义参数,可以是值或回调地址</param>
/// <param name="nTotalTime">总耗时 毫秒</param>
/// <param name="nInterval">间隔耗时 毫秒</param>
/// <param name="nState">缓动状态 常量EASING_STATE_</param>
/// <param name="nStart">开始值</param>
/// <param name="nStop">结束值</param>
/// <param name="param1">参数1</param>
/// <param name="param2">参数2</param>
/// <param name="param3">参数3</param>
/// <param name="param4">参数4</param>
/// <returns>缓动指针</returns>
HEXEASING _easing_create(DWORD dwType, LPVOID pEasingContext, DWORD dwMode,
                         LONG_PTR pContext, INT nTotalTime, INT nInterval,
                         DWORD nState, INT nStart, INT nStop, LONG_PTR param1,
                         LONG_PTR param2, LONG_PTR param3, LONG_PTR param4);

/// <summary>
/// 缓动取状态
/// </summary>
/// <param name="pEasing">缓动指针</param>
/// <returns></returns>
DWORD _easing_getstate(HEXEASING pEasing);

/// <summary>
/// 缓动置状态
/// </summary>
/// <param name="pEasing">缓动指针</param>
/// <param name="nState">状态</param>
/// <returns></returns>
BOOL _easing_setstate(HEXEASING pEasing, DWORD nState);

/// <summary>
/// 效果器创建
/// </summary>
/// <param name="guid">效果器GUID</param>
/// <param name="ret">返回效果器指针</param>
/// <returns></returns>
BOOL _effect_create(GUID guid, HEXEFFECT* ret);

/// <summary>
/// 效果器创建缓冲区
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
void _effect_create_buffer(HEXEFFECT hEffect, int width, int height);

/// <summary>
/// 效果器销毁
/// </summary>
/// <param name="hEffect">效果器指针</param>
void _effect_destroy(HEXEFFECT hEffect);

/// <summary>
/// 效果器注册
/// </summary>
/// <param name="guid">效果器GUID</param>
/// <param name="pProperityXml">效果器描述xml内容</param>
/// <param name="properitys">效果器属性绑定方法</param>
/// <param name="properityCount">效果器属性个数</param>
/// <param name="createEffect">效果器创建方法地址</param>
/// <returns></returns>
BOOL _effect_register(const GUID& guid, LPCWSTR pProperityXml,
                      EX_EFFECT_PROPERITY_INFO* properitys, int properityCount,
                      LPVOID createEffect);

/// <summary>
/// 效果器置布尔值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value">值</param>
BOOL _effect_set_bool(HEXEFFECT hEffect, PCWSTR lpszProperityName, BOOL value);

/// <summary>
/// 效果器置浮点值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value">值</param>
BOOL _effect_set_float(HEXEFFECT hEffect, PCWSTR lpszProperityName,
                       float value);

/// <summary>
/// 效果器置整数值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value">值</param>
BOOL _effect_set_int32(HEXEFFECT hEffect, PCWSTR lpszProperityName, INT value);

/// <summary>
/// 效果器置无符号整数值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value">值</param>
BOOL _effect_set_uint32(HEXEFFECT hEffect, PCWSTR lpszProperityName,
                        UINT value);

/// <summary>
/// 效果器置vector2值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value1">值1</param>
/// <param name="value2">值2</param>
BOOL _effect_set_vector2(HEXEFFECT hEffect, PCWSTR lpszProperityName,
                         float value1, float value2);

/// <summary>
/// 效果器置vector3值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value1">值1</param>
/// <param name="value2">值2</param>
/// <param name="value3">值3</param>
BOOL _effect_set_vector3(HEXEFFECT hEffect, PCWSTR lpszProperityName,
                         float value1, float value2, float value3);

/// <summary>
/// 效果器置vector4值
/// </summary>
/// <param name="hEffect">效果器指针</param>
/// <param name="lpszProperityName">属性名</param>
/// <param name="value1">值1</param>
/// <param name="value2">值2</param>
/// <param name="value3">值3</param>
/// <param name="value4">值4</param>
BOOL _effect_set_vector4(HEXEFFECT hEffect, PCWSTR lpszProperityName,
                         float value1, float value2, float value3,
                         float value4);

/// <summary>
/// 创建默认字体
/// </summary>
/// <returns></returns>
HEXFONT _font_create();

/// <summary>
/// 创建字体自字体族
/// </summary>
/// <param name="lpwzFontFace"></param>
/// <param name="dwFontSize"></param>
/// <param name="dwFontStyle"></param>
/// <returns></returns>
HEXFONT _font_createfromfamily(LPCWSTR lpwzFontFace, INT dwFontSize,
                               DWORD dwFontStyle);

/// <summary>
/// 创建字体自文件
/// </summary>
/// <param name="FontFilePaths"></param>
/// <param name="dwFontSize"></param>
/// <param name="dwFontStyle"></param>
/// <returns></returns>
HEXFONT _font_createfromfile(LPCWSTR FontFilePaths, INT dwFontSize = -1,
                             DWORD dwFontStyle = -1);

/// <summary>
/// 创建字体自内存
/// </summary>
/// <param name="fontDate">字体数据指针</param>
/// <param name="fontDateLen">字体数据长度</param>
/// <param name="dwFontSize">字体尺寸大小</param>
/// <param name="dwFontStyle">字体风格</param>
/// <returns>字体句柄</returns>
HEXFONT _font_createfrommem(const uint8_t* fontDate, size_t fontDateLen,
                            INT dwFontSize = -1, DWORD dwFontStyle = -1);

/// <summary>
/// 创建字体自逻辑字体
/// </summary>
/// <param name="lpLogfont"></param>
/// <returns></returns>
HEXFONT _font_createfromlogfont(LOGFONTW* lpLogfont);

/// <summary>
/// 字体销毁
/// </summary>
/// <param name="hFont"></param>
/// <returns></returns>
BOOL _font_destroy(HEXFONT hFont);

/// <summary>
/// 字体取描述表
/// </summary>
/// <param name="hFont"></param>
/// <returns></returns>
LPVOID _font_getcontext(HEXFONT hFont);

/// <summary>
/// 获取逻辑字体
/// </summary>
/// <param name="hFont"></param>
/// <param name="lpLogFont">返回逻辑字体</param>
/// <returns></returns>
BOOL _font_getlogfont(HEXFONT hFont, LOGFONTW* lpLogFont);

/// <summary>
/// 图像裁剪
/// </summary>
/// <param name="hImg"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="phImg">返回新图像句柄</param>
/// <returns></returns>
BOOL _img_clip(HEXIMAGE hImg, INT left, INT top, INT width, INT height,
               HEXIMAGE* phImg);

/// <summary>
/// 图像复制
/// </summary>
/// <param name="hImg"></param>
/// <param name="phImg">返回新图像句柄</param>
/// <returns></returns>
BOOL _img_copy(HEXIMAGE hImg, HEXIMAGE* phImg);

/// <summary>
/// 图像复制部分
/// </summary>
/// <param name="hImg"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="phImg">返回新图像句柄</param>
/// <returns></returns>
BOOL _img_copyrect(HEXIMAGE hImg, INT x, INT y, INT width, INT height,
                   HEXIMAGE* phImg);

/// <summary>
/// 图像创建
/// </summary>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="phImg">返回新图像句柄</param>
/// <returns></returns>
BOOL _img_create(INT width, INT height, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自画布句柄
/// </summary>
/// <param name="hCanvas"></param>
/// <param name="dstImg"></param>
/// <returns></returns>
BOOL _img_createfromcanvas(HEXCANVAS hCanvas, HEXIMAGE* dstImg);

/// <summary>
/// 图像创建自文件
/// </summary>
/// <param name="lpwzFilename"></param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfromfile(LPCWSTR lpwzFilename, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自位图句柄
/// </summary>
/// <param name="hBitmap"></param>
/// <param name="hPalette"></param>
/// <param name="fPreAlpha">是否预乘透明通道</param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfromhbitmap(HBITMAP hBitmap, HPALETTE hPalette, BOOL fPreAlpha,
                            HEXIMAGE* phImg);

/// <summary>
/// 图像创建自图标句柄
/// </summary>
/// <param name="hIcon"></param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfromhicon(HICON hIcon, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自内存
/// </summary>
/// <param name="lpData"></param>
/// <param name="dwLen"></param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfrommemory(LPVOID lpData, size_t dwLen, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自缓冲区
/// </summary>
/// <param
/// name="lpmem">缓冲区,前4字节是width,4-8字节是height,8字节之后是数据</param>
/// <param name="dstImg">输出图像句柄</param>
/// <returns></returns>
BOOL _img_createfrompngbits(LPVOID lpmem, HEXIMAGE* dstImg);

/// <summary>
/// 图像创建自缓冲区2
/// </summary>
/// <param name="nWidth">图像宽度</param>
/// <param name="nHeight">图像高度</param>
/// <param name="pbBuffer">数据</param>
/// <param name="dstImg">输出图像句柄</param>
/// <returns></returns>
BOOL _img_createfrompngbits2(INT nWidth, INT nHeight, BYTE* pbBuffer,
                             HEXIMAGE* dstImg);

/// <summary>
/// 图像创建自资源包
/// </summary>
/// <param name="hRes"></param>
/// <param name="atomPath">路径</param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfromres(HEXRES hRes, EXATOM atomPath, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自字节流
/// </summary>
/// <param name="lpStream"></param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_createfromstream(LPSTREAM lpStream, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自SVG句柄
/// </summary>
/// <param name="hSvg">SVG句柄</param>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <param name="phImg">返回图像</param>
/// <returns></returns>
BOOL _img_createfromsvg(HEXSVG hSvg, INT width, INT height, HEXIMAGE* phImg);

/// <summary>
/// 图像创建自SVG文件
/// </summary>
/// <param name="lpwzFilename">SVG文件</param>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <param name="phImg">返回图像</param>
/// <returns></returns>
BOOL _img_createfromsvgfile(LPCWSTR lpwzFilename, INT width, INT height, HEXIMAGE* phImg);

/// <summary>
/// 图像销毁
/// </summary>
/// <param name="hImg"></param>
/// <returns></returns>
BOOL _img_destroy(HEXIMAGE hImg);

/// <summary>
/// 图像取帧数
/// </summary>
/// <param name="hImage"></param>
/// <param name="nFrameCount">返回帧数</param>
/// <returns></returns>
BOOL _img_getframecount(HEXIMAGE hImage, INT* nFrameCount);

/// <summary>
/// 图像取帧延时
/// </summary>
/// <param name="hImg"></param>
/// <param name="lpDelayAry">指针指向图像帧延时数组</param>
/// <param name="nFrames">返回总帧数</param>
/// <returns></returns>
BOOL _img_getframedelay(HEXIMAGE hImg, INT* lpDelayAry, INT nFrames);

/// <summary>
/// 图像获取点像素
/// </summary>
/// <param name="hImg"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="retPixel">返回ARGB颜色</param>
/// <returns></returns>
BOOL _img_getpixel(HEXIMAGE hImg, INT x, INT y, EXARGB* retPixel);

/// <summary>
/// 图像获取尺寸
/// </summary>
/// <param name="hImg"></param>
/// <param name="lpWidth"></param>
/// <param name="lpHeight"></param>
/// <returns></returns>
BOOL _img_getsize(HEXIMAGE hImg, INT* lpWidth, INT* lpHeight);

/// <summary>
/// 图像取高度
/// </summary>
/// <param name="hImg"></param>
/// <returns></returns>
INT _img_height(HEXIMAGE hImg);

/// <summary>
/// 锁定图像
/// </summary>
/// <param name="hImg"></param>
/// <param name="lpRectL">锁定矩形指针，如果为0，则锁定整张图像</param>
/// <param name="flags">READ:1 / WRITE:2 / READ&WRITE:3</param>
/// <param
/// name="PixelFormat">参考:https://msdn.microsoft.com/en-us/library/ms534412(v=vs.85).aspx</param>
/// <param name="lpLockedBitmapData">BITMAPDATA</param>
/// <returns></returns>
BOOL _img_lock(HEXIMAGE hImg, RECT* lpRectL, DWORD flags, INT PixelFormat,
               EX_BITMAPDATA* lpLockedBitmapData);

/// <summary>
/// 图像蒙板
/// </summary>
/// <param name="hImgDst">目标图句柄</param>
/// <param name="hImgSrc">原图句柄</param>
/// <param name="nChannel">蒙板通道：0:Alpha,1:R,2:G,3:B</param>
/// <param name="bBlackMask">蒙版通道是否为 黑色/透明</param>
/// <param name="phImg">返回图像</param>
/// <returns></returns>
BOOL _img_mask(HEXIMAGE hImgDst, HEXIMAGE hImgSrc, DWORD nChannel,
               BOOL bBlackMask, HEXIMAGE* phImg);

/// <summary>
/// 图形粘贴,将小图粘贴到大图并输出新图像
/// </summary>
/// <param name="dstImg">大图</param>
/// <param name="srcImg">小图</param>
/// <param name="destX">粘贴到大图横坐标</param>
/// <param name="destY">粘贴到大图纵坐标</param>
/// <param name="phImg">输出图像</param>
/// <returns></returns>
BOOL _img_paste(HEXIMAGE dstImg, HEXIMAGE srcImg, INT destX, INT destY,
                HEXIMAGE* phImg);

/// <summary>
/// 图像旋转翻转
/// </summary>
/// <param name="hImg"></param>
/// <param
/// name="rfType">参考:https://msdn.microsoft.com/en-us/library/windows/desktop/ms534171(v=vs.85).aspx</param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_rotateflip(HEXIMAGE hImg, INT rfType, HEXIMAGE* phImg);

/// <summary>
/// 图像保存到文件
/// </summary>
/// <param name="hImg"></param>
/// <param name="wzFileName">保存的文件名</param>
/// <returns></returns>
BOOL _img_savetofile(HEXIMAGE hImg, LPCWSTR wzFileName);

/// <summary>
/// 图像保存到内存缓冲区
/// </summary>
/// <param name="hImg"></param>
/// <param name="lpBuffer">填充保存的缓冲区(0则只是返回长度)</param>
/// <returns>返回长度</returns>
size_t _img_savetomemory(HEXIMAGE hImg, LPVOID lpBuffer);

/// <summary>
/// 图像缩放
/// </summary>
/// <param name="hImg"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="phImg"></param>
/// <returns></returns>
BOOL _img_scale(HEXIMAGE hImg, INT width, INT height, HEXIMAGE* phImg);

/// <summary>
/// 图像设置当前活动帧
/// </summary>
/// <param name="hImg"></param>
/// <param name="nIndex"></param>
/// <returns></returns>
BOOL _img_selectactiveframe(HEXIMAGE hImg, INT nIndex);

/// <summary>
/// 图像设置点像素
/// </summary>
/// <param name="hImg"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="color">Argb颜色</param>
/// <returns></returns>
BOOL _img_setpixel(HEXIMAGE hImg, INT x, INT y, EXARGB color);

/// <summary>
/// 图像解锁
/// </summary>
/// <param name="hImg"></param>
/// <param name="lpLockedBitmapData"></param>
/// <returns></returns>
BOOL _img_unlock(HEXIMAGE hImg, EX_BITMAPDATA* lpLockedBitmapData);

/// <summary>
/// 图像取宽度
/// </summary>
/// <param name="hImg"></param>
/// <returns>返回宽度</returns>
INT _img_width(HEXIMAGE hImg);

/// <summary>
/// 图片组添加图片从数据指针
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex">插入位置(0为最后)</param>
/// <param name="pImg"></param>
/// <param name="dwBytes"></param>
/// <returns></returns>
size_t _imglist_add(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg,
                    size_t dwBytes);

/// <summary>
/// 图片组添加图片从图片句柄
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex">插入位置(0为最后)</param>
/// <param name="hImg"></param>
/// <returns></returns>
size_t _imglist_addimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg);

/// <summary>
/// 图片组创建
/// </summary>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <returns></returns>
HEXIMAGELIST _imglist_create(INT width, INT height);

/// <summary>
/// 从文件创建图片组
/// </summary>
/// <param name="lpwzFileName">文件名</param>
/// <returns></returns>
HEXIMAGELIST _imglist_createfromfile(LPCWSTR lpwzFileName);

/// <summary>
/// 图片组获取图片数量
/// </summary>
/// <param name="hImageList"></param>
/// <returns>返回图片组图片数量</returns>
INT _imglist_count(HEXIMAGELIST hImageList);

/// <summary>
/// 图片组删除图片
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex">图片索引</param>
/// <returns></returns>
BOOL _imglist_del(HEXIMAGELIST hImageList, size_t nIndex);

/// <summary>
/// 图片组销毁
/// </summary>
/// <param name="hImageList"></param>
/// <returns></returns>
BOOL _imglist_destroy(HEXIMAGELIST hImageList);

/// <summary>
/// 图片组绘制图片
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex"></param>
/// <param name="hCanvas"></param>
/// <param name="nLeft"></param>
/// <param name="nTop"></param>
/// <param name="nRight"></param>
/// <param name="nBottom"></param>
/// <param name="nAlpha">透明度0-255</param>
/// <returns></returns>
BOOL _imglist_draw(HEXIMAGELIST hImageList, size_t nIndex, HEXCANVAS hCanvas,
                   INT nLeft, INT nTop, INT nRight, INT nBottom, DWORD nAlpha);

/// <summary>
/// 图片组获取图片句柄
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex"></param>
/// <returns></returns>
HEXIMAGE _imglist_get(HEXIMAGELIST hImageList, size_t nIndex);

/// <summary>
/// 保存图片组到文件
/// </summary>
/// <param name="hImageList"></param>
/// <param name="lpwzFileName">文件名</param>
/// <returns></returns>
BOOL _imglist_savetofile(HEXIMAGELIST hImageList, LPCWSTR lpwzFileName);

/// <summary>
/// 图片组设置图片从数据
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex"></param>
/// <param name="pImg">图片数据指针</param>
/// <param name="dwBytes">图片数据长度</param>
/// <returns></returns>
BOOL _imglist_set(HEXIMAGELIST hImageList, size_t nIndex, LPVOID pImg,
                  size_t dwBytes);

/// <summary>
/// 图片组设置图片从图像句柄
/// </summary>
/// <param name="hImageList"></param>
/// <param name="nIndex"></param>
/// <param name="hImg"></param>
/// <returns></returns>
BOOL _imglist_setimage(HEXIMAGELIST hImageList, size_t nIndex, HEXIMAGE hImg);

/// <summary>
/// 图片组获取图片尺寸
/// </summary>
/// <param name="hImageList"></param>
/// <param name="pWidth">返回图片宽度</param>
/// <param name="pHeight">返回图片高度</param>
/// <returns></returns>
BOOL _imglist_size(HEXIMAGELIST hImageList, INT* pWidth, INT* pHeight);

/// <summary>
/// 绝对布局按当前位置锁定
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObjChild"></param>
/// <param name="tLeft">#LAYOUT_SUBPROP_ABSOLUTE_XXX_TYPE 下同</param>
/// <param name="tTop"></param>
/// <param name="tRight"></param>
/// <param name="tBottom"></param>
/// <param name="tWidth"></param>
/// <param name="tHeight"></param>
/// <returns></returns>
BOOL _layout_absolute_lock(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT tLeft,
                           INT tTop, INT tRight, INT tBottom, INT tWidth,
                           INT tHeight);

/// <summary>
/// 绝对布局置边界信息
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObjChild"></param>
/// <param name="dwEdge">#LAYOUT_SUBPROP_ABSOLUTE_XXX</param>
/// <param name="dwType">#LAYOUT_SUBPROP_ABSOLUTE_TYPE_</param>
/// <param name="nValue"></param>
/// <returns></returns>
BOOL _layout_absolute_setedge(HEXLAYOUT hLayout, HEXOBJ hObjChild, INT dwEdge,
                              INT dwType, size_t nValue);

/// <summary>
/// 布局添加组件
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL _layout_addchild(HEXLAYOUT hLayout, HEXOBJ hObj);

/// <summary>
/// 布局加入所有组件,已被加入的不会重复添加(系统按钮不加入)
/// </summary>
/// <param name="hLayout"></param>
/// <param name="fDesc">是否倒序</param>
/// <param name="dwObjClassATOM">0或空为所有</param>
/// <param name="nCount">加入的个数</param>
/// <returns></returns>
BOOL _layout_addchildren(HEXLAYOUT hLayout, BOOL fDesc, EXATOM dwObjClassATOM,
                         INT* nCount);

/// <summary>
/// 布局创建
/// </summary>
/// <param name="nType">#LAYOUT_TYPE_ 布局类型</param>
/// <param name="hObjBind">所绑定的HOBJ或HEXDUI</param>
/// <returns></returns>
HEXLAYOUT _layout_create(INT nType, EXHANDLE hObjBind);

/// <summary>
/// 布局删除组件
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL _layout_deletechild(HEXLAYOUT hLayout, HEXOBJ hObj);

/// <summary>
/// 布局删除所有组件
/// </summary>
/// <param name="hLayout"></param>
/// <param name="dwObjClassATOM"> 0或空为所有</param>
/// <returns></returns>
BOOL _layout_deletechildren(HEXLAYOUT hLayout, EXATOM dwObjClassATOM);

/// <summary>
/// 布局销毁
/// </summary>
/// <param name="hLayout"></param>
/// <returns></returns>
BOOL _layout_destroy(HEXLAYOUT hLayout);

/// <summary>
/// 布局置是否允许更新
/// </summary>
/// <param name="hLayout"></param>
/// <param name="fUpdateable"></param>
/// <returns></returns>
BOOL _layout_enableupdate(HEXLAYOUT hLayout, BOOL fUpdateable);

/// <summary>
/// 布局取子属性
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObj"></param>
/// <param name="dwPropID"></param>
/// <param name="pvValue"></param>
/// <returns></returns>
BOOL _layout_getchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID,
                          size_t* pvValue);

/// <summary>
/// 布局取子属性列表
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObj"></param>
/// <param name="lpProps">不释放</param>
/// <returns></returns>
BOOL _layout_getchildproplist(HEXLAYOUT hLayout, HEXOBJ hObj, LPVOID* lpProps);

/// <summary>
/// 布局取属性
/// </summary>
/// <param name="hLayout"></param>
/// <param name="dwPropID"></param>
/// <returns></returns>
size_t _layout_getprop(HEXLAYOUT hLayout, INT dwPropID);

/// <summary>
/// 布局取属性列表
/// </summary>
/// <param name="hLayout"></param>
/// <returns></returns>
LPVOID _layout_getproplist(HEXLAYOUT hLayout);

/// <summary>
/// 布局取类型
/// </summary>
/// <param name="hLayout"></param>
/// <returns></returns>
INT _layout_gettype(HEXLAYOUT hLayout);

/// <summary>
/// 布局分发通知
/// </summary>
/// <param name="hLayout"></param>
/// <param name="nEvent"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT _layout_notify(HEXLAYOUT hLayout, INT nEvent, WPARAM wParam,
                       LPARAM lParam);

/// <summary>
/// 布局置子属性
/// </summary>
/// <param name="hLayout"></param>
/// <param name="hObj"></param>
/// <param name="dwPropID"></param>
/// <param name="pvValue"></param>
/// <returns></returns>
BOOL _layout_setchildprop(HEXLAYOUT hLayout, HEXOBJ hObj, INT dwPropID,
                          size_t pvValue);

/// <summary>
/// 布局置属性
/// </summary>
/// <param name="hLayout"></param>
/// <param name="dwPropID"></param>
/// <param name="pvValue"></param>
/// <returns></returns>
BOOL _layout_setprop(HEXLAYOUT hLayout, INT dwPropID, size_t pvValue);

/// <summary>
/// 布局置表格信息
/// </summary>
/// <param name="hLayout"></param>
/// <param name="aRowHeight"></param>
/// <param name="cRows"></param>
/// <param name="aCellWidth"></param>
/// <param name="cCells"></param>
/// <returns></returns>
BOOL _layout_table_setinfo(HEXLAYOUT hLayout, INT* aRowHeight, INT cRows,
                           INT* aCellWidth, INT cCells);

/// <summary>
/// 布局更新
/// </summary>
/// <param name="hLayout"></param>
/// <returns></returns>
BOOL _layout_update(HEXLAYOUT hLayout);

/// <summary>
/// 矩阵创建
/// </summary>
/// <returns></returns>
HEXMATRIX _matrix_create();

/// <summary>
/// 矩阵销毁
/// </summary>
/// <param name="pMatrix"></param>
/// <returns></returns>
BOOL _matrix_destroy(HEXMATRIX pMatrix);

/// <summary>
/// 矩阵重置
/// </summary>
/// <param name="pMatrix"></param>
/// <returns></returns>
BOOL _matrix_reset(HEXMATRIX pMatrix);

/// <summary>
/// 矩阵旋转
/// </summary>
/// <param name="pMatrix"></param>
/// <param name="fAngle"></param>
/// <returns></returns>
BOOL _matrix_rotate(HEXMATRIX pMatrix, FLOAT fAngle);

/// <summary>
/// 矩阵缩放
/// </summary>
/// <param name="pMatrix"></param>
/// <param name="scaleX"></param>
/// <param name="scaleY"></param>
/// <returns></returns>
BOOL _matrix_scale(HEXMATRIX pMatrix, FLOAT scaleX, FLOAT scaleY);

/// <summary>
/// 矩阵转置
/// </summary>
/// <param name="pMatrix"></param>
/// <param name="offsetX"></param>
/// <param name="offsetY"></param>
/// <returns></returns>
BOOL _matrix_translate(HEXMATRIX pMatrix, FLOAT offsetX, FLOAT offsetY);

/// <summary>
/// 路径添加弧
/// </summary>
/// <param name="hPath"></param>
/// <param name="x1">起始坐标X</param>
/// <param name="y1">起始坐标Y</param>
/// <param name="x2">终点坐标X</param>
/// <param name="y2">终点坐标Y</param>
/// <param name="radiusX">弧中心坐标X</param>
/// <param name="radiusY">弧中心坐标Y</param>
/// <param name="fClockwise">是否顺时针</param>
/// <returns></returns>
BOOL _path_addarc(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
                  FLOAT radiusX, FLOAT radiusY, BOOL fClockwise);

/// <summary>
/// 路径添加弧 v2
/// </summary>
/// <param name="hPath">路径句柄</param>
/// <param name="left">弧所在椭圆的左边</param>
/// <param name="top">弧所在椭圆的顶边</param>
/// <param name="right">弧所在椭圆的右边</param>
/// <param name="bottom">弧所在椭圆的底边</param>
/// <param name="nAngleBegin">弧的起始角度</param>
/// <param name="nAngleEnd">弧的终止角度</param>
/// <returns></returns>
BOOL _path_addarc2(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right,
                   FLOAT bottom, FLOAT nAngleBegin, FLOAT nAngleEnd);

/// <summary>
/// 路径添加弧 v3
/// </summary>
/// <param name="hPath"></param>
/// <param name="x">终点坐标X</param>
/// <param name="y">终点坐标Y</param>
/// <param name="radiusX">弧中心坐标X</param>
/// <param name="radiusY">弧中心坐标Y</param>
/// <param name="startAngle">起始弧度</param>
/// <param name="sweepAngle">扫描弧度</param>
/// <param name="fClockwise">是否顺时针</param>
/// <param name="barcSize">是否大于180°</param>
/// <returns></returns>
BOOL _path_addarc3(HEXPATH hPath, FLOAT x, FLOAT y, FLOAT radiusX,
                   FLOAT radiusY, FLOAT startAngle, FLOAT sweepAngle,
                   BOOL fClockwise, BOOL barcSize);

/// <summary>
/// 路径添加贝塞尔曲线
/// </summary>
/// <param name="hPath"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="x2"></param>
/// <param name="y2"></param>
/// <param name="x3"></param>
/// <param name="y3"></param>
/// <returns></returns>
BOOL _path_addbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2,
                     FLOAT x3, FLOAT y3);

/// <summary>
/// 路径添加椭圆, 起始点坐标(left, (top + bottom) / 2)
/// </summary>
/// <param name="hPath"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
BOOL _path_addellipse(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right,
                      FLOAT bottom);

/// <summary>
/// 路径添加直线
/// </summary>
/// <param name="hPath"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="x2"></param>
/// <param name="y2"></param>
/// <returns></returns>
BOOL _path_addline(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2, FLOAT y2);

/// <summary>
/// 路径添加二次方贝塞尔曲线
/// </summary>
/// <param name="hPath"></param>
/// <param name="x1"></param>
/// <param name="y1"></param>
/// <param name="x2"></param>
/// <param name="y2"></param>
/// <returns></returns>
BOOL _path_addquadraticbezier(HEXPATH hPath, FLOAT x1, FLOAT y1, FLOAT x2,
                              FLOAT y2);

/// <summary>
/// 路径添加矩形
/// </summary>
/// <param name="hPath"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
BOOL _path_addrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right,
                   FLOAT bottom);

/// <summary>
/// 路径添加圆角矩形
/// </summary>
/// <param name="hPath"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="radiusTopLeft"></param>
/// <param name="radiusTopRight"></param>
/// <param name="radiusBottomLeft"></param>
/// <param name="radiusBottomRight"></param>
/// <returns></returns>
BOOL _path_addroundedrect(HEXPATH hPath, FLOAT left, FLOAT top, FLOAT right,
                          FLOAT bottom, FLOAT radiusTopLeft,
                          FLOAT radiusTopRight, FLOAT radiusBottomLeft,
                          FLOAT radiusBottomRight);

/// <summary>
/// 路径开始新图形
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_beginfigure(HEXPATH hPath);

/// <summary>
/// 路径开始新图形2,可设置起始点
/// </summary>
/// <param name="hPath"></param>
/// <param name="x">起始坐标X</param>
/// <param name="y">起始坐标Y</param>
/// <returns></returns>
BOOL _path_beginfigure2(HEXPATH hPath, FLOAT x, FLOAT y);

/// <summary>
/// 路径开始新图形3,可设置起始点和类型
/// </summary>
/// <param name="hPath"></param>
/// <param name="x">起始坐标X</param>
/// <param name="y">起始坐标Y</param>
/// <param name="figureBegin">常量PATH_BEGIN_FLAG_</param>
/// <returns></returns>
BOOL _path_beginfigure3(HEXPATH hPath, FLOAT x, FLOAT y, INT figureBegin);

/// <summary>
/// 路径关闭
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_close(HEXPATH hPath);

/// <summary>
/// 路径创建
/// </summary>
/// <param name="dwFlags">PATH_FLAG_</param>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_create(DWORD dwFlags, HEXPATH* hPath);

/// <summary>
/// 路径销毁
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_destroy(HEXPATH hPath);

/// <summary>
/// 路径结束当前图形
/// </summary>
/// <param name="hPath"></param>
/// <param name="fCloseFigure">是否需要闭合图形</param>
/// <returns></returns>
BOOL _path_endfigure(HEXPATH hPath, BOOL fCloseFigure);

/// <summary>
/// 路径取边界矩形
/// </summary>
/// <param name="hPath"></param>
/// <param name="lpBounds"></param>
/// <returns></returns>
BOOL _path_getbounds(HEXPATH hPath, EX_RECTF* lpBounds);

/// <summary>
/// 路径测试坐标是否在可见路径内
/// </summary>
/// <param name="hPath"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
BOOL _path_hittest(HEXPATH hPath, FLOAT x, FLOAT y);

/// <summary>
/// 路径打开
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_open(HEXPATH hPath);

/// <summary>
/// 路径重置
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
BOOL _path_reset(HEXPATH hPath);

/// <summary>
/// 打包主题包
/// </summary>
/// <param name="root">主题包文件夹，绝对路径</param>
/// <param name="file">打包后主题包文件保存路径如test_theme.ext</param>
/// <param
/// name="byteHeader">打包类型，可选PACKAGEHEADER_THEME主题包或者PACKAGEHEADER_FILES资源包。资源包可用Ex_ResLoadFromMemory加载，并用Ex_ResGetFile读取资源或者_img_createfromres创建图像</param>
void _res_pack(LPCWSTR root, LPCWSTR file, UCHAR byteHeader);

/// <summary>
/// 区域合并,注意路径必须创建自PATH_BEGIN_FLAG_FILLED才有效果
/// </summary>
/// <param name="hRgnSrc"></param>
/// <param name="hRgnDst"></param>
/// <param name="nCombineMode">#REGION_COMBINE_</param>
/// <param name="dstOffsetX"></param>
/// <param name="dstOffsetY"></param>
/// <returns></returns>
HEXRGN _rgn_combine(HEXRGN hRgnSrc, HEXRGN hRgnDst, INT nCombineMode,
                    INT dstOffsetX, INT dstOffsetY);

/// <summary>
/// 区域创建自路径
/// </summary>
/// <param name="hPath"></param>
/// <returns></returns>
HEXRGN _rgn_createfrompath(HEXPATH hPath);

/// <summary>
/// 区域创建自椭圆
/// </summary>
/// <param name="x">圆心横坐标</param>
/// <param name="y">圆心纵坐标</param>
/// <param name="radiusX">横向半径</param>
/// <param name="radiusY">纵向半径</param>
/// <returns></returns>
HEXRGN _rgn_createfromellipse(FLOAT x, FLOAT y, FLOAT radiusX, FLOAT radiusY);

/// <summary>
/// 区域创建自矩形
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <returns></returns>
HEXRGN _rgn_createfromrect(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom);

/// <summary>
/// 区域创建自圆角矩形
/// </summary>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="radiusX"></param>
/// <param name="radiusY"></param>
/// <returns></returns>
HEXRGN _rgn_createfromroundrect(FLOAT left, FLOAT top, FLOAT right,
                                FLOAT bottom, FLOAT radiusX, FLOAT radiusY);

/// <summary>
/// 区域销毁
/// </summary>
/// <param name="hRgn"></param>
/// <returns></returns>
BOOL _rgn_destroy(HEXRGN hRgn);

/// <summary>
/// 区域取外接矩形
/// </summary>
/// <param name="hRgn"></param>
/// <param name="lpBounds"></param>
/// <returns></returns>
BOOL _rgn_getbounds(HEXRGN hRgn, EX_RECTF* lpBounds);

/// <summary>
/// 区域转为线段并取出线段所有点
/// </summary>
/// <param name="hRgn"></param>
/// <param name="points">返回所有点数组</param>
/// <param name="pointsCount">返回点个数</param>
/// <returns></returns>
BOOL _rgn_getlines(HEXRGN hRgn, EX_POINTF** points, INT* pointsCount);

/// <summary>
/// 区域命中测试
/// </summary>
/// <param name="hRgn"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
BOOL _rgn_hittest(HEXRGN hRgn, FLOAT x, FLOAT y);

/// <summary>
/// 区域命中测试2
/// </summary>
/// <param name="hRgn1">区域1</param>
/// <param name="hRgn2">区域1</param>
/// <param name="retRelation">返回关系 0无法确定 1不相交 2属于(被包含) 3包含
/// 4相交</param> <returns></returns>
BOOL _rgn_hittest2(HEXRGN hRgn1, HEXRGN hRgn2, INT* retRelation);

/// <summary>
/// 着色器加载,注意加载成功会在程序运行目录生成编译后的result.cso文件方便下次调用,覆盖。
/// </summary>
/// <param name="pEffectContext">效果器上下文ID2D1EffectContext*</param>
/// <param name="pHlsl">着色器代码指针</param>
/// <param name="pHlslLen">着色器代码指针长度</param>
/// <param name="shaderID">着色器GUID</param>
/// <returns></returns>
BOOL _shader_load(LPVOID pEffectContext, LPCSTR pHlsl, int pHlslLen,
                  const GUID& shaderID);

/// <summary>
/// 着色器加载自文件
/// </summary>
/// <param name="pEffectContext">效果器上下文ID2D1EffectContext*</param>
/// <param name="csoFileNameInOut">已编译的着色器cso文件</param>
/// <param name="shaderID">着色器GUID</param>
/// <returns></returns>
BOOL _shader_loadfromfile(LPVOID pEffectContext, LPCWSTR csoFileNameInOut,
                          const GUID& shaderID);

/// <summary>
/// SVG创建(自内存)
/// </summary>
/// <param name="hExDuiOrhObj">引擎句柄或组件句柄</param>
/// <param name="svgdata">SVG描述内容</param>
/// <param name="phSvg">返回svg句柄</param>
/// <returns></returns>
BOOL _svg_create(const char* svgdata, HEXSVG* phSvg);

/// <summary>
/// SVG创建自文件
/// </summary>
/// <param name="hExDuiOrhObj">引擎句柄或组件句柄</param>
/// <param name="svgfile">SVG文件路径</param>
/// <param name="phSvg">返回svg句柄</param>
/// <returns></returns>
BOOL _svg_createfromfile(const wchar_t* svgfile, HEXSVG* phSvg);

/// <summary>
/// SVG销毁
/// </summary>
/// <param name="hSvg">svg句柄</param>
/// <returns></returns>
BOOL _svg_destroy(HEXSVG hSvg);

/// <summary>
/// SVG置元素填充色
/// </summary>
/// <param name="hSvg"></param>
/// <param name="id"></param>
/// <param name="color"></param>
void _svg_setelementfillcolor(HEXSVG hSvg, const char* id, EXARGB color);

/// <summary>
/// SVG置元素描边色
/// </summary>
/// <param name="hSvg"></param>
/// <param name="id"></param>
/// <param name="color"></param>
/// <param name="strokeWidth"></param>
void _svg_setelementstrokecolor(HEXSVG hSvg, const char* id, EXARGB color, float strokeWidth = 0.0f);

/// <summary>
/// SVG置填充色
/// </summary>
/// <param name="hSvg"></param>
/// <param name="color"></param>
void _svg_setfillcolor(HEXSVG hSvg, EXARGB color);

/// <summary>
/// SVG置描边色
/// </summary>
/// <param name="hSvg"></param>
/// <param name="color"></param>
/// <param name="width"></param>
void _svg_setstrokecolor(HEXSVG hSvg, EXARGB color, FLOAT width = 0);

/// <summary>
/// ansi to unicode
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring Ex_A2W(const std::string& str);

/// <summary>
/// ansi to unicode
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring Ex_A2W2(std::vector<UCHAR> str);

/// <summary>
/// ansi to utf8
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string Ex_A2U(const std::string& str);

/// <summary>
/// utf8 to ansi
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string Ex_U2A(const std::string& str);

/// <summary>
/// utf to ansi
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::string Ex_U2A2(std::vector<CHAR> str);

/// <summary>
/// unicode to utf8
/// </summary>
/// <param name="wstr"></param>
/// <returns></returns>
std::string Ex_W2U(const std::wstring& wstr);

/// <summary>
/// unicode to utf8
/// </summary>
/// <param name="wstr"></param>
/// <returns></returns>
std::vector<UCHAR> Ex_W2U2(const std::wstring& wstr);

/// <summary>
/// utf8 to unicode
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring Ex_U2W(const std::string& str);

/// <summary>
/// utf8 to unicode
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring Ex_U2W3(std::vector<UCHAR> str);

/// <summary>
/// utf8 to unicode
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
std::wstring Ex_U2W2(std::vector<CHAR> str);

/// <summary>
/// 申请内存
/// </summary>
/// <param name="dwSize"></param>
/// <returns></returns>
LPVOID Ex_AllocBuffer(size_t dwSize);

/// <summary>
/// 获取字符串唯一原子号
/// </summary>
/// <param name="lptstring"></param>
/// <returns></returns>
EXATOM Ex_Atom(LPCWSTR lptstring);

/// <summary>
/// 初始化CEF浏览框路径,创建前使用
/// </summary>
/// <param name="hModule">模块句柄</param>
/// <param
/// name="libPath">库文件夹路径,绝对路径,不为0时会写入环境变量PATH。默认程序运行路径</param>
/// <param name="dllName">库文件名称 默认为FTBrowser.dll</param>
/// <param name="cachePath">临时文件保存路径,绝对路径,0为内存模式</param>
/// <param name="userAgent">用户设备</param>
/// <param name="debuggingPort">调试端口</param>
/// <param name="lpBeforeCommandLine">可为0,初始化传入的命令行参数void(CALLBACK*
/// OnBeforeCommandLine)(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR
/// attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool*
/// pbHWEBVIEWd, void* lParam); </param> <returns></returns>
BOOL Ex_ObjCefBrowserInitialize(HMODULE hModule, LPCWSTR libPath,
                                LPCWSTR dllName, LPCWSTR cachePath,
                                LPCWSTR userAgent, INT debuggingPort,
                                void* lpBeforeCommandLine);

/// <summary>
/// 绑定窗口
/// </summary>
/// <param name="hWnd"></param>
/// <param name="hTheme"></param>
/// <param name="dwStyle">#WINDOW_STYLE_</param>
/// <returns></returns>
HEXDUI Ex_DUIBindWindow(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle);

/// <summary>
/// 绑定窗口ex
/// </summary>
/// <param name="hWnd"></param>
/// <param name="hTheme">主题句柄.(值可为0)</param>
/// <param name="dwStyle">#WINDOW_STYLE_</param>
/// <param name="lParam">附加参数.(值可为0)</param>
/// <param name="lpfnMsgProc">(LRESULT)MsgProc(HWND hWnd,HEXDUI	hExDui,INT
/// uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult)</param>
/// <returns></returns>
HEXDUI Ex_DUIBindWindowEx(HWND hWnd, HEXTHEME hTheme, DWORD dwStyle,
                          LPARAM lParam, WinMsgPROC lpfnMsgProc);

/// <summary>
/// 从窗口句柄获取引擎句柄
/// </summary>
/// <param name="hWnd"></param>
/// <returns></returns>
HEXDUI Ex_DUIFromWindow(HWND hWnd);

/// <summary>
/// 获取窗口大小,启用ENGINE_FLAG_DPI_ENABLE风格则获取缩放dpi后的窗口大小
/// </summary>
/// <param name="hExDui"></param>
/// <param name="lpClientRect"></param>
/// <returns></returns>
BOOL Ex_DUIGetClientRect(HEXDUI hExDui, RECT* lpClientRect);

/// <summary>
/// 获取引擎数值
/// </summary>
/// <param name="hExDui"></param>
/// <param name="nIndex">#ENGINE_LONG_</param>
/// <returns></returns>
size_t Ex_DUIGetLong(HEXDUI hExDui, INT nIndex);

/// <summary>
/// 获取鼠标所在窗口组件句柄
/// </summary>
/// <param name="handle">0[坐标所在窗口]/窗口句柄/引擎句柄/组件句柄</param>
/// <param name="x">handle:0相对屏幕/其他相对窗口</param>
/// <param name="y"></param>
/// <returns></returns>
HEXOBJ Ex_DUIGetObjFromPoint(EXHANDLE handle, INT x, INT y);

/// <summary>
/// 取系统dpi缩放系数
/// </summary>
/// <returns></returns>
DOUBLE Ex_DUIGetSystemDpi();

/// <summary>
/// 设置引擎数值
/// </summary>
/// <param name="hExDui"></param>
/// <param name="nIndex">#ENGINE_LONG_</param>
/// <param name="dwNewLong"></param>
/// <returns></returns>
size_t Ex_DUISetLong(HEXDUI hExDui, INT nIndex, size_t dwNewLong);

/// <summary>
/// 显示窗口
/// </summary>
/// <param name="hExDui"></param>
/// <param name="nCmdShow">相关常量:#SW_</param>
/// <param name="dwTimer">动画时间间隔(ms)</param>
/// <param name="dwFrames">动画总帧数</param>
/// <param name="dwFlags">动画标记</param>
/// <returns></returns>
BOOL Ex_DUIShowWindow(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames,
                      INT dwFlags);

/// <summary>
/// 显示窗口Ex
/// </summary>
/// <param name="hExDui"></param>
/// <param name="nCmdShow">相关常量:#SW_</param>
/// <param name="dwTimer">动画时间间隔</param>
/// <param name="dwFrames">动画总帧数</param>
/// <param name="dwFlags">动画标记.#EXA_</param>
/// <param name="uEasing">缓动类型常量#EASING_TYPE_</param>
/// <param name="wParam">参数1</param>
/// <param name="lParam">参数2</param>
/// <returns></returns>
BOOL Ex_DUIShowWindowEx(HEXDUI hExDui, INT nCmdShow, INT dwTimer, INT dwFrames,
                        INT dwFlags, INT uEasing, WPARAM wParam, LPARAM lParam);

/// <summary>
/// 弹出托盘图标
/// </summary>
/// <param name="hExDui"></param>
/// <param name="lpwzInfo">弹出文本内容</param>
/// <param name="lpwzInfoTitle">弹出标题文本</param>
/// <param name="dwInfoFlags">#NIIF_</param>
/// <returns></returns>
BOOL Ex_DUITrayIconPopup(HEXDUI hExDui, LPCWSTR lpwzInfo, LPCWSTR lpwzInfoTitle,
                         DWORD dwInfoFlags);

/// <summary>
/// 设置托盘图标
/// </summary>
/// <param name="hExDui"></param>
/// <param name="hIcon">图标句柄</param>
/// <param name="lpwzTips">提示文本</param>
/// <returns></returns>
BOOL Ex_DUITrayIconSet(HEXDUI hExDui, HICON hIcon, LPCWSTR lpwzTips);

/// <summary>
/// 释放内存
/// </summary>
/// <param name="lpBuffer"></param>
/// <returns></returns>
BOOL Ex_FreeBuffer(LPVOID lpBuffer);

/// <summary>
/// 获取最后错误代码.相关常量 :#ERROR_EX_
/// </summary>
/// <returns></returns>
INT Ex_GetLastError();

/// <summary>
/// 初始化引擎
/// </summary>
/// <param name="hInstance">值可为0</param>
/// <param name="dwGlobalFlags">相关常量:#ENGINE_FLAG_ .(值可为0)</param>
/// <param name="hDefaultCursor">默认鼠标指针.(值可为0)</param>
/// <param name="lpszDefaultClassName">默认窗口类名.(值可为0)</param>
/// <param name="lpDefaultTheme">默认主题包指针</param>
/// <param name="dwDefaultThemeLen">默认主题包长度</param>
/// <param name="lpDefaultI18N">默认语言包指针.(值可为0)</param>
/// <param name="dwDefaultI18NLen">默认语言包长度.(值可为0)</param>
/// <returns></returns>
BOOL Ex_Init(HINSTANCE hInstance, DWORD dwGlobalFlags, HCURSOR hDefaultCursor,
             LPCWSTR lpszDefaultClassName, LPVOID lpDefaultTheme,
             size_t dwDefaultThemeLen, LPVOID lpDefaultI18N,
             size_t dwDefaultI18NLen);

/// <summary>
/// 加载位图对象自内存
/// </summary>
/// <param name="lpData"></param>
/// <param name="dwLen"></param>
/// <param name="retBitMap">返回位图句柄</param>
/// <returns></returns>
BOOL Ex_LoadBitMapFromMemory(LPVOID lpData, size_t dwLen, HBITMAP* retBitMap);

/// <summary>
/// 加载图像对象自内存
/// </summary>
/// <param name="lpData"></param>
/// <param name="dwLen"></param>
/// <param name="uType">图像类型 IMAGE_</param>
/// <param name="nIndex">索引</param>
/// <returns></returns>
LPVOID Ex_LoadImageFromMemory(LPVOID lpData, size_t dwLen, INT uType,
                              INT nIndex);

/// <summary>
/// 弹出信息框
/// </summary>
/// <param
/// name="handle">组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认信息框弹出)</param>
/// <param name="lpText">消息框内容</param>
/// <param name="lpCaption">消息框标题</param>
/// <param name="uType">相关常量 #MB_</param>
/// <param name="dwFlags">相关常量 #MESSAGEBOX_FLAG_</param>
/// <returns></returns>
INT Ex_MessageBox(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType,
                  INT dwFlags);

/// <summary>
/// 弹出信息框Ex
/// </summary>
/// <param
/// name="handle">组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认信息框弹出)</param>
/// <param name="lpText">消息框内容</param>
/// <param name="lpCaption">消息框标题</param>
/// <param name="uType">相关常量 #MB_</param>
/// <param
/// name="lpCheckBox">检查框标题(消息框左下角显示一个检查框组件).(如果该窗口未使用引擎渲染,则该值无效)</param>
/// <param
/// name="lpCheckBoxChecked">返回检查框选中状态.(如果该窗口未使用引擎渲染,则该值无效)</param>
/// <param
/// name="dwMilliseconds">消息框延迟关闭时间，单位：毫秒。如果该值不为0,且超时后(即用户未操作)，消息框自动关闭，返回32000。</param>
/// <param name="dwFlags">相关常量 #MESSAGEBOX_FLAG_</param>
/// <param
/// name="lpfnMsgProc">(BOOL)MsgProc(hWnd,hExDui,uMsg,wParam,lParam,lpResult)</param>
/// <returns></returns>
INT Ex_MessageBoxEx(size_t handle, LPCWSTR lpText, LPCWSTR lpCaption, INT uType,
                    LPCWSTR lpCheckBox, BOOL* lpCheckBoxChecked,
                    INT dwMilliseconds, INT dwFlags, MsgPROC lpfnMsgProc);

/// <summary>
/// 申请内存
/// </summary>
/// <param name="dwSize">尺寸</param>
/// <param name="dwFlags">标识</param>
/// <returns></returns>
LPVOID Ex_MemAlloc(size_t dwSize, INT dwFlags = LMEM_ZEROINIT);

/// <summary>
/// 内存释放
/// </summary>
/// <param name="hMem">内存指针</param>
/// <returns></returns>
BOOL Ex_MemFree(LPVOID hMem);

/// <summary>
/// 重新申请内存
/// </summary>
/// <param name="hMem">内存指针</param>
/// <param name="dwSizes">新尺寸</param>
/// <returns>返回新指针</returns>
LPVOID Ex_MemReAlloc(LPVOID hMem, size_t dwSizes);

/// <summary>
/// 组件开始绘制
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpPS"></param>
/// <returns></returns>
BOOL Ex_ObjBeginPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS);

/// <summary>
/// 组件调用过程
/// </summary>
/// <param name="lpPrevObjProc"></param>
/// <param name="hWnd"></param>
/// <param name="hObj"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT Ex_ObjCallProc(ClsPROC lpPrevObjProc, HWND hWnd, HEXOBJ hObj, INT uMsg,
                       WPARAM wParam, LPARAM lParam);

/// <summary>
/// 组件查询拖曳信息格式
/// </summary>
/// <param name="hObj"></param>
/// <param name="pDataObject"></param>
/// <param name="dwFormat"></param>
/// <returns></returns>
BOOL Ex_ObjCheckDropFormat(HEXOBJ hObj, LPVOID pDataObject, DWORD dwFormat);

/// <summary>
/// 组件客户区坐标到屏幕坐标
/// </summary>
/// <param name="hObj"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
BOOL Ex_ObjClientToScreen(HEXOBJ hObj, INT* x, INT* y);

/// <summary>
/// 组件客户区坐标到窗口坐标
/// </summary>
/// <param name="hObj"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
BOOL Ex_ObjClientToWindow(HEXOBJ hObj, INT* x, INT* y);

/// <summary>
/// 组件创建
/// </summary>
/// <param name="lptszClassName">组件类名</param>
/// <param name="lptszObjTitle">组件标题</param>
/// <param name="dwStyle">组件风格 相关常量 OBJECT_STYLE_</param>
/// <param name="x">左边</param>
/// <param name="y">顶边</param>
/// <param name="width">宽度</param>
/// <param name="height">高度</param>
/// <param name="hParent">父组件句柄</param>
/// <returns></returns>
HEXOBJ Ex_ObjCreate(LPCWSTR lptszClassName, LPCWSTR lptszObjTitle, INT dwStyle,
                    INT x, INT y, INT width, INT height, EXHANDLE hParent);

/// <summary>
/// 组件创建Ex
/// </summary>
/// <param name="dwStyleEx">组件扩展风格 相关常量 OBJECT_STYLE_EX_</param>
/// <param name="lptszClassName">组件类名</param>
/// <param name="lptszObjTitle">组件标题</param>
/// <param name="dwStyle">组件风格 相关常量 OBJECT_STYLE_</param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="hParent">父组件句柄</param>
/// <param name="nID">组件ID</param>
/// <param name="dwTextFormat">相关常量 DT_</param>
/// <param name="lParam">附加参数</param>
/// <param name="hTheme">主题句柄.0为默认</param>
/// <param name="lpfnMsgProc">LRESULT CALLBACK MsgProc(HWND hWnd,HEXOBJ hObj,INT
/// uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult)</param>
/// <returns>返回组件句柄</returns>
HEXOBJ Ex_ObjCreateEx(INT dwStyleEx, LPCWSTR lptszClassName,
                      LPCWSTR lptszObjTitle, INT dwStyle, INT x, INT y,
                      INT width, INT height, EXHANDLE hParent, INT nID,
                      INT dwTextFormat, LPARAM lParam, HEXTHEME hTheme,
                      MsgPROC lpfnMsgProc);

/// <summary>
/// 组件默认过程
/// </summary>
/// <param name="hWnd"></param>
/// <param name="hObj"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
LRESULT Ex_ObjDefProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam,
                      LPARAM lParam);

/// <summary>
/// 组件销毁
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjDestroy(HEXOBJ hObj);

/// <summary>
/// 组件销毁背景
/// </summary>
/// <param name="handle">界面或组件句柄</param>
/// <returns></returns>
BOOL Ex_ObjDestroyBackground(EXHANDLE handle);

/// <summary>
/// 组件设置是否禁止转换空格和回车为单击事件
/// </summary>
/// <param name="hObj"></param>
/// <param name="fDisable"></param>
/// <returns></returns>
BOOL Ex_ObjDisableTranslateSpaceAndEnterToClick(HEXOBJ hObj, BOOL fDisable);

/// <summary>
/// 组件分发消息
/// </summary>
/// <param name="hObj"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
size_t Ex_ObjDispatchMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam,
                             LPARAM lParam);

/// <summary>
/// 组件分发事件
/// </summary>
/// <param name="hObj"></param>
/// <param name="nCode"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
size_t Ex_ObjDispatchNotify(HEXOBJ hObj, INT nCode, WPARAM wParam,
                            LPARAM lParam);

/// <summary>
/// 组件默认绘制背景过程
/// </summary>
/// <param name="hObj"></param>
/// <param name="hCanvas"></param>
/// <param name="lprcPaint"></param>
/// <returns></returns>
BOOL Ex_ObjDrawBackgroundProc(HEXOBJ hObj, HEXCANVAS hCanvas, RECT* lprcPaint);

/// <summary>
/// 组件设置可用状态
/// </summary>
/// <param name="hObj"></param>
/// <param name="bEnable">是否可用</param>
/// <returns></returns>
BOOL Ex_ObjEnable(HEXOBJ hObj, BOOL bEnable);

/// <summary>
/// 组件设置是否启用事件冒泡, 事件冒泡是指事件将根据父控件逐层传递至窗口
/// </summary>
/// <param name="hObj"></param>
/// <param name="bEnable"></param>
/// <returns></returns>
BOOL Ex_ObjEnableEventBubble(HEXOBJ hObj, BOOL bEnable);

/// <summary>
/// 组件是否允许启用输入法
/// </summary>
/// <param name="hObj"></param>
/// <param name="bEnable"></param>
/// <returns></returns>
BOOL Ex_ObjEnableIME(HEXOBJ hObj, BOOL bEnable);

/// <summary>
/// 组件设置是否启用绘画中消息
/// </summary>
/// <param name="hObj"></param>
/// <param name="bEnable"></param>
/// <returns></returns>
BOOL Ex_ObjEnablePaintingMsg(HEXOBJ hObj, BOOL bEnable);

/// <summary>
/// 组件结束绘制
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpPS"></param>
/// <returns></returns>
BOOL Ex_ObjEndPaint(HEXOBJ hObj, EX_PAINTSTRUCT* lpPS);

/// <summary>
/// 组件枚举子组件
/// </summary>
/// <param name="hObjParent">父组件的句柄。从根部枚举则为引擎句柄</param>
/// <param
/// name="lpEnumFunc">callback(hObj,lParam).返回1继续枚举,返回0停止枚举</param>
/// <param name="lParam">附带参数</param>
/// <returns></returns>
BOOL Ex_ObjEnumChild(EXHANDLE hObjParent, LPVOID lpEnumFunc, LPARAM lParam);

/// <summary>
/// 组件枚举属性表
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpfnCbk">回调LRESULT enum(HEXOBJ hObj,size_t dwKey,size_t
/// dwValue,size_t param)</param> <param name="param"></param>
/// <returns>返回个数</returns>
INT Ex_ObjEnumProps(HEXOBJ hObj, EnumPropsPROC lpfnCbk, size_t param);

/// <summary>
/// 组件查找
/// </summary>
/// <param name="hObjParent">父组件句柄。从根部查找则为引擎句柄</param>
/// <param name="hObjChildAfter">
/// 由此组件开始查找（不包含该组件）。如果为0，则从第一个组件开始查找</param>
/// <param name="lpClassName">欲查找的组件类名指针/Ex_ATOM()</param> <param
/// name="lpTitle">欲查找的组件标题</param> <returns></returns>
HEXOBJ Ex_ObjFind(EXHANDLE hObjParent, HEXOBJ hObjChildAfter,
                  LPCWSTR lpClassName, LPCWSTR lpTitle);

/// <summary>
/// 组件获取背景信息
/// </summary>
/// <param name="handle"></param>
/// <param name="lpBackgroundImage">相关结构 EX_BACKGROUNDIMAGEINFO</param>
/// <returns></returns>
BOOL Ex_ObjGetBackgroundImage(EXHANDLE handle,
                              EX_BACKGROUNDIMAGEINFO* lpBackgroundImage);

/// <summary>
/// 组件获取类信息
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpClassInfo"></param>
/// <returns></returns>
BOOL Ex_ObjGetClassInfo(HEXOBJ hObj, EX_CLASSINFO* lpClassInfo);

/// <summary>
/// 通过类名/类ATOM获取类信息
/// </summary>
/// <param name="lptszClassName"></param>
/// <param name="lpClassInfo">相关结构 EX_CLASSINFO</param>
/// <returns></returns>
BOOL Ex_ObjGetClassInfoEx(LPCWSTR lptszClassName, EX_CLASSINFO* lpClassInfo);

/// <summary>
/// 组件获取客户区矩形,同Ex_ObjGetRect 不包括dpi缩放
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpRect"></param>
/// <returns></returns>
BOOL Ex_ObjGetClientRect(HEXOBJ hObj, RECT* lpRect);

/// <summary>
/// 组件获取相关颜色
/// </summary>
/// <param name="hObj"></param>
/// <param name="nIndex">相关常量 COLOR_EX_</param>
/// <returns></returns>
EXARGB Ex_ObjGetColor(HEXOBJ hObj, INT nIndex);

/// <summary>
/// 组件查询拖曳文本内容
/// </summary>
/// <param name="hObj"></param>
/// <param name="pDataObject"></param>
/// <param name="lpwzBuffer">接收内容的文本</param>
/// <param name="cchMaxLength">接收内容的文本尺寸</param>
/// <returns></returns>
INT Ex_ObjGetDropString(HEXOBJ hObj, LPVOID pDataObject, LPWSTR lpwzBuffer,
                        INT cchMaxLength);

/// <summary>
/// 获取焦点组件
/// </summary>
/// <param name="hExDuiOrhObj"></param>
/// <returns></returns>
HEXOBJ Ex_ObjGetFocus(EXHANDLE hExDuiOrhObj);

/// <summary>
/// 组件获取字体。用户不应该销毁该字体对象
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
HEXFONT Ex_ObjGetFont(HEXOBJ hObj);

/// <summary>
/// 获取组件句柄自ID
/// </summary>
/// <param
/// name="hExDuiOrhObj">如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索.</param>
/// <param name="nID"></param>
/// <returns></returns>
HEXOBJ Ex_ObjGetFromID(EXHANDLE hExDuiOrhObj, INT nID);

/// <summary>
/// 获取组件句柄自名称
/// </summary>
/// <param
/// name="hExDuiOrhObj">如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索.</param>
/// <param name="lpName"></param>
/// <returns></returns>
HEXOBJ Ex_ObjGetFromName(EXHANDLE hExDuiOrhObj, LPCWSTR lpName);

/// <summary>
/// 获取组件句柄自节点ID
/// </summary>
/// <param
/// name="hExDUIOrObj">如果指定引擎句柄,则全部搜索;如果指定组件句柄,则从该组件的子组件开始搜索</param>
/// <param name="nNodeID"></param>
/// <returns></returns>
HEXOBJ Ex_ObjGetFromNodeID(EXHANDLE hExDUIOrObj, INT nNodeID);

/// <summary>
/// 组件获取数值
/// </summary>
/// <param name="hObj"></param>
/// <param name="nIndex">相关常量 #OBJECT_LONG_</param>
/// <returns></returns>
LONG_PTR Ex_ObjGetLong(HEXOBJ hObj, INT nIndex);

/// <summary>
/// 组件返回特定关系（如Z序或所有者）的组件句柄。
/// </summary>
/// <param name="hObj"></param>
/// <param name="nCmd">相关常量 #GW_</param>
/// <returns></returns>
HEXOBJ Ex_ObjGetObj(HEXOBJ hObj, INT nCmd);

/// <summary>
/// 组件获取父组件
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
EXHANDLE Ex_ObjGetParent(HEXOBJ hObj);

/// <summary>
/// 组件获取父组件和EXDUI句柄
/// </summary>
/// <param name="hObj"></param>
/// <param name="phExDUI"></param>
/// <returns>返回父组件句柄</returns>
EXHANDLE Ex_ObjGetParentEx(HEXOBJ hObj, HEXDUI* phExDUI);

/// <summary>
/// 组件获取属性
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwKey"></param>
/// <returns></returns>
size_t Ex_ObjGetProp(HEXOBJ hObj, size_t dwKey);

/// <summary>
/// 组件取矩形,同Ex_ObjGetClientRect 不包括dpi缩放
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpRect"></param>
/// <returns></returns>
BOOL Ex_ObjGetRect(HEXOBJ hObj, RECT* lpRect);

/// <summary>
/// 组件取矩形Ex
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpRect"></param>
/// <param name="nType">0组件相对位置矩形 1组件客户区矩形 2组件窗口矩形
/// 3组件脏区域矩形 4组件文本矩形</param> <returns></returns>
BOOL Ex_ObjGetRectEx(HEXOBJ hObj, RECT* lpRect, INT nType);

/// <summary>
/// 组件获取文本
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpString">缓冲区指针</param>
/// <param name="nMaxCount">缓冲区长度</param>
/// <returns>返回已拷贝字符长度,unicode每个字符两字节,包括终止符2字节,拼接文本注意减去终止符两字节</returns>
size_t Ex_ObjGetText(HEXOBJ hObj, LPCWSTR lpString, size_t nMaxCount);

/// <summary>
/// 组件获取文本长度
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
size_t Ex_ObjGetTextLength(HEXOBJ hObj);

/// <summary>
/// 组件获取文本矩形
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpRect"></param>
/// <returns></returns>
BOOL Ex_ObjGetTextRect(HEXOBJ hObj, RECT* lpRect);

/// <summary>
/// 组件获取状态
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
INT Ex_ObjGetUIState(HEXOBJ hObj);

/// <summary>
/// 组件挂接事件回调
/// </summary>
/// <param name="hObj"></param>
/// <param name="nEvent">#NM_</param>
/// <param name="pfnCallback">(LRESULT) Handler(HEXOBJ hObj,INT nID,INT
/// nCode,WPARAM wParam,LPARAM lParam)
/// 返回0继续分发事件,返回1则停止继续分发事件</param> <returns></returns>
BOOL Ex_ObjHandleEvent(HEXOBJ hObj, INT nEvent, EventHandlerPROC pfnCallback);

/// <summary>
/// 初始化属性列表,要注意每次初始化都会清空之前存储的内容,若存储的是指针需要自己先行释放
/// </summary>
/// <param name="hObj"></param>
/// <param
/// name="nPropCount">-1为哈希表模式,0为不存储属性,正数为属性个数(每个sizeof(size_t)字节)</param>
/// <returns></returns>
BOOL Ex_ObjInitPropList(HEXOBJ hObj, INT nPropCount);

/// <summary>
/// 组件设置重画区域
/// </summary>
/// <param name="hObj"></param>
/// <param name="lprcRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjInvalidateRect(HEXOBJ hObj, RECT* lprcRedraw);

/// <summary>
/// 组件是否可用
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjIsEnable(HEXOBJ hObj);

/// <summary>
/// 组件是否有效
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjIsValidate(HEXOBJ hObj);

/// <summary>
/// 组件是否可视
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjIsVisible(HEXOBJ hObj);

/// <summary>
/// 组件销毁焦点
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjKillFocus(HEXOBJ hObj);

/// <summary>
/// 组件销毁时钟
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjKillTimer(HEXOBJ hObj);

/// <summary>
/// 清空对象布局信息
/// </summary>
/// <param name="handle"></param>
/// <param name="bChildren">是否清空所有子组件,否则为当前组件</param>
/// <returns></returns>
BOOL Ex_ObjLayoutClear(EXHANDLE handle, BOOL bChildren);

/// <summary>
/// 组件获取布局对象句柄
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
HEXLAYOUT Ex_ObjLayoutGet(EXHANDLE handle);

/// <summary>
/// 组件设置布局对象句柄
/// </summary>
/// <param name="handle"></param>
/// <param name="hLayout"></param>
/// <param name="fUpdate"></param>
/// <returns></returns>
BOOL Ex_ObjLayoutSet(EXHANDLE handle, HEXLAYOUT hLayout, BOOL fUpdate);

/// <summary>
/// 组件更新对象布局
/// </summary>
/// <param name="handle"></param>
/// <returns></returns>
BOOL Ex_ObjLayoutUpdate(EXHANDLE handle);

/// <summary>
/// 移动组件
/// </summary>
/// <param name="hObj"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="bRepaint"></param>
/// <returns></returns>
BOOL Ex_ObjMove(HEXOBJ hObj, INT x, INT y, INT width, INT height,
                BOOL bRepaint);

/// <summary>
/// 坐标转换
/// </summary>
/// <param name="hObjSrc"></param>
/// <param name="hObjDst">0为所属窗口</param>
/// <param name="ptX"></param>
/// <param name="ptY"></param>
/// <returns></returns>
BOOL Ex_ObjPointTransform(HEXOBJ hObjSrc, HEXOBJ hObjDst, INT* ptX, INT* ptY);

/// <summary>
/// 组件投递消息
/// </summary>
/// <param name="hObj"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
BOOL Ex_ObjPostMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// 注册组件
/// </summary>
/// <param
/// name="lptszClassName">组件类名.最大允许长度:MAX_CLASS_NAME_LEN</param>
/// <param name="dwStyle">组件默认风格    OBJECT_STYLE_</param>
/// <param name="dwStyleEx">组件默认扩展风格 OBJECT_STYLE_EX</param>
/// <param name="dwTextFormat">相关常量 DT_</param>
/// <param name="cbObjExtra">组件额外分配字节数(值可为0),每个成员8字节</param>
/// <param name="hCursor">组件默认鼠标指针句柄(值可为0)</param>
/// <param name="dwFlags">画布标志 #ECVF_(值可为0)</param>
/// <param name="pfnObjProc">组件默认回调 LRESULT CALLBACK ClsPROC(HWND hWnd,
/// HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)</param>
/// <returns>成功返回组件类名原子,失败返回0</returns>
EXATOM Ex_ObjRegister(LPCWSTR lptszClassName, INT dwStyle, INT dwStyleEx,
                      INT dwTextFormat, DWORD cbObjExtra, HCURSOR hCursor,
                      INT dwFlags, ClsPROC pfnObjProc);

/// <summary>
/// 组件移除属性
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwKey"></param>
/// <returns>返回旧属性</returns>
size_t Ex_ObjRemoveProp(HEXOBJ hObj, size_t dwKey);

/// <summary>
/// 组件屏幕坐标到客户区坐标
/// </summary>
/// <param name="hObj"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
BOOL Ex_ObjScreenToClient(HEXOBJ hObj, INT* x, INT* y);

/// <summary>
/// 组件禁用/启用滚动条
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">支持#SCROLLBAR_TYPE_</param>
/// <param name="wArrows">相关常量 ESB_</param>
/// <returns></returns>
BOOL Ex_ObjScrollEnable(HEXOBJ hObj, INT nBar, INT wArrows);

/// <summary>
/// 组件获取滚动条句柄
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">相关常量 #SCROLLBAR_TYPE_</param>
/// <returns>返回滚动条句柄</returns>
HEXOBJ Ex_ObjScrollGetControl(HEXOBJ hObj, INT nBar);

/// <summary>
/// 组件获取滚动条信息
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar"></param>
/// <param name="lpnMin"></param>
/// <param name="lpnMax"></param>
/// <param name="lpnPos"></param>
/// <param name="lpnTrackPos"></param>
/// <returns></returns>
BOOL Ex_ObjScrollGetInfo(HEXOBJ hObj, INT nBar, INT* lpnMin, INT* lpnMax,
                         INT* lpnPos, INT* lpnTrackPos);

/// <summary>
/// 组件获取滚动条位置
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">相关常量 #SCROLLBAR_TYPE_</param>
/// <returns>返回滚动条位置</returns>
INT Ex_ObjScrollGetPos(HEXOBJ hObj, INT nBar);

/// <summary>
/// 组件获取滚动条范围
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">相关常量 #SCROLLBAR_TYPE_</param>
/// <param name="lpnMinPos"></param>
/// <param name="lpnMaxPos"></param>
/// <returns></returns>
BOOL Ex_ObjScrollGetRange(HEXOBJ hObj, INT nBar, INT* lpnMinPos,
                          INT* lpnMaxPos);

/// <summary>
/// 组件获取滚动条拖动位置
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">相关常量 #SCROLLBAR_TYPE_</param>
/// <returns></returns>
INT Ex_ObjScrollGetTrackPos(HEXOBJ hObj, INT nBar);

/// <summary>
/// 组件设置滚动条信息
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">SCROLLBAR_TYPE_</param>
/// <param name="Mask">SIF_</param>
/// <param name="nMin"></param>
/// <param name="nMax"></param>
/// <param name="nPage"></param>
/// <param name="nPos"></param>
/// <param name="bRedraw"></param>
/// <returns></returns>
INT Ex_ObjScrollSetInfo(HEXOBJ hObj, INT nBar, INT Mask, INT nMin, INT nMax,
                        INT nPage, INT nPos, BOOL bRedraw);

/// <summary>
/// 组件设置滚动条位置
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">SCROLLBAR_TYPE_</param>
/// <param name="nPos"></param>
/// <param name="bRedraw"></param>
/// <returns></returns>
INT Ex_ObjScrollSetPos(HEXOBJ hObj, INT nBar, INT nPos, BOOL bRedraw);

/// <summary>
/// 组件设置滚动条范围
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">SCROLLBAR_TYPE_</param>
/// <param name="nMin"></param>
/// <param name="nMax"></param>
/// <param name="bRedraw"></param>
/// <returns></returns>
INT Ex_ObjScrollSetRange(HEXOBJ hObj, INT nBar, INT nMin, INT nMax,
                         BOOL bRedraw);

/// <summary>
/// 组件显示/隐藏滚动条
/// </summary>
/// <param name="hObj"></param>
/// <param name="nBar">SCROLLBAR_TYPE_</param>
/// <param name="fShow"></param>
/// <returns></returns>
BOOL Ex_ObjScrollShow(HEXOBJ hObj, INT nBar, BOOL fShow);

/// <summary>
/// 组件发送消息
/// </summary>
/// <param name="hObj"></param>
/// <param name="uMsg"></param>
/// <param name="wParam"></param>
/// <param name="lParam"></param>
/// <returns></returns>
size_t Ex_ObjSendMessage(HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);

/// <summary>
/// 组件设置背景信息
/// </summary>
/// <param name="handle"></param>
/// <param name="lpImage">图片指针</param>
/// <param name="dwImageLen">图片长度  0 为销毁背景 或调用
/// Ex_ObjDestroyBackground 销毁</param> <param name="x">偏移X</param> <param
/// name="y">偏移Y</param> <param name="dwRepeat">相关常量 BIR_</param> <param
/// name="lpGrid">九宫矩形指针 (值可为0)</param> <param name="dwFlags">相关常量
/// BIF_</param> <param name="dwAlpha">透明度(0-255)</param> <param
/// name="fUpdate">是否立即刷新</param> <returns></returns>
BOOL Ex_ObjSetBackgroundImage(EXHANDLE handle, LPVOID lpImage,
                              size_t dwImageLen, INT x, INT y, DWORD dwRepeat,
                              RECT* lpGrid, INT dwFlags, DWORD dwAlpha,
                              BOOL fUpdate);

/// <summary>
/// 组件设置背景信息从Svg句柄
/// </summary>
/// <param name="handle"></param>
/// <param name="hSvg">Svg句柄</param>
/// <param name="color">填充颜色，0为不填充</param>
/// <param name="x">偏移X</param>
/// <param name="y">偏移Y</param>
/// <param name="dwRepeat">相关常量 BIR_</param>
/// <param name="lpGrid">九宫矩形指针 (值可为0)</param>
/// <param name="dwFlags">相关常量 BIF_</param>
/// <param name="dwAlpha">透明度(0-255)</param>
/// <param name="fUpdate">是否立即刷新</param>
/// <returns></returns>
BOOL Ex_ObjSetBackgroundImageFromSvg(EXHANDLE handle, HEXSVG hSvg,
                                        INT x, INT y,
                                        DWORD dwRepeat, RECT* lpGrid,
                                        INT dwFlags, DWORD dwAlpha,
                                        BOOL fUpdate);

/// <summary>
/// 组件设置背景图片播放状态
/// </summary>
/// <param name="handle"></param>
/// <param name="fPlayFrames">是否播放</param>
/// <param name="fResetFrame">是否重置当前帧为首帧</param>
/// <param name="fUpdate">是否更新背景</param>
/// <returns></returns>
BOOL Ex_ObjSetBackgroundPlayState(EXHANDLE handle, BOOL fPlayFrames,
                                  BOOL fResetFrame, BOOL fUpdate);

/// <summary>
/// 组件设置模糊度
/// </summary>
/// <param name="hObj"></param>
/// <param name="fDeviation">模糊度</param>
/// <param name="bRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetBlur(HEXOBJ hObj, FLOAT fDeviation, BOOL bRedraw);

/// <summary>
/// 组件设置相关颜色
/// </summary>
/// <param name="hObj"></param>
/// <param name="nIndex"> 相关常量 COLOR_EX_</param>
/// <param name="dwColor"></param>
/// <param name="fRedraw"></param>
/// <returns></returns>
EXARGB Ex_ObjSetColor(HEXOBJ hObj, INT nIndex, EXARGB dwColor, BOOL fRedraw);

/// <summary>
/// 编辑框设置选中行字符格式
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwMask">相关常量 #EDIT_SELECT_CHARFORMAT_</param>
/// <param name="crText">文本颜色,可空</param>
/// <param name="wzFontFace">字体名称,可空</param>
/// <param name="fontSize">字体尺寸,可空</param>
/// <param name="yOffset">字体垂直偏移,可空</param>
/// <param name="bBold">是否加粗,可空</param>
/// <param name="bItalic">是否倾斜,可空</param>
/// <param name="bUnderLine">是否下划线,可空</param>
/// <param name="bStrikeOut">是否删除线,可空</param>
/// <param name="bLink">是否超链接,可空</param>
/// <returns>返回结果</returns>
size_t Ex_ObjEditSetSelCharFormat(HEXOBJ hObj, INT dwMask, EXARGB crText = NULL,
                                  LPCWSTR wzFontFace = NULL,
                                  DWORD fontSize = NULL, INT yOffset = NULL,
                                  BOOL bBold = FALSE, BOOL bItalic = FALSE,
                                  BOOL bUnderLine = FALSE,
                                  BOOL bStrikeOut = FALSE, BOOL bLink = FALSE);

/// <summary>
/// 编辑框设置选中行段落格式
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwMask">相关常量 #EDIT_SELECT_PARAGRAPHFORMAT_</param>
/// <param name="wNumbering">编号类型,可空 #EDIT_PARAGRAPHFSYMBOL_常量</param>
/// <param name="dxStartIndent">首行缩进,可空</param>
/// <param name="dxRightIndent">右侧缩进,可空</param>
/// <param name="dxOffset">非首行缩进,可空</param>
/// <param name="wAlignment">段落对齐方式,可空
/// #EDIT_PARAGRAPHFALIGN_常量</param> <returns>返回结果</returns>
size_t Ex_ObjEditSetSelParFormat(HEXOBJ hObj, DWORD dwMask,
                                 WORD wNumbering = NULL,
                                 INT dxStartIndent = NULL,
                                 INT dxRightIndent = NULL, INT dxOffset = NULL,
                                 WORD wAlignment = NULL);

/// <summary>
/// 组件设置焦点
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjSetFocus(HEXOBJ hObj);

/// <summary>
/// 组件设置文本字体从字体句柄
/// </summary>
/// <param name="hObj"></param>
/// <param name="hFont"></param>
/// <param name="fRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetFont(HEXOBJ hObj, HEXFONT hFont, BOOL fRedraw);

/*
 * @fnType ExDirectUI
 * @brief Ex_ObjSetFontFromFamily
 * @param 1  hObj                    [HEXOBJ]
 * @param 2  lpszFontfamily          [LPCWSTR]             -1 为默认字体
 * @param 3  dwFontsize              [INT]               -1 为默认尺寸
 * @param 4  dwFontstyle             [INT]               -1 为默认风格
 * @param 5  fRedraw                 [BOOL]
 * @return [BOOL]返回是否成功
 */

/// <summary>
/// 组件设置文本字体从字体名称
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpszFontfamily">-1 为默认字体</param>
/// <param name="dwFontsize">-1 为默认尺寸</param>
/// <param name="dwFontstyle">-1 为默认风格</param>
/// <param name="fRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetFontFromFamily(HEXOBJ hObj, LPCWSTR lpszFontfamily,
                             INT dwFontsize, INT dwFontstyle, BOOL fRedraw);

/// <summary>
/// 组件设置窗口的输入法状态
/// </summary>
/// <param name="hObjOrExDui"></param>
/// <param name="fOpen"></param>
/// <returns></returns>
BOOL Ex_ObjSetIMEState(EXHANDLE hObjOrExDui, BOOL fOpen);

/// <summary>
/// 组件设置数值
/// </summary>
/// <param name="hObj"></param>
/// <param name="nIndex">#OBJECT_LONG_</param>
/// <param name="dwNewLong"></param>
/// <returns></returns>
LONG_PTR Ex_ObjSetLong(HEXOBJ hObj, INT nIndex, LONG_PTR dwNewLong);

/// <summary>
/// 组件设置偏移矩形
/// </summary>
/// <param name="hObj"></param>
/// <param name="nPaddingType"></param>
/// <param name="left"></param>
/// <param name="top"></param>
/// <param name="right"></param>
/// <param name="bottom"></param>
/// <param name="fRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetPadding(HEXOBJ hObj, INT nPaddingType, INT left, INT top,
                      INT right, INT bottom, BOOL fRedraw);

/// <summary>
/// 组件置父
/// </summary>
/// <param name="hObj"></param>
/// <param name="hParent">新的父控件/引擎/窗口句柄</param>
/// <returns></returns>
BOOL Ex_ObjSetParent(HEXOBJ hObj, EXHANDLE hParent);

/// <summary>
/// 组件设置位置
/// </summary>
/// <param name="hObj"></param>
/// <param name="hObjInsertAfter"></param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="flags">相关常量 #SWP_</param>
/// <returns></returns>
BOOL Ex_ObjSetPos(HEXOBJ hObj, HEXOBJ hObjInsertAfter, INT x, INT y, INT width,
                  INT height, INT flags);

/// <summary>
/// 组件设置属性
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwKey"></param>
/// <param name="dwValue"></param>
/// <returns>返回旧属性</returns>
size_t Ex_ObjSetProp(HEXOBJ hObj, size_t dwKey, size_t dwValue);

/// <summary>
/// 组件设置圆角度
/// </summary>
/// <param name="hObj"></param>
/// <param name="topleft"></param>
/// <param name="topright"></param>
/// <param name="bottomright"></param>
/// <param name="bottomleft"></param>
/// <param name="fUpdate"></param>
/// <returns></returns>
BOOL Ex_ObjSetRadius(HEXOBJ hObj, FLOAT topleft, FLOAT topright,
                     FLOAT bottomright, FLOAT bottomleft, BOOL fUpdate);

/// <summary>
/// 组件设置是否可以重画.如果组件扩展风格存在OBJECT_STYLE_EX_COMPOSITED,则该函数无效.
/// </summary>
/// <param name="hObj"></param>
/// <param name="fCanbeRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetRedraw(HEXOBJ hObj, BOOL fCanbeRedraw);

/// <summary>
/// 组件设置文本
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpString">指向一个空结束的字符串的指针</param>
/// <param name="fRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetText(HEXOBJ hObj, LPCWSTR lpString, BOOL fRedraw);

/// <summary>
/// 组件设置文本格式
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwTextFormat">DT_</param>
/// <param name="bRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetTextFormat(HEXOBJ hObj, DWORD dwTextFormat, BOOL bRedraw);

/// <summary>
/// 组件设置时钟
/// </summary>
/// <param name="hObj"></param>
/// <param name="uElapse">单位毫秒</param>
/// <returns></returns>
INT Ex_ObjSetTimer(HEXOBJ hObj, INT uElapse);

/// <summary>
/// 组件设置状态,用于更新组件界面显示状态
/// </summary>
/// <param name="hObj"></param>
/// <param name="dwState"></param>
/// <param name="fRemove"></param>
/// <param name="lprcRedraw"></param>
/// <param name="fRedraw"></param>
/// <returns></returns>
BOOL Ex_ObjSetUIState(HEXOBJ hObj, DWORD dwState, BOOL fRemove,
                      RECT* lprcRedraw, BOOL fRedraw);

/// <summary>
/// 组件设置可视状态
/// </summary>
/// <param name="hObj"></param>
/// <param name="fShow"></param>
/// <returns></returns>
BOOL Ex_ObjShow(HEXOBJ hObj, BOOL fShow);

/// <summary>
/// 组件弹出或关闭提示文本
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpText">提示文本.该值为0则关闭提示文本</param>
/// <returns></returns>
BOOL Ex_ObjTooltipsPop(HEXOBJ hObj, LPCWSTR lpText);

/// <summary>
/// 组件弹出或关闭提示文本Ex
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpTitle">标题内容</param>
/// <param name="lpText">提示文本</param>
/// <param name="x">屏幕坐标.(-1:默认)</param>
/// <param name="y">屏幕坐标.(-1:默认)</param>
/// <param name="dwTime">单位:毫秒.(-1:默认,0:始终显示)</param>
/// <param name="nIcon"></param>
/// <param name="fShow"></param>
/// <returns></returns>
BOOL Ex_ObjTooltipsPopEx(HEXOBJ hObj, LPCWSTR lpTitle, LPCWSTR lpText, INT x,
                         INT y, INT dwTime, INT nIcon, BOOL fShow);

/// <summary>
/// 组件设置提示文本
/// </summary>
/// <param name="hObj"></param>
/// <param name="lpString"></param>
/// <returns></returns>
BOOL Ex_ObjTooltipsSetText(HEXOBJ hObj, LPCWSTR lpString);

/// <summary>
/// 组件立即更新
/// </summary>
/// <param name="hObj"></param>
/// <returns></returns>
BOOL Ex_ObjUpdate(HEXOBJ hObj);

/// <summary>
/// 读文件
/// </summary>
/// <param name="filePath">文件路径</param>
/// <param name="retData">返回数据</param>
/// <returns></returns>
BOOL Ex_ReadFile(LPCWSTR filePath, std::vector<CHAR>* retData);

/// <summary>
/// 读取RC资源
/// </summary>
/// <param name="lpname">资源ID</param>
/// <param name="lpType">资源类型</param>
/// <param name="retData">返回数据</param>
/// <returns></returns>
BOOL Ex_ReadResSource(WORD lpname, LPCWSTR lpType, std::vector<CHAR>* retData);

/// <summary>
/// 资源释放
/// </summary>
/// <param name="hRes"></param>
/// <returns></returns>
BOOL Ex_ResFree(HEXRES hRes);

/// <summary>
/// 获取资源文件
/// </summary>
/// <param name="hRes"></param>
/// <param name="lpwzPath"></param>
/// <param name="lpFile">文件数据指针.用户不应该释放该内存.</param>
/// <param name="dwFileLen">文件尺寸</param>
/// <returns></returns>
BOOL Ex_ResGetFile(HEXRES hRes, LPCWSTR lpwzPath, LPVOID* lpFile,
                   size_t* dwFileLen);

/// <summary>
/// 获取资源文件从路径原子
/// </summary>
/// <param name="hRes"></param>
/// <param name="atomPath">路径原子</param>
/// <param name="lpFile">文件数据指针.用户不应该释放该内存</param>
/// <param name="dwFileLen">文件尺寸</param>
/// <returns></returns>
BOOL Ex_ResGetFileFromAtom(HEXRES hRes, EXATOM atomPath, LPVOID* lpFile,
                           size_t* dwFileLen);

/// <summary>
/// 从文件加载资源
/// </summary>
/// <param name="lptszFile"></param>
/// <returns></returns>
HEXRES Ex_ResLoadFromFile(LPCWSTR lptszFile);

/// <summary>
/// 从内存加载资源，可加载从_res_pack打包的资源包
/// </summary>
/// <param name="lpData"></param>
/// <param name="dwDataLen"></param>
/// <returns></returns>
HEXRES Ex_ResLoadFromMemory(LPVOID lpData, size_t dwDataLen);

/// <summary>
/// 取DPI缩放值
/// </summary>
/// <param name="n"></param>
/// <returns></returns>
FLOAT Ex_Scale(FLOAT n);

/// <summary>
/// 设置最后错误代码.相关常量 :#ERROR_EX_
/// </summary>
/// <param name="nError"></param>
void Ex_SetLastError(INT nError);

/// <summary>
/// 暂停
/// </summary>
/// <param name="us"></param>
void Ex_Sleep(INT us);

/// <summary>
/// 绘制主题数据
/// </summary>
/// <param name="hTheme">主题句柄</param>
/// <param name="hCanvas"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="atomClass"></param>
/// <param name="atomSrcRect"></param>
/// <param name="dwAlpha">透明度(0-255)</param>
/// <returns></returns>
BOOL Ex_ThemeDrawControl(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft,
                         FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
                         EXATOM atomClass, EXATOM atomSrcRect, DWORD dwAlpha);

/// <summary>
/// 绘制主题数据Ex
/// </summary>
/// <param name="hTheme"></param>
/// <param name="hCanvas"></param>
/// <param name="dstLeft"></param>
/// <param name="dstTop"></param>
/// <param name="dstRight"></param>
/// <param name="dstBottom"></param>
/// <param name="atomClass"></param>
/// <param name="atomSrcRect"></param>
/// <param name="atomBackgroundRepeat"></param>
/// <param name="atomBackgroundPositon"></param>
/// <param name="atomBackgroundGrid"></param>
/// <param name="atomBackgroundFlags"></param>
/// <param name="dwAlpha"></param>
/// <returns></returns>
BOOL Ex_ThemeDrawControlEx(HEXTHEME hTheme, HEXCANVAS hCanvas, FLOAT dstLeft,
                           FLOAT dstTop, FLOAT dstRight, FLOAT dstBottom,
                           EXATOM atomClass, EXATOM atomSrcRect,
                           EXATOM atomBackgroundRepeat,
                           EXATOM atomBackgroundPositon,
                           EXATOM atomBackgroundGrid,
                           EXATOM atomBackgroundFlags, DWORD dwAlpha);

/// <summary>
/// 释放主题
/// </summary>
/// <param name="hTheme"></param>
/// <returns></returns>
BOOL Ex_ThemeFree(HEXTHEME hTheme);

/// <summary>
/// 获取主题相关颜色值
/// </summary>
/// <param name="hTheme"></param>
/// <param name="nIndex">#COLOR_EX_</param>
/// <returns></returns>
EXARGB Ex_ThemeGetColor(HEXTHEME hTheme, INT nIndex);

/// <summary>
/// 获取组件属性值指针
/// </summary>
/// <param name="hTheme"></param>
/// <param name="atomClass"></param>
/// <param name="atomProp"></param>
/// <returns></returns>
LPVOID Ex_ThemeGetValuePtr(HEXTHEME hTheme, EXATOM atomClass, EXATOM atomProp);

/// <summary>
/// 加载主题包自文件
/// </summary>
/// <param name="lptszFile"></param>
/// <param name="lpKey"></param>
/// <param name="dwKeyLen"></param>
/// <param name="bDefault"></param>
/// <returns></returns>
HEXTHEME Ex_ThemeLoadFromFile(LPCWSTR lptszFile, LPVOID lpKey, size_t dwKeyLen,
                              BOOL bDefault);

/// <summary>
/// 加载主题包自内存
/// </summary>
/// <param name="lpData"></param>
/// <param name="dwDataLen"></param>
/// <param name="lpKey"></param>
/// <param name="dwKeyLen"></param>
/// <param name="bDefault"></param>
/// <returns></returns>
HEXTHEME Ex_ThemeLoadFromMemory(LPVOID lpData, size_t dwDataLen, LPVOID lpKey,
                                size_t dwKeyLen, BOOL bDefault);

/// <summary>
/// 弹出菜单
/// </summary>
/// <param name="hMenu">菜单句柄</param>
/// <param name="uFlags">相关常量 TPM_</param>
/// <param name="x">弹出坐标X(屏幕坐标)</param>
/// <param name="y">弹出坐标Y(屏幕坐标)</param>
/// <param name="nReserved">0.备用</param>
/// <param
/// name="handle">组件句柄/引擎句柄/窗口句柄.(如果该值为窗口句柄且窗口未使用引擎渲染,则以默认菜单弹出)</param>
/// <param name="lpRC"></param>
/// <param name="pfnCallback">(LRESULT)MsgProc(HWND hWnd,size_t hExDui,INT
/// uMsg,WPARAM wParam,LPARAM lParam,LRESULT* lpResult)</param> <param
/// name="dwFlags">相关常量 EMNF_</param> <returns></returns>
BOOL Ex_TrackPopupMenu(HMENU hMenu, DWORD uFlags, INT x, INT y,
                       size_t nReserved, EXHANDLE handle, RECT* lpRC,
                       MsgPROC pfnCallback, DWORD dwFlags);

/// <summary>
/// 反初始化引擎
/// </summary>
void Ex_UnInit();

/// <summary>
/// 窗口居中
/// </summary>
/// <param name="hWnd">预居中的原始窗口</param>
/// <param name="hWndFrom">预居中的目标窗口,如果为0则为屏幕居中</param>
/// <param name="bFullScreen">是否全屏模式</param>
void Ex_WndCenterFrom(HWND hWnd, HWND hWndFrom, BOOL bFullScreen);

/// <summary>
/// 创建窗口
/// </summary>
/// <param name="hWndParent">父窗口句柄</param>
/// <param name="lpwzClassName">窗口类名</param>
/// <param name="lpwzWindowName">窗口标题</param>
/// <param name="x"></param>
/// <param name="y"></param>
/// <param name="width"></param>
/// <param name="height"></param>
/// <param name="dwStyle">窗口风格</param>
/// <param name="dwStyleEx">窗口扩展风格</param>
/// <returns>返回窗口句柄</returns>
HWND Ex_WndCreate(HWND hWndParent, LPCWSTR lpwzClassName,
                  LPCWSTR lpwzWindowName, INT x, INT y, INT width, INT height,
                  INT dwStyle, INT dwStyleEx);

/// <summary>
/// 窗口消息循环
/// </summary>
/// <returns>返回msg.wParam</returns>
WPARAM Ex_WndMsgLoop();

/// <summary>
/// 从缓冲区写文件
/// </summary>
/// <param name="szFileName">保存路径</param>
/// <param name="pData">数据指针</param>
/// <param name="dataSize">数据长度</param>
/// <returns>返回是否成功</returns>
BOOL Ex_WriteFile(LPCWSTR szFileName, LPVOID pData, size_t dataSize);

/// <summary>
/// 注册窗口类
/// </summary>
/// <param name="lpwzClassName">窗口类名</param>
/// <param name="hIcon">窗口大图标句柄</param>
/// <param name="hIconsm">窗口小图标句柄</param>
/// <param name="hCursor">窗口鼠标句柄</param>
/// <returns>返回窗口类原子</returns>
WORD Ex_WndRegisterClass(LPCWSTR lpwzClassName, HICON hIcon, HICON hIconsm,
                         HCURSOR hCursor);

/// <summary>
/// 取指针size_t值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <returns></returns>
LONG_PTR __get(LPVOID lpAddr, LONG_PTR offset);

/// <summary>
/// 取指针INT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <returns></returns>
INT __get_int(LPVOID lpAddr, LONG_PTR offset);

/// <summary>
/// 取指针FLOAT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <returns></returns>
FLOAT __get_float(LPVOID lpAddr, LONG_PTR offset);

/// <summary>
/// 取指针SHORT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <returns></returns>
SHORT __get_short(LPVOID lpAddr, LONG_PTR offset);

/// <summary>
/// 置指针size_t值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns>返回旧值</returns>
LONG_PTR __set(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/// <summary>
/// 置指针FLOAT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns></returns>
FLOAT __set_float(LPVOID lpAddr, LONG_PTR offset, FLOAT value);

/// <summary>
/// 置指针INT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns></returns>
INT __set_int(LPVOID lpAddr, LONG_PTR offset, INT value);

/// <summary>
/// 置指针SHORT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns></returns>
SHORT __set_short(LPVOID lpAddr, LONG_PTR offset, SHORT value);

/// <summary>
/// 指针位查询size_t值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL __query(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/// <summary>
/// 指针位查询INT值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
/// <returns></returns>
BOOL __query_int(LPVOID lpAddr, LONG_PTR offset, INT value);

/// <summary>
/// 指针位删除size_t值
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
void __del(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/// <summary>
/// 指针位添加size_t值(位或)
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
void __add(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/// <summary>
/// 指针位添加size_t值(相加)
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
void __addn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);

/// <summary>
/// 指针位减少size_t值(相减)
/// </summary>
/// <param name="lpAddr"></param>
/// <param name="offset"></param>
/// <param name="value"></param>
void __subn(LPVOID lpAddr, LONG_PTR offset, LONG_PTR value);
