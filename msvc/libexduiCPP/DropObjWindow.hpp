#include <vector>
#include "ExDUIRCPP.hpp"


class DropObjWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateDropObjWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 300, 300, L"测试接收拖曳信息", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_static = ExStatic(m_skin, 25, 25, 250, 250, L"拖拽文本、文件到这里", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_BORDER, OBJECT_STYLE_EX_DRAGDROP | OBJECT_STYLE_EX_ACCEPTFILES, -1, 0, 0, 0, OnDropObjDataMsgProc);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		ExControl obj = ExControl(hObj);
		if (uMsg == WM_EX_DROP) //先触发本消息
		{
			EX_DROPINFO di{ 0 };
			RtlMoveMemory(&di, (LPVOID)lParam, sizeof(EX_DROPINFO));
			if (obj.CheckDropFormat(di.pDataObject, CF_UNICODETEXT) || obj.CheckDropFormat(di.pDataObject, CF_TEXT))//判断是不是文本
			{
				INT len = obj.GetDropStringLen(di.pDataObject);
				if (len > 0)
				{
					std::wstring str;
					str.resize(len);
					obj.GetDropString(di.pDataObject, str, len * 2);
					OUTPUTW(L"接收到文本拖拽:", str);
					*lpResult = DROPEFFECT_COPY; //按复制模式处理
					return 1;
				}
			}
		}
		else if (uMsg == WM_DROPFILES) //若上面未处理,且控件拥有#OBJECT_STYLE_EX_ACCEPTFILES风格,则继续触发本消息
		{
			UINT fileNumber = DragQueryFileW((HDROP)wParam, 0xFFFFFFFF, NULL, 0);
			for (UINT index = 0; index < fileNumber; index++)
			{
				UINT fileNameLength = DragQueryFileW((HDROP)wParam, index, NULL, 0);
				if (fileNameLength)
				{
					std::wstring fileName;
					fileName.resize(fileNameLength);
					DragQueryFileW((HDROP)wParam, index, (LPWSTR)fileName.data(), fileNameLength * 2);
					OUTPUTW(L"接收到文件拖拽:", fileName);
				}
			}
			*lpResult = DROPEFFECT_LINK; //按建立快捷方式处理
			return 1;
		}
		return 0;
	}

	static DropObjWindow& GetInstance()
	{
		static DropObjWindow ret;
		return ret;
	}
};