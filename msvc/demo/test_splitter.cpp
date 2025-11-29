#include "test_splitter.h"

void test_splitter(HWND hWnd) {
  HWND hWnd_splitter =
      Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试分隔条", 0, 0, 400, 330, 0, 0);
  auto m_hExDui =
      Ex_DUIBindWindowEx(hWnd_splitter, 0,
                         WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
                             WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_BUTTON_MAX |
                             WINDOW_STYLE_SIZEABLE | WINDOW_STYLE_CENTERWINDOW |
                             WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON,
                         0, 0);
  Ex_DUISetLong(m_hExDui, ENGINE_LONG_CRBKG, ExARGB(80, 80, 90, 255));

  // 创建绝对布局
  HEXLAYOUT hLayout = _layout_create(LAYOUT_TYPE_ABSOLUTE, m_hExDui);
  auto splitter =
      Ex_ObjCreate(L"Splitter", 0, OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER,
                   0, 0, 0, 0, m_hExDui);

  _layout_absolute_setedge(hLayout, splitter, LAYOUT_SUBPROP_ABSOLUTE_LEFT,
                           LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 10);
  _layout_absolute_setedge(hLayout, splitter, LAYOUT_SUBPROP_ABSOLUTE_TOP,
                           LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 40);
  _layout_absolute_setedge(hLayout, splitter, LAYOUT_SUBPROP_ABSOLUTE_RIGHT,
                           LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 10);
  _layout_absolute_setedge(hLayout, splitter, LAYOUT_SUBPROP_ABSOLUTE_BOTTOM,
                           LAYOUT_SUBPROP_ABSOLUTE_TYPE_PX, 10);

  Ex_ObjSetLong(splitter, SPLITTER_LONG_POSITION, 35);  // 默认位置35%
  // 设置分割条颜色
  Ex_ObjSetLong(splitter, SPLITTER_LONG_COLOR, ExARGB(50, 50, 50, 255));
  // 默认固定面板1
  Ex_ObjSetLong(splitter, SPLITTER_LONG_FIXEDPANEL, 1);
  auto static1 =
      Ex_ObjCreate(L"static", L"面板一", -1, 0, 0, 100, 30, splitter);
  Ex_ObjSetColor(static1, COLOR_EX_BACKGROUND, ExARGB(50, 150, 150, 55), TRUE);

  auto splitter2 = Ex_ObjCreate(L"Splitter", 0, -1, 0, 0, 0, 0, splitter);
  // 设置分割条大小和方向
  Ex_ObjSetLong(splitter, SPLITTER_LONG_SIZE, 5);
  Ex_ObjSetLong(splitter2, SPLITTER_LONG_DIRECTION, 1);
  // 默认固定面板2
  Ex_ObjSetLong(splitter2, SPLITTER_LONG_FIXEDPANEL, 2);
  // 附加面板
  Ex_ObjSendMessage(splitter, SPLITTER_MESSAGE_SET_PANEL, static1, splitter2);

  auto static2 =
      Ex_ObjCreate(L"static", L"面板二", -1, 0, 0, 100, 30, splitter2);
  Ex_ObjSetColor(static2, COLOR_EX_BACKGROUND, ExARGB(150, 150, 50, 55), TRUE);
  auto static3 =
      Ex_ObjCreate(L"static", L"面板三", -1, 0, 0, 100, 30, splitter2);
  Ex_ObjSetColor(static3, COLOR_EX_BACKGROUND, ExARGB(150, 50, 150, 55), TRUE);
  // 附加面板
  Ex_ObjSendMessage(splitter2, SPLITTER_MESSAGE_SET_PANEL, static2, static3);

  Ex_ObjLayoutSet(m_hExDui, hLayout, TRUE);

  Ex_DUIShowWindow(m_hExDui, SW_SHOWNORMAL);
}
