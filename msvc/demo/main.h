#pragma once
#include <fstream>
#include <iostream>
#include <random>
#include <shlwapi.h>
#include <string>
#include <vector>
#pragma comment(lib, "shlwapi.lib")

#include "ExDUIR_Func.h"
#include "ExDUIR_Interface.h"

#include "test_button.h"
#include "test_label.h"
#include "test_checkbutton.h"
#include "test_edit.h"
#include "test_listbutton.h"
#include "test_custombkg.h"
#include "test_listview.h"
#include "test_groupbox.h"
#include "test_navbutton.h"
#include "test_relative.h"
#include "test_absolute.h"
#include "test_linear.h"
#include "test_flow.h"
#include "test_table.h"
#include "test_combobox.h"
#include "test_ani.h"
#include "test_customredraw.h"
#include "test_messagebox.h"
#include "test_colorbutton.h"
#include "test_treelistview.h"
#include "test_reportlistview.h"
#include "test_iconlistview.h"
#include "test_matrix.h"
#include "test_buttonex.h"
#include "test_editex.h"
#include "test_custommenu.h"
#include "test_event.h"
#include "test_loading.h"
#include "test_sliderbar.h"
#include "test_rotateimgbox.h"
#include "test_dragobj.h"
#include "test_dropobj.h"
#include "test_progressbar.h"
#include "test_nchittest.h"
#include "test_modal.h"
#include "test_colorpicker.h"
#include "test_datebox.h"
#include "test_titlebar.h"
#include "test_calendar.h"
#include "test_chromium.h"
#include "test_scorebtn.h"
#include "test_carousel.h"
#include "test_templatelistview.h"
#include "test_drawingboard.h"
#include "test_palette.h"
#include "test_propertygrid.h"
#include "test_nativewindow.h"
#include "test_fullscreen.h"
#include "test_svg_and_font.h"
#include "test_rollmenu.h"
#include "test_tray.h"
#include "test_mask.h"
#include "test_tagging.h"
#include "test_effect.h"
#include "test_path_and_rgn.h"
#include "test_vlcplay.h"
#include "test_respack.h"
#include "test_circleprogressbar.h"
#include "test_waveprogressbar.h"
#include "test_linechart.h"
#include "test_chatbox.h"
#include "test_flowchart.h"
#include "test_splitter.h"
#include "test_d3d.h"

INT CalculateContainerHeight(INT buttonsNumber, INT containerWidth, INT buttonWidth,
                             INT buttonHeight, INT horizontalSpacing, INT verticalSpacing);
LRESULT CALLBACK OnPageScrollMsg(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                 LRESULT* lpResult);
LRESULT CALLBACK OnMainWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam,
                                  LRESULT* lpResult);
LRESULT CALLBACK button_click(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
INT              PageScrollDefaultProc(HEXOBJ hObj, DWORD nBar, WPARAM wParam, INT nLine, INT nPage,
                                       BOOL fRedraw);
void             test_exdui();
