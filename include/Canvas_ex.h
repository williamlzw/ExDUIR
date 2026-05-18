#pragma once

#define MAX_SELECTABLE_TEXTS 100

struct selectable_text_info_s {
    IDWriteTextLayout* pLayout = nullptr;
    FLOAT layoutX = 0;
    FLOAT layoutY = 0;
    INT textLen = 0;
    LPWSTR pText = nullptr;
};

struct canvas_s
{
    INT                          dwFlags_;
    INT                          width_;
    INT                          height_;
    ID2D1DeviceContext* pContext_;
    ID2D1GdiInteropRenderTarget* pGdiInterop_;
    ID2D1Bitmap* pBitmap_;
    wnd_s* pWnd_;
    INT                          c_left_;
    INT                          c_top_;
    INT                          c_right_;
    INT                          c_bottom_;
    INT selAnchorTextIdx = -1;   // 鼠标按下时的文本索引
    UINT32 selAnchorPos = 0;     // 鼠标按下时的字符位置

    INT selEndTextIdx = -1;      // 当前活动端的文本索引
    UINT32 selEndPos = 0;        // 当前活动端的字符位置
    BOOL isSelecting = FALSE;
    selectable_text_info_s selectableTexts[MAX_SELECTABLE_TEXTS];
    INT selectableTextCount = 0;

};

BOOL _md_create(LPVOID pData, size_t offsetDc, size_t offsetBmp, size_t offsetBits, INT width,
    INT height, INT* nError);
BOOL _md_destroy(LPVOID pData, size_t OffsetDc, size_t OffsetBmp, size_t OffsetBits);
BOOL _canvas_drawtextwitheffect(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hrText, LPCWSTR lpwzText,
    INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right,
    FLOAT bottom, INT iGlowsize, HEXBRUSH hrShadom);
void _canvas_recreate(canvas_s* pCanvas, INT width, INT height, INT* nError);
void _canvas_init(INT* nError);
void _canvas_uninit();
BOOL _canvas_drawimagefrombkgimg_ex(HEXCANVAS hCanvas, HEXIMAGE hImage, INT x, INT y, INT dwRepeat,
    RECT* lpGrid, INT dwFlags, INT dwAlpha, RECT* lpRcSrc,
    EX_RECTF* lpRCFDst);
BOOL _canvas_drawimagefrombkgimg(HEXCANVAS hCanvas, EX_BACKGROUNDIMAGEINFO* lpBkgImg);

BOOL _canvas_calctextsize_ex(canvas_s* pCanvas, font_s* pFont, LPCWSTR lpwzText, INT dwLen,
    INT dwDTFormat, FLOAT layoutWidth, FLOAT layoutHeight,
    FLOAT* lpWidth, FLOAT* lpHeight, IDWriteTextLayout** pLayout,
    INT* nError);
BOOL _canvas_drawtextex2(HEXCANVAS hCanvas, HEXFONT hFont, HEXBRUSH hBursh, LPCWSTR lpwzText,
    INT dwLen, INT dwDTFormat, FLOAT left, FLOAT top, FLOAT right,
    FLOAT bottom);
HEXCANVAS _canvas_createfrompwnd(wnd_s* pWnd, INT width, INT height, INT dwFlags, INT* nError);
HDC       _canvas_getdc_ex(canvas_s* pCanvas, INT* nError);
void      _canvas_releasedc_ex(canvas_s* pCanvas, INT* nError);

BOOL _canvas_hittesttext(IDWriteTextLayout* pLayout, FLOAT mouseX, FLOAT mouseY, FLOAT layoutX, FLOAT layoutY, UINT32* pTextPos, BOOL* pIsTrailing, BOOL* pIsInside);
void _canvas_get_selection_range(canvas_s* pCanvas, INT idx, UINT32 textLen, UINT32& outStart, UINT32& outEnd);