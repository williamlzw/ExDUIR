#pragma once

#define PALETTE_LONG_X1        0 // 选中点X位置（主区域）
#define PALETTE_LONG_Y1        1 // 七彩条Y位置
#define PALETTE_LONG_Y2        2 // 选中点Y位置（主区域）
#define PALETTE_LONG_QCRGB     3 // 当前主色调（ARGB，但只用 RGB）
#define PALETTE_LONG_ALPHA     4 // 透明度
#define PALETTE_LONG_AX        5 // 滑块X位置
#define PALETTE_LONG_DOWN      6 // 调色板属性_是否按下
#define PALETTE_LONG_DOWN_AREA 7 // 记录按下时的区域

#define PALETTE_COLOR_BAR_WIDTH       14      // 七彩条宽度
#define PALETTE_MARGIN                18      // 主颜色/七彩条右边底部边距
#define PALETTE_SELECTOR_HEIGHT       6       // 选择器高度

// HSB 结构体
struct EXHSB {
    float h; // Hue: [0.0, 360.0)
    float s; // Saturation: [0.0, 1.0]
    float b; // Brightness/Value: [0.0, 1.0]
};

void             _palette_register();
LRESULT CALLBACK _palette_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void _palette_on_draw(HEXOBJ hObj, EX_PAINTSTRUCT* ps);
EXARGB _palette_get_color_from_hue(FLOAT t);

EXHSB ExRGBA2HSB(EXARGB argb);
EXARGB _palette_update_final_color(HEXOBJ hObj, RECT* rc);
void _palette_update_alpha_only(HEXOBJ hObj, RECT* rc);
EXARGB _palette_hsv_to_rgb(EXARGB baseColor, FLOAT s, FLOAT v);
