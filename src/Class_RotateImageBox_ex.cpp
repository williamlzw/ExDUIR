#include "Class_RotateImageBox_ex.h"

ClsPROC m_pfnStaticProc;/*控件基类的消息回调函数*/

void _rotateimagebox_register()
{
	EX_CLASSINFO	pClsInfoStatic;

	/* 超类化(从现有控件派生)过程
	 * 超类化的好处是可以直接利用现有控件，省去从头编写控件的时间，提高扩展效率*/

	 /* 1、获取父类控件信息*/
	WCHAR	oldwzCls[] = L"Static";
	Ex_ObjGetClassInfoEx(oldwzCls, &pClsInfoStatic);

	/* 2、保存父类控件回调函数指针*/
	m_pfnStaticProc = pClsInfoStatic.pfnClsProc;

	/* 3、注册新控件*/
	WCHAR	newwzCls[] = L"RotateImageBox";
	Ex_ObjRegister(newwzCls, pClsInfoStatic.dwStyle, pClsInfoStatic.dwStyleEx, pClsInfoStatic.dwTextFormat, NULL, pClsInfoStatic.hCursor, pClsInfoStatic.dwFlags, _rotateimagebox_proc);
}


LRESULT CALLBACK _rotateimagebox_proc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (uMsg == WM_CREATE)
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ERIBS_ROTATE) == ERIBS_ROTATE)
		{
			/* 创建时启用 绘图中消息*/
			Ex_ObjEnablePaintingMsg(hObj, TRUE);
			/* 开启时钟*/
			Ex_ObjSetTimer(hObj, 50);
		}
	}
	else if (uMsg == WM_EX_PAINTING)
	{
		EX_PAINTSTRUCT* ps = (EX_PAINTSTRUCT*)lParam;
		if (wParam == EPP_BEGIN)/* 当控件开始绘制时*/
		{
			/*创建矩阵，并旋转一个角度*/
			HEXMATRIX	mx = _matrix_create();
			_matrix_translate(mx, (FLOAT)ps->uWidth / 2, (FLOAT)ps->uHeight / 2);
			_matrix_rotate(mx, (FLOAT)Ex_ObjGetLong(hObj, EOL_USERDATA));
			_matrix_translate(mx, (FLOAT)ps->uWidth / (-2), (FLOAT)ps->uHeight / (-2));
			/* 设置到当前画布上*/
			_canvas_settransform(ps->hCanvas, mx);
			/* 设置后就可以释放了*/
			_matrix_destroy(mx);

		}
		else if (wParam == EPP_BKG)/* 绘制背景后*/
		{
			/* 还原当前画布的变换(此时设置仅旋转背景)*/
			_canvas_settransform(ps->hCanvas, 0);
		}

		else if (wParam == EPP_END)/*绘制结束后*/
		{
			/* 还原当前画布的变换(此时设置旋转整个控件内容)*/
			// _canvas_settransform (ps.hCanvas, 0);
		}
	}
	else if (uMsg == WM_TIMER)
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, (Ex_ObjGetLong(hObj, EOL_USERDATA) + 1) % 360);/* 每50ms增加1°*/
		Ex_ObjInvalidateRect(hObj, 0);
	}
	else if (uMsg == WM_MOUSEHOVER)
	{
		/*移入则停止旋转*/
		Ex_ObjKillTimer(hObj);
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, FALSE);
	}
	else if (uMsg == WM_MOUSELEAVE)
	{
		/*移出则恢复旋转*/
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, FALSE);
		Ex_ObjSetTimer(hObj, 50);
	}
	return Ex_ObjCallProc(m_pfnStaticProc, hWnd, hObj, uMsg, wParam, lParam);
}
