#pragma once
#include <Windows.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <olectl.h>
#include <sstream>
#include <string>
#include <vector>
#include <bcrypt.h>
#include <ocidl.h>
#include <richedit.h>
#include <richole.h>
#include <textserv.h>

#pragma comment(lib, "OleAut32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")

#include <d2d1.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <dwrite.h>
#include <wincodec.h>
#include <Initguid.h>
#include <d2d1effectauthor.h>
#include <d2d1effecthelpers.h>
#include <d3dcompiler.h>
#pragma comment(lib, "bcrypt.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "Dwrite.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "windowscodecs.lib")

#pragma region 警告禁用
#pragma warning(disable : 4005)
#pragma warning(disable : 4308)
// 禁用警告：类型转换可能导致数据/精度丢失
#pragma warning(disable : 4244)
#pragma warning(disable : 4267)
#pragma warning(disable : 4838)
#pragma warning(disable : 4996)
#pragma warning(disable : 4311)
#pragma warning(disable : 4302)
#pragma warning(disable : 4312)
#pragma warning(disable : 4309)
#pragma warning(disable : 4018)

// 禁用警告：返回值被忽略
#pragma warning(disable : 6031)
// 禁用警告：行为更改: 使用 () 形式的初始值设定项构造的 POD
// 类型的对象将被默认初始化
#pragma warning(disable : 4345)
// 禁用警告：忽略未引用的参数，因为它们在实现回调时非常常见。
#pragma warning(disable : 4100)

#pragma endregion

// 断言
#ifndef _DEBUG
#ifndef EX_ASSERT
#define EX_ASSERT(expr, msg, ...) ((void)0)
#endif
#else
#ifndef EX_ASSERT
#define EX_ASSERT(expr, msg, ...)                                              \
  (void)((!!(expr)) ||                                                         \
         (1 != _CrtDbgReportW(_CRT_ASSERT, _CRT_WIDE(__FILE__), __LINE__,      \
                              NULL, msg, __VA_ARGS__)) ||                      \
         (_CrtDbgBreak(), 0))
#endif
#endif

#include "ExDUIR_Func.h"
#include "ExDUIR_Interface.h"
#include "ExDUIR_Struct.h"

#include "svgrender/SVGDocument.h"
#include "CustomFont.h"
#include "help_ex.h"
#include "Array_ex.h"
#include "Geometry_ex.h"
#include "Canvas_ex.h"

#include "DirectX_ex.h"
#include "DropTarget_ex.h"
#include "Easing_ex.h"
#include "Font_ex.h"
#include "Format_ex.h"
#include "HandelTable_ex.h"
#include "HashTable_ex.h"
#include "Hook_ex.h"
#include "Image_ex.h"
#include "Layout_ex.h"
#include "Matrix_ex.h"
#include "MemPool_ex.h"
#include "Object_ex.h"
#include "Path_ex.h"
#include "Region_ex.h"
#include "Resource_ex.h"
#include "StrokeStyle_ex.h"
#include "Theme_ex.h"
#include "Thread_ex.h"
#include "Wnd_ex.h"


#include "Class_ButtonEx_ex.h"
#include "Class_Button_Item_ex.h"
#include "Class_Calendar_ex.h"
#include "Class_Carousel_ex.h"
#include "Class_CefBrowser_ex.h"
#include "Class_CheckBox_ex.h"
#include "Class_CheckButtonEx_ex.h"
#include "Class_ColorPicker_ex.h"
#include "Class_ComboBox_ex.h"
#include "Class_DateBox_ex.h"
#include "Class_DrawingBoard_ex.h"
#include "Class_EditEx_ex.h"
#include "Class_Edit_ex.h"
#include "Class_GroupBox_ex.h"
#include "Class_IconListView_ex.h"
#include "Class_ListButton_ex.h"
#include "Class_ListView_ex.h"
#include "Class_MosaicLoading_ex.h"
#include "Class_NavButton_ex.h"
#include "Class_Palette_ex.h"
#include "Class_ProgressBar_ex.h"
#include "Class_PropertyGrid_ex.h"
#include "Class_RadioButtonEx_ex.h"
#include "Class_ReportListView_ex.h"
#include "Class_RollMenu.h"
#include "Class_RotateImageBox_ex.h"
#include "Class_ScoreButton_ex.h"
#include "Class_SliderBar_ex.h"
#include "Class_Static_SysLink_ex.h"
#include "Class_Switch_ex.h"
#include "Class_SysButton_Page_ex.h"
#include "Class_TaggingBoard.h"
#include "Class_TitleBar_ex.h"
#include "Class_TreeView_ex.h"
#include "Class_Win10Loading_ex.h"
#include "Class_scrollbar_ex.h"
#include "Class_CircleProgressBar_ex.h"
#include "Class_WaveProgressBar_ex.h"
#include "Class_LineChart_ex.h"
#include "Class_ChatBox_ex.h"
#include "Class_FlowChart_ex.h"
#include "Class_FlowScrollView_ex.h"
#include "Class_Grid_ex.h"
#include "Class_Splitter_ex.h"
#include "Class_SysMenu_ex.h"

#ifdef WEB_VIEW2
#include "Class_WebView_ex.h"
#pragma comment(lib, "WebView2LoaderStatic.lib")
#endif

#ifdef VCL_PLAYER
#include "Class_VLCPlayer_ex.h"
#pragma comment(lib, "libvlc.lib")
#pragma comment(lib, "libvlccore.lib")
#endif

