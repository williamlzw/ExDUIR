#pragma once
#include "stdafx.h"

// 分割条组件回调函数
inline LRESULT CALLBACK SplitterProc(HWND hWnd, HEXOBJ hObj, INT uMsg,
                                     WPARAM wParam, LPARAM lParam) {
  switch (uMsg) {
    case WM_CREATE: {
      // 初始化默认属性
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DIRECTION, 0);  // 默认垂直方向
      Ex_ObjSetLong(hObj, SPLITTER_LONG_POSITION, 20);  // 默认位置20%
      Ex_ObjSetLong(hObj, SPLITTER_LONG_PANEL1, 0);     // 默认面板1为空
      Ex_ObjSetLong(hObj, SPLITTER_LONG_PANEL2, 0);     // 默认面板2为空
      Ex_ObjSetLong(hObj, SPLITTER_LONG_SIZE, 4);       // 默认分割条宽度4像素
      Ex_ObjSetLong(hObj, SPLITTER_LONG_COLOR,
                    ExARGB(200, 200, 200, 255));           // 默认颜色
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, FALSE);  // 默认不拖动
      Ex_ObjSetLong(hObj, SPLITTER_LONG_LOCK, 0);          // 默认不锁定
      Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, 0);   // 当前像素位置0
    } break;
    case WM_SIZE: {
      // 调整大小时，更新分割条位置
      RECT m_rect{};
      Ex_ObjGetRectEx(hObj, &m_rect, 1);
      INT direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      INT position = Ex_ObjGetLong(hObj, SPLITTER_LONG_POSITION);
      if (direction == 0) {
        // 垂直分割条
        if (position) {
          position = (m_rect.right - m_rect.left) * position / 100;
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, position);
        }
      } else {
        // 水平分割条
        if (position) {
          position = (m_rect.bottom - m_rect.top) * position / 100;
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, position);
        }
      }
      HEXOBJ panel1 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
      HEXOBJ panel2 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);
      INT newPosition = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE);

      if (panel1) {
        Ex_ObjMove(panel1, m_rect.left, m_rect.top,
                   direction == 0 ? newPosition - m_rect.left
                                  : m_rect.right - m_rect.left,
                   direction == 0 ? m_rect.bottom - m_rect.top
                                  : newPosition - m_rect.top,
                   false);
      }

      if (panel2) {
        Ex_ObjMove(panel2, direction == 0 ? newPosition + size : m_rect.left,
                   direction == 0 ? m_rect.top : newPosition + size,
                   direction == 0 ? m_rect.right - m_rect.left -
                                        (newPosition - m_rect.left + size)
                                  : m_rect.right - m_rect.left,
                   direction == 0 ? m_rect.bottom - m_rect.top
                                  : m_rect.bottom - m_rect.top -
                                        (newPosition - m_rect.top + size),
                   false);

        Ex_ObjInvalidateRect(hObj, NULL);
      }
    } break;
    case WM_LBUTTONDOWN: {
      Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, FALSE);
      float dpi = Flag_Query(ENGINE_FLAG_DPI_ENABLE)
                      ? GetDpiForWindow(hWnd) / 96.f
                      : 1.f;

      int x = GET_X_LPARAM(lParam) / dpi;
      int y = GET_Y_LPARAM(lParam) / dpi;

      RECT m_rect{};  // 当前m_rect
      Ex_ObjGetRectEx(hObj, &m_rect, 1);

      INT direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      INT position = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE);
      HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);
      if (direction == 0) {
        if (x >= m_rect.left + position && x <= m_rect.left + position + size) {
          // 水平分割条
          hCursor = LoadCursor(NULL, IDC_SIZEWE);
          Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, TRUE);
        }
      } else {
        if (y >= m_rect.top + position && y <= m_rect.top + position + size) {
          // 垂直分割条
          hCursor = LoadCursor(NULL, IDC_SIZENS);
          Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, TRUE);
        }
      }
      SetCursor(hCursor);

    } break;
    case WM_MOUSEMOVE: {
      float dpi = Flag_Query(ENGINE_FLAG_DPI_ENABLE)
                      ? GetDpiForWindow(hWnd) / 96.f
                      : 1.f;

      int x = GET_X_LPARAM(lParam) / dpi;
      int y = GET_Y_LPARAM(lParam) / dpi;
      // output("WM_MOUSEMOVE", x, y);
      RECT m_rect{};  // 当前m_rect
      Ex_ObjGetRectEx(hObj, &m_rect, 1);

      INT direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      INT position = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE);
      INT minPos = direction == 0 ? m_rect.left : m_rect.top;
      INT maxPos =
          direction == 0 ? (m_rect.right - size) : (m_rect.bottom - size);

      if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) == STATE_DOWN &&
          Ex_ObjGetLong(hObj, SPLITTER_LONG_DRAGGING)) {
        HEXOBJ panel1 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
        HEXOBJ panel2 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);
        // 如果锁定则不允许拖动
        if (Ex_ObjGetLong(hObj, SPLITTER_LONG_LOCK)) {
          // 貌似如果整个分割条组件大小变化了，这里会有问题，下/右面板是否应该变动尺寸？？？

          break;
        }

        // 计算新位置
        INT newPosition = direction == 0 ? x : y;

        // 限制在最小最大值范围内
        if (newPosition < minPos) newPosition = minPos;
        if (newPosition > maxPos) newPosition = maxPos;

        if (newPosition != position) {
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, newPosition);

          if (panel1) {
            Ex_ObjMove(panel1, m_rect.left, m_rect.top,
                       direction == 0 ? newPosition - m_rect.left
                                      : m_rect.right - m_rect.left,
                       direction == 0 ? m_rect.bottom - m_rect.top
                                      : newPosition - m_rect.top,
                       false);
          }

          if (panel2) {
            Ex_ObjMove(panel2,
                       direction == 0 ? newPosition + size : m_rect.left,
                       direction == 0 ? m_rect.top : newPosition + size,
                       direction == 0 ? m_rect.right - m_rect.left -
                                            (newPosition - m_rect.left + size)
                                      : m_rect.right - m_rect.left,
                       direction == 0 ? m_rect.bottom - m_rect.top
                                      : m_rect.bottom - m_rect.top -
                                            (newPosition - m_rect.top + size),
                       false);
          }
          // 设置新的百分比
          auto pos = direction == 0
                         ? newPosition * 100 / (m_rect.right - size)
                         : newPosition * 100 / (m_rect.bottom - size);
          Ex_ObjSetLong(hObj, SPLITTER_LONG_POSITION, pos);
          Ex_ObjInvalidateRect(hObj, NULL);
        }
      } else {
        HCURSOR hCursor = LoadCursor(nullptr, IDC_ARROW);
        if (direction == 0) {
          if (x >= m_rect.left + position &&
              x <= m_rect.left + position + size) {
            // 水平分割条
            hCursor = LoadCursor(NULL, IDC_SIZEWE);
            Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, TRUE);
          }
        } else {
          if (y >= m_rect.top + position && y <= m_rect.top + position + size) {
            // 垂直分割条
            hCursor = LoadCursor(NULL, IDC_SIZENS);
            Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, TRUE);
          }
        }
        SetCursor(hCursor);
      }
    } break;
    case WM_LBUTTONUP: {
      Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, FALSE);
    } break;
    case SPLITTER_GET_DIRECTION: {
      return Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
    } break;
    case SPLITTER_SET_DIRECTION: {
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DIRECTION, wParam);
      Ex_ObjInvalidateRect(hObj, NULL);
    } break;
    case SPLITTER_GET_POSITION: {
      return Ex_ObjGetLong(hObj, SPLITTER_LONG_POSITION);
    } break;
    case SPLITTER_SET_POSITION: {
      RECT m_rect{};  // 当前m_rect
      Ex_ObjGetRectEx(hObj, &m_rect, 1);
      INT direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      int size = Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE);
      INT minPos = direction == 0 ? m_rect.left : m_rect.top;
      INT maxPos =
          direction == 0 ? (m_rect.right - size) : (m_rect.bottom - size);

      INT newPos = (INT)wParam;

      if (newPos < minPos) newPos = minPos;
      if (newPos > maxPos) newPos = maxPos;

      Ex_ObjSetLong(hObj, SPLITTER_LONG_POSITION, newPos);
      Ex_ObjInvalidateRect(hObj, NULL);
    } break;
    case SPLITTER_GET_PANEL: {
      if (wParam == 0)
        return Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
      else if (wParam == 1)
        return Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);
    } break;
    case SPLITTER_SET_PANEL: {
      HEXOBJ panel1 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
      HEXOBJ panel2 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);
      // 已有组件的面板到底允不允许被覆盖？？？？
      assert(!(wParam && panel1));
      assert(!(lParam && panel2));

      if (wParam) {
        if (Ex_ObjGetParent(wParam) != hObj) Ex_ObjSetParent(wParam, hObj);
        Ex_ObjSetLong(hObj, SPLITTER_LONG_PANEL1, (LONG_PTR)wParam);
      }
      if (lParam) {
        if (Ex_ObjGetParent(lParam) != hObj) Ex_ObjSetParent(lParam, hObj);
        Ex_ObjSetLong(hObj, SPLITTER_LONG_PANEL2, (LONG_PTR)lParam);
      }
      Ex_ObjSendMessage(hObj, WM_SIZE, 0, 0);
    } break;
    case WM_PAINT: {
      EX_PAINTSTRUCT ps;
      if (Ex_ObjBeginPaint(hObj, &ps)) {
        _canvas_clear(ps.hCanvas, 0);
        // 绘制分割条
        int size = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE));
        INT position = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION));

        HEXBRUSH hBrush =
            _brush_create(Ex_ObjGetLong(hObj, SPLITTER_LONG_COLOR));
        if (Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION) == 0) {
          // 垂直分割条
          _canvas_fillrect(ps.hCanvas, hBrush, ps.rcPaint.left + position, 0,
                           ps.rcPaint.left + position + size, ps.uHeight);
        } else {
          // 水平分割条
          _canvas_fillrect(ps.hCanvas, hBrush, 0, ps.rcPaint.top + position,
                           ps.uWidth, ps.rcPaint.top + position + size);
        }
        _brush_destroy(hBrush);
        Ex_ObjEndPaint(hObj, &ps);
      }
    } break;
    default:
      break;
  }
  return Ex_ObjDefProc(hWnd, hObj, uMsg, wParam, lParam);
}

// 注册分割条组件
EXATOM SplitterRegister() {
  return Ex_ObjRegister(L"Splitter", OBJECT_STYLE_VISIBLE,
                        OBJECT_STYLE_EX_FOCUSABLE, 0, 10 * sizeof(size_t), NULL,
                        0, SplitterProc);
}
