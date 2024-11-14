#include <vector>
#include "ExDUIRCPP.hpp"

using namespace ExDUIR::FrameWorks::Control;
using namespace ExDUIR::FrameWorks::Layout;

class EditWindow
{
private:
	ExSkin m_skin;
	ExEdit m_edit1;
	ExEdit m_edit2;
	ExEdit m_edit3;
	ExEdit m_edit4;
	ExEdit m_edit5;

	ExEdit m_edit6;
	ExEdit m_edit7;

	ExPage m_page;
	ExFlowLayout m_layout;
	std::vector<ExButton> m_buttons;

public:
	void CreateEditWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 850, 350, L"测试编辑框", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(120, 120, 120, 255));

		m_edit1 = ExEdit(m_skin, 10, 30, 150, 30, L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_CUSTOMDRAW, DT_VCENTER);
		m_edit1.SetCueBanner(L"背景图片编辑框", ExARGB(0, 0, 0, 100));
		m_edit1.SetBackgroundImage(L"../test/res/editbkg.jpg");

		m_edit2 = ExEdit(m_skin, 10, 70, 150, 30, L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_USEPASSWORD, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, DT_VCENTER);
		m_edit2.SetCueBanner(L"密码输入编辑框", ExARGB(0, 0, 0, 100));
		
		m_edit3 = ExEdit(m_skin, 10, 110, 150, 30, L"", OBJECT_STYLE_VISIBLE | EDIT_STYLE_NUMERICINPUT, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, DT_VCENTER);
		m_edit3.SetCueBanner(L"数值输入编辑框", ExARGB(0, 0, 0, 100));

		m_edit4 = ExEdit(m_skin, 10, 150, 150, 30, L"只读编辑框", OBJECT_STYLE_VISIBLE | EDIT_STYLE_READONLY, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED, DT_VCENTER);
		
		m_edit5 = ExEdit(m_skin, 10, 190, 150, 30, L"透明圆角编辑框", OBJECT_STYLE_VISIBLE | EDIT_STYLE_HIDESELECTION, OBJECT_STYLE_EX_FOCUSABLE | OBJECT_STYLE_EX_COMPOSITED | OBJECT_STYLE_EX_TABSTOP | OBJECT_STYLE_EX_CUSTOMDRAW, DT_VCENTER);
		m_edit5.SetFont(L"微软雅黑", 18, FONT_STYLE_UNDERLINE | FONT_STYLE_ITALIC);
		m_edit5.SetColorBackground(ExARGB(200, 120, 130, 100));
		m_edit5.SetColorTextNormal(ExRGB2ARGB(16872215, 100));
		m_edit5.SetRadius(10, 10, 10, 0);

		m_edit6 = ExEdit(m_skin, 10, 245, 150, 100, L"测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\r\n测试多行编辑框\n", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT | DT_TOP);
		m_edit6.AddLine(L"新添加一行");

		m_edit7 = ExEdit(m_skin, 180, 30, 300, 300, L"", OBJECT_STYLE_VISIBLE | OBJECT_STYLE_VSCROLL | OBJECT_STYLE_HSCROLL | EDIT_STYLE_RICHTEXT | EDIT_STYLE_PARSEURL | EDIT_STYLE_ALLOWTAB | EDIT_STYLE_NEWLINE, OBJECT_STYLE_EX_FOCUSABLE, DT_LEFT | DT_TOP, 101);
		m_edit7.LoadRtf(L"../test/res/test.rtf");
		m_edit7.HandleEvent(EDIT_EVENT_SELCHANGE, OnEditNotifyEvent);
		m_edit7.HandleEvent(EDIT_EVENT_LINK, OnEditNotifyEvent);
		m_edit7.AddLine(L"\r\n");
		m_edit7.AddBitmap(L"../test/res/rotateimgbox.jpg", 200, 200);

		std::vector<std::wstring> buttonTitles = { L"全选", L"取消选择", L"置选择", L"左对齐 ", L"居中对齐",
		L"右对齐", L"首行缩进 ", L"右侧缩进", L"非首行缩进", L"项目符", L"文本红色 ", L"加粗", L"倾斜",
		L"下划线", L"删除线", L"超链接", L"幼圆30", L"撤销 ", L"重做", L"复制", L"剪切", L"粘贴",
		L"删除", L"取行数 ", L"寻找文本", L"替换文本" };
		m_buttons.resize(buttonTitles.size());
		m_page = ExPage(m_skin, 500, 30, 350, 300);

		m_layout = ExFlowLayout(m_page);
		BOOL bWrapLine;
		for (int i = 0; i < buttonTitles.size(); i++)
		{
			bWrapLine = buttonTitles[i].find_last_of(' ') == buttonTitles[i].length() - 1;
			std::wstring title;
			if (bWrapLine)
			{
				title = buttonTitles[i].substr(0, buttonTitles[i].length() - 1);
			}
			else
			{
				title = buttonTitles[i];
			}
			m_buttons[i] = ExButton(m_page, 10, 50, 100, 100, title, -1, OBJECT_STYLE_EX_AUTOSIZE, -1, 201 + i);
			m_layout.SetSubPropMarginRight(m_buttons[i], 5);
			m_layout.SetSubPropMarginBottom(m_buttons[i], 5);
			if (bWrapLine)
			{
				m_layout.SetSubPropNewLine(m_buttons[i], 1);
			}
			auto ret = m_buttons[i].HandleEvent(NM_CLICK, OnEditButtonEvent);
		}
		m_layout.BindObj(m_page, TRUE);

		m_skin.Show();
	}

	static LRESULT CALLBACK OnEditButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		switch (nID)
		{
			case 201:
			{
				//全选
				EditWindow::GetInstance().m_edit7.SelectAll();
				break;
			}
			case 202:
			{
				//取消选择
				EditWindow::GetInstance().m_edit7.CancelSelect();
				break;
			}
			case 203:
			{
				//置选择
				EditWindow::GetInstance().m_edit7.Select(1, 15);
				break;
			}
			case 204:
			{
				//左对齐
				EditWindow::GetInstance().m_edit7.AlignLeft();
				break;
			}
			case 205:
			{
				//居中对齐
				EditWindow::GetInstance().m_edit7.AlignCenter();
				break;
			}
			case 206:
			{
				//右对齐
				EditWindow::GetInstance().m_edit7.AlignRight();
				break;
			}
			case 207:
			{
				//首行缩进
				EditWindow::GetInstance().m_edit7.SetSelParStartIndentation(20);
				break;
			}
			case 208:
			{
				//右侧缩进
				EditWindow::GetInstance().m_edit7.SetSelParRightIndentation(20);
				break;
			}
			case 209:
			{
				//非首行缩进
				EditWindow::GetInstance().m_edit7.SetSelParOffset(50);
				break;
			}
			case 210:
			{
				//段落项目符号
				EditWindow::GetInstance().m_edit7.SetSelParNumbering(EDIT_PARAGRAPHFSYMBOL_LCROMAN);
				break;
			}
			case 211:
			{
				//文本颜色
				EditWindow::GetInstance().m_edit7.SetSelCharColor(ExRGB2ARGB(16711680, 255));
				break;
			}
			case 212:
			{
				//加粗
				EditWindow::GetInstance().m_edit7.SetSelCharBold();
				break;
			}
			case 213:
			{
				//倾斜
				EditWindow::GetInstance().m_edit7.SetSelCharItalic();
				break;
			}
			case 214:
			{
				//下划线
				EditWindow::GetInstance().m_edit7.SetSelCharUnderLine();
				break;
			}
			case 215:
			{
				//删除线
				EditWindow::GetInstance().m_edit7.SetSelCharStrikeOut();
				break;
			}
			case 216:
			{
				//超链接
				EditWindow::GetInstance().m_edit7.SetSelCharLink();
				break;
			}
			case 217:
			{
				//幼圆30
				EditWindow::GetInstance().m_edit7.SetSelCharFont(L"幼圆", 30);
				break;
			}
			case 218:
			{
				//撤销
				EditWindow::GetInstance().m_edit7.Undo();
				break;
			}
			case 219:
			{
				//重做
				EditWindow::GetInstance().m_edit7.Redo();
				break;
			}
			case 220:
			{
				//复制
				EditWindow::GetInstance().m_edit7.Copy();
				break;
			}
			case 221:
			{
				//剪切
				EditWindow::GetInstance().m_edit7.Cut();
				break;
			}
			case 222:
			{
				//粘贴
				EditWindow::GetInstance().m_edit7.Paste();
				break;
			}
			case 223:
			{
				//删除
				EditWindow::GetInstance().m_edit7.Clear();
				break;
			}
			case 224:
			{
				//取行数
				OUTPUTW(L"编辑框行数:", EditWindow::GetInstance().m_edit7.GetLineCount());
				break;
			}
			case 225:
			{
				//寻找文本跳转
				EditWindow::GetInstance().m_edit7.FindEditText(L"a");
				break;
			}
			case 226:
			{
				//替换选择的文本
				EditWindow::GetInstance().m_edit7.ReplaceTextLine(L"选中替换为这个");
				break;
			}
			default:
				break;
		}
		return 0;
	}

	static LRESULT CALLBACK OnEditNotifyEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam)
	{
		if (nCode == EDIT_EVENT_SELCHANGE)
		{
			OUTPUTW(L"选中区域改变:", ((EX_SELCHANGE*)lParam)->chrg.cpMin, ((EX_SELCHANGE*)lParam)->chrg.cpMax);
		}
		else if (nCode == EDIT_EVENT_LINK)
		{
			if (((EX_ENLINK*)lParam)->msg == WM_LBUTTONDOWN)
			{
				ExEdit obj = ExEdit(hObj);
				EX_TEXTRANGE TextRange;
				TextRange.chrg = ((EX_ENLINK*)lParam)->chrg;
				TextRange.pwzText = (LPCWSTR)Ex_AllocBuffer((TextRange.chrg.cpMax - TextRange.chrg.cpMin + 2) * 2);
				obj.GetTextRange(TextRange);
				OUTPUTW(L"链接被按下:", TextRange.chrg.cpMax, TextRange.chrg.cpMin, TextRange.pwzText);
				Ex_FreeBuffer((LPVOID)TextRange.pwzText);
			}
		}
		return 0;
	}

	static EditWindow& GetInstance()
	{
		static EditWindow ret;
		return ret;
	}
};