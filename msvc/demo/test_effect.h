#pragma once
#include "ExDUIR_Func.h"
#include "ExDUIR_Interface.h"

// {982B95F0-FFA4-4BDB-A933-2B2EE5F74B58}
DEFINE_GUID(GUID_MYSHADER, 0x982b95f0, 0xffa4, 0x4bdb, 0xa9, 0x33, 0x2b, 0x2e, 0xe5, 0xf7, 0x4b,
            0x58);

// {85C487CA-1ABF-4760-9B17-3F62D2A54D63}
DEFINE_GUID(CLSID_MYEFFECT, 0x85c487ca, 0x1abf, 0x4760, 0x9b, 0x17, 0x3f, 0x62, 0xd2, 0xa5, 0x4d,
            0x63);

struct EffectExtraData
{
    D2D_VECTOR_3F color;
    float         time;
    D2D_VECTOR_2F resolution;
};

// 以下代码参考https://github.com/7Brandyn7/Magpie
// https://learn.microsoft.com/zh-cn/windows/win32/direct2d/custom-effects?redirectedfrom=MSDN

class MyTransform : public SimpleDrawTransform<1>
{
public:
    MyTransform(const GUID& guid)
        : SimpleDrawTransform<1>(guid)
    {}

public:
    HRESULT SetTime(float value)
    {
        _time = value;
        return S_OK;
    }

    float GetTime() const { return _time; }

    HRESULT SetResolution(D2D_VECTOR_2F value)
    {
        _resolution = value;
        return S_OK;
    }

    D2D_VECTOR_2F GetResolution() const { return _resolution; }

    HRESULT SetColor(D2D_VECTOR_3F value)
    {
        _color = value;
        return S_OK;
    }

    D2D_VECTOR_3F GetColor() const { return _color; }

protected:
    /// <summary>
    /// 重写设置数据
    /// </summary>
    /// <param name="srcSize"></param>
    void _SetShaderConstantBuffer(const SIZE& srcSize) override
    {
        EffectExtraData shaderConstants{_color, _time, _resolution};

        _drawInfo->SetPixelShaderConstantBuffer((BYTE*)&shaderConstants, sizeof(shaderConstants));
    }

private:
    float         _time = 0;
    D2D_VECTOR_2F _resolution;
    D2D_VECTOR_3F _color;
};


class MyEffect : public EffectBase
{
public:
    IFACEMETHODIMP Initialize(_In_ ID2D1EffectContext*  pEffectContext,
                              _In_ ID2D1TransformGraph* pTransformGraph)
    {
        _shader_loadfromfile(pEffectContext, L"res/effect/effect.cso", GUID_MYSHADER);
        /*std::vector<CHAR> data1;
        Ex_ReadFile(L"res/effect/effect.hlsl", &data1);
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

    HRESULT SetColor(D2D_VECTOR_3F value)
    {
        _transform->SetColor(value);
        return S_OK;
    }

    D2D_VECTOR_3F GetColor() const { return _transform->GetColor(); }

    HRESULT SetTime(float value)
    {
        _transform->SetTime(value);
        return S_OK;
    }

    float GetTime() const { return _transform->GetTime(); }

    HRESULT SetResolution(D2D_VECTOR_2F value)
    {
        _transform->SetResolution(value);
        return S_OK;
    }

    D2D_VECTOR_2F GetResolution() const { return _transform->GetResolution(); }

    static HRESULT CALLBACK CreateEffect(_Outptr_ IUnknown** ppEffectImpl)
    {
        *ppEffectImpl = static_cast<ID2D1EffectImpl*>(new MyEffect());

        if (*ppEffectImpl == nullptr) {
            return E_OUTOFMEMORY;
        }

        return S_OK;
    }

private:
    MyEffect() {}
    ~MyEffect()
    {
        _transform->Release();
        _transform = nullptr;
    }
    MyTransform* _transform = nullptr;
};

LRESULT CALLBACK OnEffectObjMsgProc(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam,
                                    LRESULT* lpResult);
void             test_effect(HWND hWnd);
