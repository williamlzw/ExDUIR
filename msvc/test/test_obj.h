#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")


#include "ExDUIR_Func.h"
#include "ExDUIR_Interface.h"

#define Random(min, max) (rand() % (max - min)) + min + 1

#define COLOR_EX_CBTN_CRBKG_NORMAL 100
#define COLOR_EX_CBTN_CRBKG_HOVER 101
#define COLOR_EX_CBTN_CRBKG_DOWN 102
#define COLOR_EX_CBTN_CRBKG_FOCUS 103
#define EOUL_CBTN_CRBKG_NORMAL 0
#define EOUL_CBTN_CRBKG_HOVER 1
#define EOUL_CBTN_CRBKG_DOWN 2
#define EOUL_CBTN_CRBKG_FOCUS 3

#define SCROLLBAR_MESSAGE_SETVISIBLE 56212

// {982B95F0-FFA4-4BDB-A933-2B2EE5F74B58}
DEFINE_GUID(GUID_MYSHADER, 0x982b95f0, 0xffa4, 0x4bdb, 0xa9, 0x33, 0x2b, 0x2e, 0xe5, 0xf7, 0x4b, 0x58);

// {85C487CA-1ABF-4760-9B17-3F62D2A54D63}
DEFINE_GUID(CLSID_MYEFFECT, 0x85c487ca, 0x1abf, 0x4760, 0x9b, 0x17, 0x3f, 0x62, 0xd2, 0xa5, 0x4d, 0x63);

struct LISTVIEW_ITEM
{
	std::wstring text;
	EXARGB color;
	INT depth;
};

struct TLISTVIEW_ITEM
{
	std::wstring title;
	std::wstring text;
	std::wstring btnTitle;
};

LRESULT CALLBACK OnButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_button(HWND hWnd);

void test_label(HWND hWnd);

LRESULT CALLBACK OnCheckButtonCheckedEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_checkbutton(HWND hWnd);

LRESULT CALLBACK OnEditWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnEditNotifyEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnEditButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_edit(HWND hWnd);

LRESULT CALLBACK OnListViewMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_listview(HWND hWnd);

LRESULT CALLBACK OnListButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnListButtonMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnListButtonWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnListButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_listbutton(HWND hWnd);

void test_custombkg(HWND hWnd);
void test_combobox(HWND hWnd);
void test_groupbox(HWND hWnd);
void test_messagebox(HWND hWnd);

LRESULT CALLBACK OnNavButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_navbutton(HWND hWnd);

void test_absolute(HWND hWnd);
void test_relative(HWND hWnd);
void test_linear(HWND hWnd);
void test_flow(HWND hWnd);
void test_table(HWND hWnd);

LRESULT CALLBACK OnAniWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
size_t CALLBACK OnAniEasing(LPVOID pEasing, DOUBLE nProgress, DOUBLE nCurrent, LPVOID pEasingContext, INT nTimeSurplus, size_t p1, size_t p2, size_t p3, size_t p4);
LRESULT CALLBACK OnAniButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void AniShow(BOOL fShow);
void test_ani(HWND hWnd);

LRESULT CALLBACK OnCustomRedrawWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_customredraw(HWND hWnd);

class ColorButton2
{
public:
	void Create(EXHANDLE handle, INT left, INT top, INT width, INT height, LPCWSTR title = NULL, INT style = -1, INT styleEx = -1, INT nID = NULL, INT dwTextFormat = -1, LPARAM lParam = NULL, MsgPROC lpMsgProc = NULL);
	void SetBkgNormalColor(EXARGB color, BOOL redraw);
	void SetBkgHoverColor(EXARGB color, BOOL redraw);
	void SetBkgDownColor(EXARGB color, BOOL redraw);
	void SetBkgFocusColor(EXARGB color, BOOL redraw);
	void SetTextHoverColor(EXARGB color, BOOL redraw);
	void SetTextDownColor(EXARGB color, BOOL redraw);
	void SetTextFocusColor(EXARGB color, BOOL redraw);
	void SetRadius(FLOAT topleft, FLOAT topright, FLOAT bottomright, FLOAT bottomleft, BOOL redraw);

private:
	HEXOBJ mhObj;
};

