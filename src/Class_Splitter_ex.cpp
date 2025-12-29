#pragma once
#include "stdafx.h"

// 辅助函数：是否是分割条组件
inline bool IsSplitter(HEXOBJ hObj) {
  EX_CLASSINFO pClassInfo{};
  Ex_ObjGetClassInfo(hObj, &pClassInfo);
  return pClassInfo.atomName == Ex_Atom(L"Splitter");
}

// 辅助函数：递归锁定逻辑
void LockPanel(HEXOBJ panel) {
  bool lockPanel2 = true;
  if (IsSplitter(panel)) {
    int oldLock = Ex_ObjGetLong(panel, SPLITTER_LONG_LOCK);
    if (!(oldLock & 2)) Ex_ObjSetLong(panel, SPLITTER_LONG_LOCK, oldLock | 2);

    int fixedPanel = Ex_ObjGetLong(panel, SPLITTER_LONG_FIXEDPANEL);
    if (fixedPanel == 0) return;
    if (fixedPanel == 1)
      lockPanel2 = false;
    else if (fixedPanel == 2)
      lockPanel2 = true;

    int direction = Ex_ObjGetLong(panel, SPLITTER_LONG_DIRECTION);
    int newPosition = Ex_ObjGetLong(panel, SPLITTER_LONG_CURPOSITION);
    int position = Ex_ObjGetLong(panel, SPLITTER_LONG_POSITION);
    int size = Ex_Scale(Ex_ObjGetLong(panel, SPLITTER_LONG_SIZE));
    // 计算锁定面板的宽度和高度值
    HEXOBJ subPanel = (HEXOBJ)Ex_ObjGetLong(
        panel, lockPanel2 ? SPLITTER_LONG_PANEL2 : SPLITTER_LONG_PANEL1);
    if (subPanel) {
      RECT subRect{};
      Ex_ObjGetRectEx(subPanel, &subRect, 1);
      Ex_ObjSetLong(
          panel, SPLITTER_LONG_LOCKSIZE,
          MAKELONG(subRect.right - subRect.left, subRect.bottom - subRect.top));
    }
    // 如果没有要锁定的子面板，则按照当前分割条位置计算锁定面板的宽度和高度值
    else {
      RECT m_rect{};
      Ex_ObjGetRectEx(panel, &m_rect, 1);
      int width = m_rect.right - m_rect.left;
      int height = m_rect.bottom - m_rect.top;
      if (direction == 0) {
        // 垂直分割条
        width = lockPanel2 ? ((m_rect.right - m_rect.left) - newPosition - size)
                           : newPosition;
      } else {
        // 水平分割条
        height = lockPanel2
                     ? ((m_rect.bottom - m_rect.top) - newPosition - size)
                     : newPosition;
      }
      Ex_ObjSetLong(panel, SPLITTER_LONG_LOCKSIZE, MAKELONG(width, height));
    }
  }
}

