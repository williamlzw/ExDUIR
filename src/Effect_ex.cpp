#include "Effect_ex.h"

BOOL _effect_register(GUID guid, LPCWSTR pProperityXml, EX_EFFECT_PROPERITY_INFO* properitys, int properityCount, LPVOID createEffect)
{
	bool isRegistered;
	IsEffectRegistered(g_Ri.pD2Dfactory, guid, isRegistered);
	HRESULT hr = S_OK;
	if (!isRegistered)
	{
		D2D1_PROPERTY_BINDING* bindings = new D2D1_PROPERTY_BINDING[properityCount];
		for (int i = 0; i < properityCount; i++)
		{
			D2D1_PROPERTY_BINDING binding;
			binding.propertyName = properitys[i].propertyName;
			binding.getFunction = (PD2D1_PROPERTY_GET_FUNCTION)properitys[i].getFun;
			binding.setFunction = (PD2D1_PROPERTY_SET_FUNCTION)properitys[i].setFun;
			bindings[i] = binding;
		}
		hr = g_Ri.pD2Dfactory->RegisterEffectFromString(guid, pProperityXml, bindings, properityCount, (PD2D1_EFFECT_FACTORY)createEffect);
		return hr == S_OK;
	}
	return isRegistered;
}

BOOL _effect_create(GUID guid, HEXEFFECT* ret)
{
	ID2D1Effect* pEffect = nullptr;
	auto hr = g_Ri.pD2DDeviceContext->CreateEffect(guid, &pEffect);
	if (hr == S_OK)
	{
		if (ret)
		{
			*ret = pEffect;
			return TRUE;
		}
	}
	return FALSE;
}

void _effect_create_buffer(HEXEFFECT hEffect, int width, int height)
{
	D2D1_BITMAP_PROPERTIES1 pro;
	pro.pixelFormat.format = DXGI_FORMAT::DXGI_FORMAT_B8G8R8A8_UNORM;
	pro.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	pro.dpiX = 96;
	pro.dpiY = 96;
	pro.bitmapOptions = D2D1_BITMAP_OPTIONS_NONE;
	pro.colorContext = NULL;
	ID2D1Bitmap1* lpID2D1BitmapBuffer = nullptr;
	D2D_SIZE_U sizeu;
	sizeu.width = width;
	sizeu.height = height;
	g_Ri.pD2DDeviceContext->CreateBitmap(sizeu, 0, 0, pro, &lpID2D1BitmapBuffer);

	((ID2D1Effect*)hEffect)->SetInput(0, lpID2D1BitmapBuffer, TRUE);
}

void _effect_set_vector2(HEXEFFECT hEffect, PCWSTR lpszProperityName, float value1, float value2)
{
	D2D_VECTOR_2F vec{ value1, value2 };
	((ID2D1Effect*)hEffect)->SetValueByName(lpszProperityName, D2D1_PROPERTY_TYPE_VECTOR2, (BYTE*)&vec, 8);
}

void _effect_set_float(HEXEFFECT hEffect, PCWSTR lpszProperityName, float value)
{
	((ID2D1Effect*)hEffect)->SetValueByName(lpszProperityName, D2D1_PROPERTY_TYPE_FLOAT, (BYTE*)&value, 4);
}

void _effect_set_vector3(HEXEFFECT hEffect, PCWSTR lpszProperityName, float value1, float value2, float value3)
{
	D2D_VECTOR_3F vec{ value1, value2, value3 };
	((ID2D1Effect*)hEffect)->SetValueByName(lpszProperityName, D2D1_PROPERTY_TYPE_VECTOR3, (BYTE*)&vec, 12);
}

void _effect_set_uint32(HEXEFFECT hEffect, PCWSTR lpszProperityName, UINT value)
{
	((ID2D1Effect*)hEffect)->SetValueByName(lpszProperityName, D2D1_PROPERTY_TYPE_UINT32, (BYTE*)&value, 4);
}

void _effect_set_int32(HEXEFFECT hEffect, PCWSTR lpszProperityName, INT value)
{
	((ID2D1Effect*)hEffect)->SetValueByName(lpszProperityName, D2D1_PROPERTY_TYPE_INT32, (BYTE*)&value, 4);
}