LRESULT CALLBACK OnColorButtonProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam);
void test_colorbutton(HWND hWnd);

LRESULT CALLBACK OnReportListViewItemChecked(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnReportListViewItemChange(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnReportListViewColumnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_reportlistview(HWND hWnd);

LRESULT CALLBACK OnIconWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_iconlistview(HWND hWnd);

void test_treelistview(HWND hWnd);

LRESULT CALLBACK OnMatrixMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_matrix(HWND hWnd);

void test_buttonex(HWND hWnd);

LRESULT CALLBACK OnEditChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_editex(HWND hWnd);

LRESULT CALLBACK OnMenuBtnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnMenuItemMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnMenuWndMsgProc(HWND hWnd, HEXDUI hExDUI, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnMenuButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_custommenu(HWND hWnd);

LRESULT CALLBACK OnSideButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnParentButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnEventButtonMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnEventButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnEventWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_event(HWND hWnd);

void test_loading(HWND hWnd);

LRESULT CALLBACK OnSliderBarPosChangeEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_sliderbar(HWND hWnd);

void test_rotateimgbox(HWND hWnd);

LRESULT CALLBACK OnDragMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_dragobj(HWND hWnd);

LRESULT CALLBACK OnDropObjDataMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_dropobj(HWND hWnd);

LRESULT CALLBACK OnProgressBarProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_progressbar(HWND hWnd);

LRESULT CALLBACK OnNchitTestMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_nchittest(HWND hWnd);

INT_PTR CALLBACK OnDialgWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnModalButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_modal(HWND hWnd);

LRESULT CALLBACK OnColorPickerButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_colorpicker(HWND hParent);

void test_titlebar(HWND hParent);

LRESULT CALLBACK OnDateBoxButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_datebox(HWND hParent);

LRESULT CALLBACK OnCalendarEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_calendar(HWND hParent);

LRESULT CALLBACK OnChromiumWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void CALLBACK OnFunction(LPCWSTR name, HV8VALUE object, std::vector<uintptr_t*> arguments, uintptr_t* retval, LPCWSTR exception, bool* pbHandled, void* lParam);
void CALLBACK OnBeforeCommandLine(int uMsg, LONG_PTR handler, LONG_PTR hObj, LONG_PTR attach1, LONG_PTR attach2, LONG_PTR attach3, LONG_PTR attach4, bool* pbHWEBVIEWd, void* lParam);
LRESULT CALLBACK OnChromiumEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_chromium(HWND hParent);

LRESULT CALLBACK OnScoreButtonCheckEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_scorebtn(HWND hParent);

void test_carousel(HWND hParent);

LRESULT CALLBACK OnTemplateListViewItemBtnClick(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnTemplateListViewProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_templatelistview(HWND hParent);

LRESULT CALLBACK OnDrawingBoardSwitchEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_drawingboard(HWND hParent);

LRESULT CALLBACK OnPaletteEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_palette(HWND hParent);

LRESULT CALLBACK OnPropertyGridButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnPropertyGridEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_propertygrid(HWND hParent);

LRESULT CALLBACK OnNativeParentWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
LRESULT CALLBACK OnNativeWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_nativewindow(HWND hParent);

LRESULT CALLBACK OnFullScreenWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_fullscreen(HWND hWnd);

LRESULT CALLBACK OnSvgAndFontProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_svgAndfont(HWND hWnd);

LRESULT CALLBACK OnRollMenuWndBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnRollMenuBtnEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OnRollMenuWndMsgProc(HWND hWnd, HEXDUI hExDui, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_rollmenu(HWND hWnd);

LRESULT CALLBACK OnTrayButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_tray(HWND hWnd);

LRESULT CALLBACK OnMaskObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_mask(HWND hWnd);

LRESULT CALLBACK OnTaggingButtonEvent(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_tagging(HWND hWnd);


struct EffectExtraData {
	D2D_VECTOR_3F color;
	float time;
	D2D_VECTOR_2F resolution;
};

//以下代码参考https://github.com/7Brandyn7/Magpie
//https://learn.microsoft.com/zh-cn/windows/win32/direct2d/custom-effects?redirectedfrom=MSDN

class MyTransform : public SimpleDrawTransform<1> {
public:
	MyTransform(const GUID& guid) : SimpleDrawTransform<1>(guid) {}

public:
	HRESULT  SetTime(float value) {
		_time = value;
		return S_OK;
	}

	float GetTime() const {
		return _time;
	}

	HRESULT SetResolution(D2D_VECTOR_2F value) {
		_resolution = value;
		return S_OK;
	}

	D2D_VECTOR_2F GetResolution() const {
		return _resolution;
	}

	HRESULT SetColor(D2D_VECTOR_3F value) {
		_color = value;
		return S_OK;
	}

	D2D_VECTOR_3F GetColor() const {
		return _color;
	}

protected:
	/// <summary>
	/// 重写设置数据
	/// </summary>
	/// <param name="srcSize"></param>
	void _SetShaderConstantBuffer(const SIZE& srcSize) override {
		EffectExtraData shaderConstants{
			_color,
			_time,
			_resolution
		};

		_drawInfo->SetPixelShaderConstantBuffer((BYTE*)&shaderConstants, sizeof(shaderConstants));
	}

private:
	float _time = 0;
	D2D_VECTOR_2F _resolution;
	D2D_VECTOR_3F _color;
};


class MyEffect : public EffectBase {
public:
	IFACEMETHODIMP Initialize(
		_In_ ID2D1EffectContext* pEffectContext,
		_In_ ID2D1TransformGraph* pTransformGraph
	) {
		_shader_loadfromfile(pEffectContext, L"res/effect.cso", GUID_MYSHADER);
		/*std::vector<CHAR> data1;
		Ex_ReadFile(L"res/effect.hlsl", &data1);
		std::string buf = Ex_U2A2(data1);
		if (!_shader_load(pEffectContext, buf.c_str(), buf.length(), GUID_MYSHADER))
		{
			return E_INVALIDARG;
		}*/
		
		_transform = new MyTransform(GUID_MYSHADER);

		auto hr = pTransformGraph->SetSingleTransformNode(_transform);
		if (FAILED(hr)) {
			return hr;
		}

		return S_OK;
	}

	HRESULT SetColor(D2D_VECTOR_3F value) {
		_transform->SetColor(value);
		return S_OK;
	}

	D2D_VECTOR_3F GetColor() const {
		return _transform->GetColor();
	}

	HRESULT SetTime(float value) {
		_transform->SetTime(value);
		return S_OK;
	}

	float GetTime() const {
		return _transform->GetTime();
	}

	HRESULT SetResolution(D2D_VECTOR_2F value) {
		_transform->SetResolution(value);
		return S_OK;
	}

	D2D_VECTOR_2F GetResolution() const {
		return _transform->GetResolution();
	}

	static HRESULT CALLBACK CreateEffect(_Outptr_ IUnknown** ppEffectImpl) {
		*ppEffectImpl = static_cast<ID2D1EffectImpl*>(new MyEffect());

		if (*ppEffectImpl == nullptr) {
			return E_OUTOFMEMORY;
		}

		return S_OK;
	}

private:
	MyEffect() {}
	~MyEffect() {
		_transform->Release();
		_transform = nullptr;
	}
	MyTransform* _transform = nullptr;
};

LRESULT CALLBACK OnEffectObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_effect(HWND hWnd);

LRESULT CALLBACK OnPathAndRgnMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_pathandrgn(HWND hWnd);

LRESULT CALLBACK OnMediaVLCBtnEnevt(HEXOBJ hObj, INT nID, INT nCode, WPARAM wParam, LPARAM lParam);
void test_vlcPlay(HWND hWnd);