// 分割条组件回调函数
LRESULT CALLBACK _splitter_proc(HWND hWnd, HEXOBJ hObj, INT uMsg,
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
      Ex_ObjSetLong(hObj, SPLITTER_LONG_LOCKSIZE,
                    0);  // 锁定面板2的宽度和高度值为0
      Ex_ObjSetLong(hObj, SPLITTER_LONG_FIXEDPANEL, 0);  // 默认不固定面板
    } break;
    case WM_SIZE: {
      // output("WM_SIZE", hObj, wParam, lParam);
      //  调整大小时，更新分割条位置
      RECT m_rect{};
      Ex_ObjGetRectEx(hObj, &m_rect, 1);
      int direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      int position = Ex_ObjGetLong(hObj, SPLITTER_LONG_POSITION);
      int newPosition = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE));
      HEXOBJ panel1 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
      HEXOBJ panel2 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);

      // 锁定逻辑
      int lockState = Ex_ObjGetLong(hObj, SPLITTER_LONG_LOCK);
      if (lParam && lockState & 2) {
        int lockSize = Ex_ObjGetLong(hObj, SPLITTER_LONG_LOCKSIZE);
        int lockWidth = LOWORD(lockSize);
        int lockHeight = HIWORD(lockSize);
        int fixedPanel = Ex_ObjGetLong(hObj, SPLITTER_LONG_FIXEDPANEL);

        int left = 0, top = 0, width = 0, height = 0;
        if (panel1) {
          if (fixedPanel == 0) {
            newPosition = direction == 0
                              ? (m_rect.right - m_rect.left) * position / 100
                              : (m_rect.bottom - m_rect.top) * position / 100;
            left = m_rect.left;
            top = m_rect.top;
            width = direction == 0 ? newPosition - m_rect.left
                                   : m_rect.right - m_rect.left;
            height = direction == 0 ? m_rect.bottom - m_rect.top
                                    : newPosition - m_rect.top;
          } else if (fixedPanel == 1) {
            newPosition = direction == 0 ? lockWidth : lockHeight;
            left = m_rect.left;
            top = m_rect.top;
            width = direction == 0 ? lockWidth : m_rect.right - m_rect.left;
            height = direction == 0 ? m_rect.bottom - m_rect.top : lockHeight;
          } else if (fixedPanel == 2) {
            newPosition = direction == 0 ? m_rect.right - lockWidth - size
                                         : m_rect.bottom - lockHeight - size;
            left = m_rect.left;
            top = m_rect.top;
            width = direction == 0
                        ? m_rect.right - m_rect.left - lockWidth - size
                        : m_rect.right - m_rect.left;
            height = direction == 0
                         ? m_rect.bottom - m_rect.top
                         : m_rect.bottom - m_rect.top - lockHeight - size;
          }
          Ex_ObjMove(panel1, left, top, width, height, false);
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, newPosition);
        }

        if (panel2) {
          if (fixedPanel == 0) {
            newPosition = direction == 0
                              ? (m_rect.right - m_rect.left) * position / 100
                              : (m_rect.bottom - m_rect.top) * position / 100;
            left = direction == 0 ? newPosition + size : m_rect.left;
            top = direction == 0 ? m_rect.top : newPosition + size;
            width = direction == 0 ? m_rect.right - m_rect.left -
                                         (newPosition - m_rect.left + size)
                                   : m_rect.right - m_rect.left;
            height = direction == 0 ? m_rect.bottom - m_rect.top
                                    : m_rect.bottom - m_rect.top -
                                          (newPosition - m_rect.top + size);
          } else if (fixedPanel == 1) {
            newPosition = direction == 0 ? lockWidth : lockHeight;
            left = direction == 0 ? lockWidth + size : m_rect.left;
            top = direction == 0 ? m_rect.top : lockHeight + size;
            width = direction == 0 ? m_rect.right - lockWidth - size
                                   : m_rect.right - m_rect.left;
            height = direction == 0 ? m_rect.bottom - m_rect.top
                                    : m_rect.bottom - lockHeight - size;
          } else if (fixedPanel == 2) {
            newPosition = direction == 0 ? m_rect.right - lockWidth - size
                                         : m_rect.bottom - lockHeight - size;
            left = direction == 0 ? m_rect.right - lockWidth : m_rect.left;
            top = direction == 0 ? m_rect.top : m_rect.bottom - lockHeight;
            width = direction == 0 ? lockWidth : m_rect.right - m_rect.left;
            height = direction == 0 ? m_rect.bottom - m_rect.top : lockHeight;
          }
          Ex_ObjMove(panel2, left, top, width, height, false);
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, newPosition);
        }
        // output("Second Splitter Adjusted", hObj, wParam, lParam);
        break;
      }
      // output("Fist Splitter Adjusted", hObj, wParam, lParam);
      //  计算第一次位置
      newPosition = direction == 0
                        ? (m_rect.right - m_rect.left) * position / 100
                        : (m_rect.bottom - m_rect.top) * position / 100;
    
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

      if (lParam && !(lockState & 2)) {
        LockPanel(hObj);
      }
      Ex_ObjInvalidateRect(hObj, NULL);
    } break;
    case WM_SETCURSOR: {
      if (Ex_ObjGetLong(hObj, SPLITTER_LONG_DRAGGING)) {
        int direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
        SetCursor(direction == 0 ? LoadCursor(NULL, IDC_SIZEWE)
                                 : LoadCursor(NULL, IDC_SIZENS));
        return 1;
      }
    } break;
    case WM_LBUTTONDOWN: {
      Ex_ObjSetUIState(hObj, STATE_DOWN, FALSE, 0, FALSE);
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, FALSE);

      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);

      RECT m_rect{};  // 当前m_rect
      Ex_ObjGetRectEx(hObj, &m_rect, 1);

      int direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      int position = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE));

      bool isDragging = false;
      if (direction == 0) {
        if (x >= m_rect.left + position && x <= m_rect.left + position + size) {
          // 水平分割条
          isDragging = true;
        }
      } else {
        if (y >= m_rect.top + position && y <= m_rect.top + position + size) {
          // 垂直分割条
          isDragging = true;
        }
      }
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, isDragging);
    } break;
    case WM_MOUSEMOVE: {
      // 如果锁定则不允许拖动
      if (Ex_ObjGetLong(hObj, SPLITTER_LONG_LOCK) & 1) {
        break;
      }

      int x = GET_X_LPARAM(lParam);
      int y = GET_Y_LPARAM(lParam);
      // output("WM_MOUSEMOVE", x, y);
      RECT m_rect{};  // 当前m_rect
      Ex_ObjGetRectEx(hObj, &m_rect, 1);

      int direction = Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
      int position = Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION);
      int size = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE));
      int minPos = direction == 0 ? m_rect.left : m_rect.top;
      int maxPos =
          direction == 0 ? (m_rect.right - size) : (m_rect.bottom - size);

      if ((Ex_ObjGetUIState(hObj) & STATE_DOWN) == STATE_DOWN &&
          Ex_ObjGetLong(hObj, SPLITTER_LONG_DRAGGING)) {
        // 计算新位置
        int newPosition = direction == 0 ? x : y;

        // 限制在最小最大值范围内
        if (newPosition < minPos) newPosition = minPos;
        if (newPosition > maxPos) newPosition = maxPos;

        if (newPosition != position) {
          Ex_ObjSetLong(hObj, SPLITTER_LONG_CURPOSITION, newPosition);
          HEXOBJ panel1 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
          HEXOBJ panel2 = (HEXOBJ)Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);

          if (panel1) {
              Ex_ObjMove(panel1, m_rect.left, m_rect.top,
                             direction == 0 ? newPosition - m_rect.left
                                            : m_rect.right - m_rect.left,
                             direction == 0 ? m_rect.bottom - m_rect.top
                                            : newPosition - m_rect.top,
                             false);
          }

          if (panel2) {
              Ex_ObjMove(
                panel2, direction == 0 ? newPosition + size : m_rect.left,
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
        bool isDragging = false;
        if (direction == 0) {
          if (x >= m_rect.left + position &&
              x <= m_rect.left + position + size) {
            // 水平分割条
            isDragging = true;
          }
        } else {
          if (y >= m_rect.top + position && y <= m_rect.top + position + size) {
            // 垂直分割条
            isDragging = true;
          }
        }
        Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, isDragging);
        Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
      }
    } break;
    case WM_LBUTTONUP: {
      LockPanel(hObj);
      Ex_ObjSetUIState(hObj, STATE_DOWN, TRUE, 0, FALSE);
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DRAGGING, FALSE);
    } break;
    case SPLITTER_MESSAGE_GET_DIRECTION: {
      return Ex_ObjGetLong(hObj, SPLITTER_LONG_DIRECTION);
    } break;
    case SPLITTER_MESSAGE_SET_DIRECTION: {
      Ex_ObjSetLong(hObj, SPLITTER_LONG_DIRECTION, wParam);
      Ex_ObjInvalidateRect(hObj, NULL);
    } break;
    case SPLITTER_MESSAGE_GET_POSITION: {
      return Ex_ObjGetLong(hObj, SPLITTER_LONG_POSITION);
    } break;
    case SPLITTER_MESSAGE_SET_POSITION: {
      Ex_ObjSetLong(hObj, SPLITTER_LONG_POSITION, wParam);
      Ex_ObjInvalidateRect(hObj, NULL);
    } break;
    case SPLITTER_MESSAGE_GET_PANEL: {
      if (wParam == 0)
        return Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL1);
      else if (wParam == 1)
        return Ex_ObjGetLong(hObj, SPLITTER_LONG_PANEL2);
    } break;
    case SPLITTER_MESSAGE_SET_PANEL: {
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
      Ex_ObjSendMessage(hObj, WM_SIZE, 1, 0);
    } break;
    case WM_PAINT: {
      EX_PAINTSTRUCT ps;
      if (Ex_ObjBeginPaint(hObj, &ps)) {
        _canvas_clear(ps.hCanvas, 0);
        // 绘制分割条
        int size = Ex_Scale(Ex_ObjGetLong(hObj, SPLITTER_LONG_SIZE));
        int position = (Ex_ObjGetLong(hObj, SPLITTER_LONG_CURPOSITION));

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
EXATOM _splitter_register() {
  return Ex_ObjRegister(L"Splitter", OBJECT_STYLE_VISIBLE,
                        OBJECT_STYLE_EX_FOCUSABLE, 0, 15 * sizeof(size_t), NULL,
                        0, _splitter_proc);
}
