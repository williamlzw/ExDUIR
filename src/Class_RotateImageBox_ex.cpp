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
	switch (uMsg)
	{
		/*创建时初始化控件属性*/
	case WM_CREATE:
	{
		if ((Ex_ObjGetLong(hObj, EOL_STYLE) & ERIBS_ROTATE) == ERIBS_ROTATE)
		{
			/* 创建时启用 绘图中消息*/
			Ex_ObjEnablePaintingMsg(hObj, TRUE);
			/* 开启时钟*/
			Ex_ObjSetTimer(hObj, 50);
		}
		break;
	}

	case WM_EX_PAINTING:
	{
		EX_PAINTSTRUCT2* ps = (EX_PAINTSTRUCT2*)lParam;

		switch (wParam)
		{
			/* 当控件开始绘制时*/
		case EPP_BEGIN:
		{
			/*创建矩阵，并旋转一个角度*/
			HEXMATRIX	mx = _matrix_create();
			_matrix_translate(mx, (FLOAT)ps->width / 2, (FLOAT)ps->height / 2);
			_matrix_rotate(mx, (FLOAT)Ex_ObjGetLong(hObj, EOL_USERDATA));
			_matrix_translate(mx, (FLOAT)ps->width / (-2), (FLOAT)ps->height / (-2));
			/* 设置到当前画布上*/
			_canvas_settransform(ps->hCanvas, mx);
			/* 设置后就可以释放了*/
			_matrix_destroy(mx);
			break;
		}
		/* 绘制背景后*/
		case EPP_BKG:
		{
			/* 还原当前画布的变换(此时设置仅旋转背景)*/
			_canvas_settransform(ps->hCanvas, 0);
			break;
		}
		/*绘制结束后*/
		case EPP_END:
		{
			/* 还原当前画布的变换(此时设置旋转整个控件内容)*/
			// _canvas_settransform (ps.hCanvas, 0);
			break;
		}
		default:
			break;
		}

		break;
	}
	case WM_TIMER:
	{
		Ex_ObjSetLong(hObj, EOL_USERDATA, (Ex_ObjGetLong(hObj, EOL_USERDATA) + 1) % 360);/* 每50ms增加1°*/
		Ex_ObjInvalidateRect(hObj, 0);
		break;
	}
	case WM_MOUSEHOVER:
	{

		/*移入则停止旋转*/
		Ex_ObjKillTimer(hObj);
		Ex_ObjSetUIState(hObj, STATE_HOVER, FALSE, 0, FALSE);
		break;
	}
	case  WM_MOUSELEAVE:
	{

		/*移出则恢复旋转*/
		Ex_ObjSetUIState(hObj, STATE_HOVER, TRUE, 0, FALSE);
		Ex_ObjSetTimer(hObj, 50);
		break;
	}
	default:
		break;
	}
	return Ex_ObjCallProc(m_pfnStaticProc, hWnd, hObj, uMsg, wParam, lParam);
}

