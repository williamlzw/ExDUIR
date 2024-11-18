#include <vector>
#include "ExDUIRCPP.hpp"
#include "help_ex.h"


// {982B95F0-FFA4-4BDB-A933-2B2EE5F74B58}
DEFINE_GUID(GUID_MYSHADER, 0x982b95f0, 0xffa4, 0x4bdb, 0xa9, 0x33, 0x2b, 0x2e, 0xe5, 0xf7, 0x4b, 0x58);

// {85C487CA-1ABF-4760-9B17-3F62D2A54D63}
DEFINE_GUID(CLSID_MYEFFECT, 0x85c487ca, 0x1abf, 0x4760, 0x9b, 0x17, 0x3f, 0x62, 0xd2, 0xa5, 0x4d, 0x63);

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
		std::vector<CHAR> data1;
		Ex_ReadFile(L"../test/res/effect.hlsl", &data1);
		std::string buf = u2a2(data1);
		_shader_loadfromfile(pEffectContext, L"../test/res/effect.cso", GUID_MYSHADER);
		/*if (!_shader_load(pEffectContext, buf.c_str(), buf.length(), GUID_MYSHADER))
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

class EffectWindow
{
private:
	ExSkin m_skin;
	ExStatic m_static;

public:
	void CreateEffectWindow(HWND pOwner)
	{
		DWORD dwStyleDUI = WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE | WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE | WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE | WINDOW_STYLE_HASICON | WINDOW_STYLE_NOSHADOW;
		m_skin = ExSkin(pOwner, 0, 0, 500, 360, L"测试效果器", dwStyleDUI);
		m_skin.SetBackgroundColor(ExARGB(150, 150, 150, 255));
		m_static = ExStatic(m_skin, 50, 50, 400, 260, L"", -1, -1, -1, 0, 0, 0, OnEffectObjMsgProc);
		m_static.SetTimer(1);
		m_skin.Show();
	}

	static LRESULT CALLBACK OnEffectObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
	{
		ExControl obj = ExControl(hObj);
		if (uMsg == WM_CREATE)
		{
			obj.InitPropList(3);
			//注册效果器
			std::wstring pProperityXml;
			std::vector<CHAR> data1;
			Ex_ReadFile(L"../test/res/effect.xml", &data1);
			pProperityXml = u2w2(data1);

			EX_EFFECT_PROPERITY_INFO bindings[] =
			{
				D2D1_VALUE_TYPE_BINDING(L"Color", &MyEffect::SetColor, &MyEffect::GetColor),
				D2D1_VALUE_TYPE_BINDING(L"Time", &MyEffect::SetTime, &MyEffect::GetTime),
				D2D1_VALUE_TYPE_BINDING(L"Resolution", &MyEffect::SetResolution, &MyEffect::GetResolution)
			};

			_effect_register(CLSID_MYEFFECT, pProperityXml.c_str(), bindings, ARRAYSIZE(bindings), MyEffect::CreateEffect);
			HEXEFFECT pEffect = nullptr;
			auto ret = _effect_create(CLSID_MYEFFECT, &pEffect);

			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			LARGE_INTEGER start;
			QueryPerformanceCounter(&start);
			auto nTimeDeta = start.QuadPart / freq.QuadPart;
			RECT rc;
			obj.GetRect(rc);
			int width = rc.right - rc.left;
			int height = rc.bottom - rc.top;
			//创建临时图像保存绘制内容
			_effect_create_buffer(pEffect, width, height);
			_effect_set_vector2(pEffect, L"Resolution", width, height);
			obj.SetObjProp(0, (size_t)pEffect);
			obj.SetObjProp(1, (size_t)nTimeDeta);
		}
		else if (uMsg == WM_PAINT)
		{
			HEXEFFECT pEffect = (HEXEFFECT)obj.GetObjProp(0);
			ULONGLONG nTimeDeta = obj.GetObjProp(1);
			LARGE_INTEGER freq;
			QueryPerformanceFrequency(&freq);
			LARGE_INTEGER start;
			QueryPerformanceCounter(&start);
			auto time2 = (FLOAT)start.QuadPart / freq.QuadPart;
			float timeoffset = time2 - nTimeDeta;

			_effect_set_float(pEffect, L"Time", timeoffset);
			//呈现到组件
			EX_PAINTSTRUCT ps{ 0 };
			obj.BeginPaint(ps);
			ExCanvas canvas = ExCanvas(ps.hCanvas);
			canvas.ApplyEffect(pEffect);
			obj.EndPaint(ps);

		}
		else if (uMsg == WM_TIMER)
		{
			obj.Update();
		}
		else if (uMsg == WM_DESTROY)
		{
			obj.KillTimer();
			HEXEFFECT pEffect = (HEXEFFECT)obj.GetObjProp(0);
			_effect_destroy(pEffect);
		}
		return 0;
	}

	static EffectWindow& GetInstance()
	{
		static EffectWindow ret;
		return ret;
	}
};