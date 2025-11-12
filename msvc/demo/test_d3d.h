#pragma once
#include "ExDUIR_Func.h"
#include <d2d1.h>
#include <d2d1_1.h>
#include <d3d11.h>
#include <d3d11_1.h>
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <shlwapi.h>
#include <algorithm>
#include <locale>
#include <functional>
#include <DirectXMath.h> 
#include <DirectXColors.h> 
#include <d3dcompiler.h>
#include "Geometry.h"


struct VertexPos
{
	DirectX::XMFLOAT3 pos;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[1];
};

struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosTex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 tex;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosSize
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT2 size;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[2];
};

struct VertexPosNormalColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 color;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};


struct VertexPosNormalTex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 tex;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[3];
};

struct VertexPosNormalTangentTex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT4 tangent;
	DirectX::XMFLOAT2 tex;
	static const D3D11_INPUT_ELEMENT_DESC inputLayout[4];
};


class CpuTimer
{
public:
	CpuTimer();

	float TotalTime()const;     // 返回从Reset()调用之后经过的时间，但不包括暂停期间的
	float DeltaTime()const;		// 返回帧间隔时间

	void Reset();               // 计时开始前或者需要重置时调用
	void Start();               // 在开始计时或取消暂停的时候调用
	void Stop();                // 在需要暂停的时候调用
	void Tick();                // 在每一帧开始的时候调用
	bool IsStopped() const;     // 计时器是否暂停/结束

private:
	double m_SecondsPerCount = 0.0;
	double m_DeltaTime = -1.0;

	__int64 m_BaseTime = 0;
	__int64 m_PausedTime = 0;
	__int64 m_StopTime = 0;
	__int64 m_PrevTime = 0;
	__int64 m_CurrTime = 0;

	bool m_Stopped = false;
};


struct VSConstantBuffer
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX view;
	DirectX::XMMATRIX proj;
	DirectX::XMMATRIX worldInvTranspose;

};

// 方向光
struct DirectionalLight
{
	DirectionalLight() = default;

	DirectionalLight(const DirectionalLight&) = default;
	DirectionalLight& operator=(const DirectionalLight&) = default;

	DirectionalLight(DirectionalLight&&) = default;
	DirectionalLight& operator=(DirectionalLight&&) = default;

	DirectionalLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, const DirectX::XMFLOAT4& _specular,
		const DirectX::XMFLOAT3& _direction) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), direction(_direction), pad() {
	}

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 direction;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 点光
struct PointLight
{
	PointLight() = default;

	PointLight(const PointLight&) = default;
	PointLight& operator=(const PointLight&) = default;

	PointLight(PointLight&&) = default;
	PointLight& operator=(PointLight&&) = default;

	PointLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, const DirectX::XMFLOAT4& _specular,
		const DirectX::XMFLOAT3& _position, float _range, const DirectX::XMFLOAT3& _att) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), position(_position), range(_range), att(_att), pad() {
	}

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	// 打包成4D向量: (position, range)
	DirectX::XMFLOAT3 position;
	float range;

	// 打包成4D向量: (A0, A1, A2, pad)
	DirectX::XMFLOAT3 att;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 聚光灯
struct SpotLight
{
	SpotLight() = default;

	SpotLight(const SpotLight&) = default;
	SpotLight& operator=(const SpotLight&) = default;

	SpotLight(SpotLight&&) = default;
	SpotLight& operator=(SpotLight&&) = default;

	SpotLight(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, const DirectX::XMFLOAT4& _specular,
		const DirectX::XMFLOAT3& _position, float _range, const DirectX::XMFLOAT3& _direction,
		float _spot, const DirectX::XMFLOAT3& _att) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular),
		position(_position), range(_range), direction(_direction), spot(_spot), att(_att), pad() {
	}

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;

	// 打包成4D向量: (position, range)
	DirectX::XMFLOAT3 position;
	float range;

	// 打包成4D向量: (direction, spot)
	DirectX::XMFLOAT3 direction;
	float spot;

	// 打包成4D向量: (att, pad)
	DirectX::XMFLOAT3 att;
	float pad; // 最后用一个浮点数填充使得该结构体大小满足16的倍数，便于我们以后在HLSL设置数组
};

// 物体表面材质
struct Material
{
	Material() = default;

	Material(const Material&) = default;
	Material& operator=(const Material&) = default;

	Material(Material&&) = default;
	Material& operator=(Material&&) = default;

	Material(const DirectX::XMFLOAT4& _ambient, const DirectX::XMFLOAT4& _diffuse, const DirectX::XMFLOAT4& _specular,
		const DirectX::XMFLOAT4& _reflect) :
		ambient(_ambient), diffuse(_diffuse), specular(_specular), reflect(_reflect) {
	}

	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular; // w = 镜面反射强度
	DirectX::XMFLOAT4 reflect;
};

struct PSConstantBuffer
{
	DirectionalLight dirLight;
	PointLight pointLight;
	SpotLight spotLight;
	Material material;
	DirectX::XMFLOAT4 eyePos;
};


struct D3DINFO {
	ID3D11Texture2D* texture;

	ID3D11InputLayout* m_pVertexLayout;	    // 顶点输入布局
	ID3D11Buffer* m_pVertexBuffer;			// 顶点缓冲区
	ID3D11Buffer* m_pIndexBuffer;			// 索引缓冲区
	ID3D11Buffer* m_pConstantBuffers[2];	// 常量缓冲区
	UINT m_IndexCount;						// 绘制物体的索引数组大小

	ID3D11VertexShader* m_pVertexShader;	    // 顶点着色器
	ID3D11PixelShader* m_pPixelShader;		// 像素着色器
	VSConstantBuffer m_VSConstantBuffer;			// 用于修改用于VS的GPU常量缓冲区的变量
	PSConstantBuffer m_PSConstantBuffer;			// 用于修改用于PS的GPU常量缓冲区的变量

	DirectionalLight m_DirLight;					// 默认环境光
	PointLight m_PointLight;						// 默认点光
	SpotLight m_SpotLight;						    // 默认汇聚光

	ID3D11RasterizerState* m_pRSWireframe;	// 光栅化状态: 线框模式

	// 常用资源
	ID3D11Texture2D* m_pDepthStencilBuffer;				// 深度模板缓冲区
	ID3D11RenderTargetView* m_pRenderTargetView;			// 渲染目标视图
	ID3D11DepthStencilView* m_pDepthStencilView;			// 深度模板视图
	D3D11_VIEWPORT m_ScreenViewport;                            // 视口

	CpuTimer m_Timer;           // 计时器

	IDXGISurface* pDXGISurface;
};

LRESULT CALLBACK d3d_callback(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult);
void test_d3d(HWND hWnd);
void UpdateScene(D3DINFO* d3d, float dt);

bool ResetMesh(D3DINFO* d3d, const Geometry::MeshData<VertexPosNormalColor>& meshData);

DirectX::XMMATRIX XM_CALLCONV InverseTranspose(DirectX::FXMMATRIX M);
HRESULT CreateShaderFromFile(
	const WCHAR* csoFileNameInOut,
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut);