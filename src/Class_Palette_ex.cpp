#include "stdafx.h"



// 注册函数
void _palette_register() {
    DWORD cbObjExtra = 8 * sizeof(size_t); 
    Ex_ObjRegister(L"Palette", OBJECT_STYLE_VISIBLE, OBJECT_STYLE_EX_FOCUSABLE,
        DT_LEFT, cbObjExtra, LoadCursor(0, IDC_HAND), CANVAS_FLAG_CANVASANTIALIAS,
        _palette_proc);
}
// 主消息处理
LRESULT CALLBACK _palette_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam) {
    if (uMsg == WM_CREATE) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 0);
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN_AREA, 0); // 记录按下时的区域
        Ex_ObjSetLong(hObj, PALETTE_LONG_X1, 0);
        Ex_ObjSetLong(hObj, PALETTE_LONG_Y1, 0.0f);
        Ex_ObjSetLong(hObj, PALETTE_LONG_Y2, 0);
        Ex_ObjSetLong(hObj, PALETTE_LONG_QCRGB, ExRGB2ARGB(151315,255)); // 默认色
        Ex_ObjSetLong(hObj, PALETTE_LONG_ALPHA, 255); // 默认不透明
        Ex_ObjSetLong(hObj, PALETTE_LONG_AX, 255);    // 初始滑块在最右
    }
    else if (uMsg == WM_PAINT) {
        EX_PAINTSTRUCT ps = { 0 };
        if (Ex_ObjBeginPaint(hObj, &ps)) {
            _palette_on_draw(hObj, &ps);
            Ex_ObjEndPaint(hObj, &ps);
        }
    }
    else if (uMsg == WM_DESTROY) {
        // 销毁
    }
    else if (uMsg == WM_LBUTTONDOWN) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 1);
        // 记录按下时的区域
        RECT rc;
        Ex_ObjGetClientRectForDpi(hObj, &rc);
        auto x = GET_X_LPARAM(lParam);
        auto y = GET_Y_LPARAM(lParam);

        FLOAT colorBarLeft = (FLOAT)(rc.right - Ex_Scale(PALETTE_COLOR_BAR_WIDTH));
        FLOAT colorBarRight = colorBarLeft + Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
        INT downArea = 0;
        if (x >= colorBarLeft && x < colorBarRight && y >= rc.top && y < rc.bottom - Ex_Scale(PALETTE_MARGIN)) {
            downArea = 1;
        }
        else if (x >= rc.left && x < rc.right - Ex_Scale(PALETTE_MARGIN) && y >= rc.top && y < rc.bottom - Ex_Scale(PALETTE_MARGIN)) {
            downArea = 2;
        }
        else if (y >= rc.bottom - Ex_Scale(PALETTE_MARGIN) && x >= 0 && x < rc.right - Ex_Scale(PALETTE_MARGIN)) {
            downArea = 3;
        }
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN_AREA, downArea);
        Ex_ObjSendMessage(hObj, WM_MOUSEMOVE, 0, lParam);
    }
    else if (uMsg == WM_LBUTTONUP) {
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN, 0);
        Ex_ObjSetLong(hObj, PALETTE_LONG_DOWN_AREA, 0); // 重置区域
    }
    else if (uMsg == WM_MOUSEMOVE) {
        auto down = Ex_ObjGetLong(hObj, PALETTE_LONG_DOWN);
        if (down) {
            RECT rc;
            Ex_ObjGetClientRectForDpi(hObj, &rc);
            auto x = GET_X_LPARAM(lParam);
            auto y = GET_Y_LPARAM(lParam);
            if (x >= rc.left && x <= rc.right && y >= rc.top && y <= rc.bottom) {              
                FLOAT colorBarLeft = (FLOAT)(rc.right - Ex_Scale(PALETTE_COLOR_BAR_WIDTH)); // 与绘制起始x对齐
                FLOAT colorBarRight = colorBarLeft + Ex_Scale(PALETTE_COLOR_BAR_WIDTH);     // 宽14，与绘制一致
                auto downArea = Ex_ObjGetLong(hObj, PALETTE_LONG_DOWN_AREA); // 获取按下时的区域
                if (x >= colorBarLeft && x < colorBarRight && y >= rc.top && y < rc.bottom - Ex_Scale(PALETTE_MARGIN) && downArea == 1){
                    // 在七彩条上 - 只更新基色，保持主颜色区域选择点
                    Ex_ObjSetLong(hObj, PALETTE_LONG_Y1, y);
                    FLOAT colorBarHeight = rc.bottom - rc.top - Ex_Scale(PALETTE_MARGIN);
                    FLOAT t = (y - rc.top) / colorBarHeight;
                    t = max(0.0f, min(1.0f, t));
                    EXARGB cr = _palette_get_color_from_hue(t);
                    Ex_ObjSetLong(hObj, PALETTE_LONG_QCRGB, cr);
                    _palette_update_final_color(hObj, &rc);

                }
                else if (x >= rc.left && x < rc.right - Ex_Scale(PALETTE_MARGIN) && y >= rc.top && y < rc.bottom - Ex_Scale(PALETTE_MARGIN) && downArea == 2) {
                    // 在主颜色区域 - 更新饱和度和明度
                    Ex_ObjSetLong(hObj, PALETTE_LONG_X1, x);
                    Ex_ObjSetLong(hObj, PALETTE_LONG_Y2, y);

                    // 计算最终颜色并通知
                    EXARGB finalColor = _palette_update_final_color(hObj, &rc);
                    BYTE alpha = (BYTE)Ex_ObjGetLong(hObj, PALETTE_LONG_ALPHA);
                    // 发送通知
                    Ex_ObjDispatchNotify(hObj, PALETTE_EVENT_MOUSEMOVE, finalColor, alpha);
                }
                else if (y >= rc.bottom - Ex_Scale(PALETTE_MARGIN) && x >= 0 && x < rc.right - Ex_Scale(PALETTE_MARGIN) && downArea == 3) {
                    // 在 Alpha 滑块区域 - 只更新透明度
                    FLOAT alphaW = (FLOAT)(rc.right - rc.left - Ex_Scale(PALETTE_MARGIN));
                    FLOAT t = (x - rc.left) / alphaW;
                    t = max(0.0f, min(1.0f, t));
                    BYTE alpha = (BYTE)(t * 255 + 0.5f);
                    Ex_ObjSetLong(hObj, PALETTE_LONG_ALPHA, alpha);
                    Ex_ObjSetLong(hObj, PALETTE_LONG_AX, x);

                    // 只更新透明度，重新通知最终颜色
                    _palette_update_alpha_only(hObj, &rc);
                }
                Ex_ObjInvalidateRect(hObj, nullptr);
                
            }
        }
    }
    else if (uMsg == PALETTE_MESSAGE_SETCOLOR) {
        RECT rc;
        Ex_ObjGetClientRectForDpi(hObj, &rc);
        // 将传入的颜色转换为HSV，获取色调
        EXHSB hsb = ExRGBA2HSB((EXARGB)lParam);

        // 计算七色条上的位置（根据色调）
        FLOAT colorBarHeight = rc.bottom - rc.top - Ex_Scale(PALETTE_MARGIN);
        FLOAT y1 = rc.top + (hsb.h / 360.0f) * colorBarHeight;       
        Ex_ObjSetLong(hObj, PALETTE_LONG_Y1, (LONG)y1);// 更新七色条位置

        // 更新基色为传入颜色对应的纯色调颜色
        EXARGB hueColor = _palette_get_color_from_hue(hsb.h / 360.0f);
        Ex_ObjSetLong(hObj, PALETTE_LONG_QCRGB, hueColor);

        // 同时更新主颜色区域的选择点（根据饱和度和明度）
        FLOAT mainWidth = (FLOAT)(rc.right - rc.left - Ex_Scale(PALETTE_MARGIN));
        FLOAT mainHeight = (FLOAT)(rc.bottom - rc.top - Ex_Scale(PALETTE_MARGIN));

        FLOAT x1 = rc.left + hsb.s * mainWidth;
        FLOAT y2 = rc.top + (1.0f - hsb.b) * mainHeight;

        Ex_ObjSetLong(hObj, PALETTE_LONG_X1, (LONG)x1);
        Ex_ObjSetLong(hObj, PALETTE_LONG_Y2, (LONG)y2);

        // 更新Alpha通道（如果传入颜色有Alpha）
        BYTE alpha = ExGetA((EXARGB)lParam);
        if (alpha != 0) { // 只有当传入颜色有Alpha时才更新
            Ex_ObjSetLong(hObj, PALETTE_LONG_ALPHA, alpha);

            // 更新Alpha滑块位置
            FLOAT alphaW = (FLOAT)(rc.right - rc.left - Ex_Scale(PALETTE_MARGIN));
            FLOAT ax = rc.left + (alpha / 255.0f) * alphaW;
            Ex_ObjSetLong(hObj, PALETTE_LONG_AX, (LONG)ax);
        }

    }
    return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}
// 核心绘制函数
void _palette_on_draw(HEXOBJ hObj, EX_PAINTSTRUCT* ps) {
    if (!ps || !ps->hCanvas) return;
    _canvas_setantialias(ps->hCanvas, TRUE);
    FLOAT width = (FLOAT)ps->uWidth;
    FLOAT height = (FLOAT)ps->uHeight;
    // 获取状态
    FLOAT _x1 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_X1);
    FLOAT _y1 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_Y1);
    FLOAT _y2 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_Y2);
    EXARGB _qcrgb = (EXARGB)Ex_ObjGetLong(hObj, PALETTE_LONG_QCRGB);
    HEXBRUSH whiteBrush = _brush_create(ExARGB(255, 255, 255, 255));
    HEXBRUSH borderBrush = _brush_create(ExARGB(0, 0, 0, 66));
    // 七彩垂直渐变条区域
    {
        FLOAT x = width - Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
        FLOAT y = 0;
        FLOAT w = Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
        FLOAT h = height - Ex_Scale(PALETTE_MARGIN);

        const int N = 37; // 0~360 每 10 度一个点
        FLOAT pts[N] = {0};
        INT colors[N] = {0};
        for (int i = 0; i < N; ++i) {
            pts[i] = (FLOAT)i / (N - 1);
            EXARGB c = _palette_get_color_from_hue(pts[i]);
            colors[i] = c;
        }
        HEXBRUSH brush = _brush_createlinear_ex(x, y, x + w, y + h, pts, colors, N);
        if (brush) {
            _canvas_fillrect(ps->hCanvas, brush, x, y, x + w, y + h);
            _brush_destroy(brush);
        }

        // 七色条选择块
        {
            FLOAT colorBarTop = 0;
            FLOAT colorBarHeight = height - Ex_Scale(PALETTE_MARGIN); 
            // 仅当 y1 在七彩条有效范围内才绘制
            if (_y1 >= colorBarTop && _y1 <= colorBarTop + colorBarHeight) {
                // 填充白色
                FLOAT rectLeft = width - Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
                FLOAT rectTop = _y1 - 2;      // 向上偏移 2 像素，使 y1 居中
                FLOAT rectRight = rectLeft + Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
                FLOAT rectBottom = rectTop + PALETTE_SELECTOR_HEIGHT;
                _canvas_fillrect(ps->hCanvas, whiteBrush, rectLeft, rectTop, rectRight, rectBottom);

                // 绘制黑色边框
                _canvas_drawrect(ps->hCanvas, borderBrush, rectLeft, rectTop, rectRight, rectBottom, 1.0f, 0);
            }
        }
    }
    // 主颜色区域
    {
        FLOAT mainW = width - Ex_Scale(PALETTE_MARGIN);
        FLOAT mainH = height - Ex_Scale(PALETTE_MARGIN);
        // 白 → _qcrgb
        HEXBRUSH h1 = _brush_createlinear(0, 0, mainW, 0, ExARGB(255, 255, 255, 255), _qcrgb);
        if (h1) {
            _canvas_fillrect(ps->hCanvas, h1, 0, 0, mainW, mainH);
            _brush_destroy(h1);
        }
        // 透明黑 → 黑
        HEXBRUSH h2 = _brush_createlinear(0, 0, 0, mainH, ExRGB2ARGB(0, 0), ExRGB2ARGB(0, 255));
        if (h2) {
            _canvas_fillrect(ps->hCanvas, h2, 0, 0, mainW, mainH);
            _brush_destroy(h2);
        }
        // 选中点高亮
        // 仅当 x1, y2 落在主颜色区域内才绘制高亮
        if (_x1 >= 0 && _x1 < mainW && _y2 >= 0 && _y2 < mainH) {
            _canvas_drawroundedrect(ps->hCanvas, whiteBrush, _x1 - 3, _y2 - 3, _x1 + 7, _y2 + 7, 5, 5, 1.5f, 0);
        }
    }
    // Alpha滑块区域
    {
        FLOAT alphaY = height - Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
        FLOAT alphaH = Ex_Scale(PALETTE_COLOR_BAR_WIDTH);
        FLOAT alphaW = width - Ex_Scale(PALETTE_MARGIN);
        // 黑白棋盘格背景
        {
            const FLOAT tileSize = 7.0f;
            INT cols = (INT)ceil(alphaW / tileSize);
            INT rows = (INT)ceil(alphaH / tileSize);

            for (INT row = 0; row < rows; ++row) {
                for (INT col = 0; col < cols; ++col) {
                    BOOL isBlack = ((row + col) % 2) == 0;
                    EXARGB tileColor = isBlack ? ExARGB(0, 0, 0, 50) : ExARGB(255, 255, 255, 50);
                    HEXBRUSH tileBrush = _brush_create(tileColor);
                    if (tileBrush) {
                        FLOAT x1 = col * tileSize;
                        FLOAT y1 = alphaY + row * tileSize;
                        FLOAT x2 = min(x1 + tileSize, alphaW);
                        FLOAT y2 = min(y1 + tileSize, alphaY + alphaH);
                        _canvas_fillrect(ps->hCanvas, tileBrush, x1, y1, x2, y2);
                        _brush_destroy(tileBrush);
                    }
                }
            }
        }

        // Alpha渐变条（从透明到不透明）
        EXARGB baseColor = _palette_update_final_color(hObj,&ps->rcPaint);
        BYTE r = ExGetR(baseColor), g = ExGetG(baseColor), b = ExGetB(baseColor);
        EXARGB color0 = ExARGB(r, g, b, 0);      // 完全透明
        EXARGB color255 = ExARGB(r, g, b, 255);  // 完全不透明
        HEXBRUSH alphaBrush = _brush_createlinear(0, alphaY, alphaW, alphaY, color0, color255);
        if (alphaBrush) {
            _canvas_fillrect(ps->hCanvas, alphaBrush, 0, alphaY, alphaW, alphaY + alphaH);
            _brush_destroy(alphaBrush);
        }

        // 滑块指示器
        INT ax = (INT)Ex_ObjGetLong(hObj, PALETTE_LONG_AX);
        if (ax >= 0 && ax <= (INT)alphaW) {
            _canvas_fillrect(ps->hCanvas, whiteBrush, (FLOAT)ax - 2, alphaY, (FLOAT)ax + 4, alphaY + alphaH);

            _canvas_drawrect(ps->hCanvas, borderBrush, (FLOAT)ax - 2, alphaY, (FLOAT)ax + 4, alphaY + alphaH, 1.0f, 0);           
        }
    }
    _brush_destroy(whiteBrush);
    _brush_destroy(borderBrush);
}
// 归一化位置获取七彩色
EXARGB _palette_get_color_from_hue(FLOAT t) {
    // t ∈ [0, 1] → Hue ∈ [0, 360)
    FLOAT h = t * 360.0f;
    FLOAT s = 1.0f; // 饱和度最大
    FLOAT v = 1.0f; // 明度最大

    // HSV to RGB
    INT i = (INT)floor(h / 60.0f) % 6;
    FLOAT f = h / 60.0f - floor(h / 60.0f);
    FLOAT p = v * (1.0f - s);
    FLOAT q = v * (1.0f - f * s);
    FLOAT t_val = v * (1.0f - (1.0f - f) * s);

    FLOAT r, g, b;
    switch (i) {
    case 0: r = v; g = t_val; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t_val; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t_val; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
    default: r = v; g = p; b = q; break;
    }

    return ExARGB(
        (BYTE)(r * 255 + 0.5f),
        (BYTE)(g * 255 + 0.5f),
        (BYTE)(b * 255 + 0.5f),
        255
    );
}
// RGBA到HSB转换函数
EXHSB ExRGBA2HSB(EXARGB argb) {
    EXHSB hsb = { 0.0f, 0.0f, 0.0f };

    // 提取 RGB 分量
    BYTE r = ExGetR(argb);
    BYTE g = ExGetG(argb);
    BYTE b = ExGetB(argb);

    // 转换为 0-1 范围的浮点数
    float fr = r / 255.0f;
    float fg = g / 255.0f;
    float fb = b / 255.0f;

    // 找出最大值和最小值
    float max = (fr > fg) ? ((fr > fb) ? fr : fb) : ((fg > fb) ? fg : fb);
    float min = (fr < fg) ? ((fr < fb) ? fr : fb) : ((fg < fb) ? fg : fb);
    float delta = max - min;

    // 计算亮度
    hsb.b = max;

    // 计算饱和度
    if (max > 0.0f) {
        hsb.s = delta / max;
    }
    else {
        hsb.s = 0.0f;
    }

    // 计算色调
    if (delta > 0.0f) {
        if (max == fr) {
            // 红色为主
            hsb.h = 60.0f * fmodf(((fg - fb) / delta), 6.0f);
        }
        else if (max == fg) {
            // 绿色为主
            hsb.h = 60.0f * (((fb - fr) / delta) + 2.0f);
        }
        else {
            // 蓝色为主
            hsb.h = 60.0f * (((fr - fg) / delta) + 4.0f);
        }

        // 确保色调在 0-360 范围内
        if (hsb.h < 0.0f) {
            hsb.h += 360.0f;
        }
    }
    else {
        hsb.h = 0.0f; // 无色调
    }

    return hsb;
}
EXARGB _palette_update_final_color(HEXOBJ hObj, RECT* rc) {
    // 获取当前状态
    FLOAT x1 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_X1);
    FLOAT y2 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_Y2);
    EXARGB base = (EXARGB)Ex_ObjGetLong(hObj, PALETTE_LONG_QCRGB);

    // 计算饱和度和明度
    FLOAT mainWidth = (FLOAT)(rc->right - rc->left - Ex_Scale(PALETTE_MARGIN));
    FLOAT mainHeight = (FLOAT)(rc->bottom - rc->top - Ex_Scale(PALETTE_MARGIN));

    FLOAT s = (x1 - rc->left) / mainWidth;
    FLOAT v = 1.0f - (y2 - rc->top) / mainHeight;
    s = max(0.0f, min(1.0f, s));
    v = max(0.0f, min(1.0f, v));

    // 正确的HSV到RGB转换
    EXARGB rgbColor = _palette_hsv_to_rgb(base, s, v);
    //EXARGB finalColor = ExARGB(ExGetR(rgbColor), ExGetG(rgbColor), ExGetB(rgbColor), alpha);
    return rgbColor;
}
void _palette_update_alpha_only(HEXOBJ hObj, RECT* rc) {
    // 获取当前RGB颜色（不带Alpha）
    FLOAT x1 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_X1);
    FLOAT y2 = (FLOAT)Ex_ObjGetLong(hObj, PALETTE_LONG_Y2);
    EXARGB base = (EXARGB)Ex_ObjGetLong(hObj, PALETTE_LONG_QCRGB);
    BYTE alpha = (BYTE)Ex_ObjGetLong(hObj, PALETTE_LONG_ALPHA);

    // 计算RGB颜色
    FLOAT mainWidth = (FLOAT)(rc->right - rc->left - Ex_Scale(PALETTE_MARGIN));
    FLOAT mainHeight = (FLOAT)(rc->bottom - rc->top - Ex_Scale(PALETTE_MARGIN));

    FLOAT s = (x1 - rc->left) / mainWidth;
    FLOAT v = 1.0f - (y2 - rc->top) / mainHeight;
    s = max(0.0f, min(1.0f, s));
    v = max(0.0f, min(1.0f, v));

    EXARGB rgbColor = _palette_hsv_to_rgb(base, s, v);
    //EXARGB finalColor = ExARGB(ExGetR(rgbColor), ExGetG(rgbColor), ExGetB(rgbColor), alpha);
    // 发送通知
    Ex_ObjDispatchNotify(hObj, PALETTE_EVENT_MOUSEMOVE, rgbColor, alpha);
}
// HSV到RGB转换函数
EXARGB _palette_hsv_to_rgb(EXARGB baseColor, FLOAT s, FLOAT v) {
    // 从基色提取色调
    EXHSB baseHSB = ExRGBA2HSB(baseColor);
    FLOAT h = baseHSB.h; // 色调 (0-360)

    // HSV to RGB 转换
    FLOAT c = v * s;
    FLOAT x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    FLOAT m = v - c;

    FLOAT r, g, b;
    if (h >= 0 && h < 60) {
        r = c; g = x; b = 0;
    }
    else if (h < 120) {
        r = x; g = c; b = 0;
    }
    else if (h < 180) {
        r = 0; g = c; b = x;
    }
    else if (h < 240) {
        r = 0; g = x; b = c;
    }
    else if (h < 300) {
        r = x; g = 0; b = c;
    }
    else {
        r = c; g = 0; b = x;
    }

    return ExRGB((BYTE)((r + m) * 255),
        (BYTE)((g + m) * 255),
        (BYTE)((b + m) * 255));
}