#include "test_d3d.h"
using namespace Geometry;

const D3D11_INPUT_ELEMENT_DESC VertexPos::inputLayout[1] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosColor::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosTex::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosSize::inputLayout[2] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosNormalColor::inputLayout[3] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosNormalTex::inputLayout[3] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC VertexPosNormalTangentTex::inputLayout[4] = {
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

CpuTimer::CpuTimer()
{
	__int64 countsPerSec{};
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double)countsPerSec;
}


float CpuTimer::TotalTime()const
{
	// 如果调用了Stop()，暂停中的这段时间我们不需要计入。此外
	// m_StopTime - m_BaseTime可能会包含之前的暂停时间，为
	// 此我们可以从m_StopTime减去之前累积的暂停的时间
	//
	//                     |<-- 暂停的时间 -->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  m_BaseTime       m_StopTime        startTime     m_StopTime    m_CurrTime

	if (m_Stopped)
	{
		return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}

	// m_CurrTime - m_BaseTime包含暂停时间，但我们不想将它计入。
	// 为此我们可以从m_CurrTime减去之前累积的暂停的时间
	//
	//  (m_CurrTime - m_PausedTime) - m_BaseTime 
	//
	//                     |<-- 暂停的时间 -->|
	// ----*---------------*-----------------*------------*------> time
	//  m_BaseTime       m_StopTime        startTime     m_CurrTime

	else
	{
		return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
	}
}

float CpuTimer::DeltaTime()const
{
	return (float)m_DeltaTime;
}

void CpuTimer::Reset()
{
	__int64 currTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_PausedTime = 0;   // 涉及到多次Reset的话需要将其归0
	m_Stopped = false;
}

void CpuTimer::Start()
{
	__int64 startTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


	// 累积暂停开始到暂停结束的这段时间
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  m_BaseTime       m_StopTime        startTime     

	if (m_Stopped)
	{
		m_PausedTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		m_StopTime = 0;
		m_Stopped = false;
	}
}

void CpuTimer::Stop()
{
	if (!m_Stopped)
	{
		__int64 currTime{};
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_StopTime = currTime;
		m_Stopped = true;
	}
}

void CpuTimer::Tick()
{
	if (m_Stopped)
	{
		m_DeltaTime = 0.0;
		return;
	}

	__int64 currTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;

	// 当前Tick与上一Tick的帧间隔
	m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

	m_PrevTime = m_CurrTime;

	if (m_DeltaTime < 0.0)
	{
		m_DeltaTime = 0.0;
	}
}

bool CpuTimer::IsStopped() const
{
	return m_Stopped;
}

HRESULT CreateShaderFromFile(
	const WCHAR* csoFileNameInOut,
	const WCHAR* hlslFileName,
	LPCSTR entryPoint,
	LPCSTR shaderModel,
	ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	// 寻找是否有已经编译好的顶点着色器
	if (csoFileNameInOut && D3DReadFileToBlob(csoFileNameInOut, ppBlobOut) == S_OK)
	{
		return hr;
	}
	else
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// 设置 D3DCOMPILE_DEBUG 标志用于获取着色器调试信息。该标志可以提升调试体验，
		// 但仍然允许着色器进行优化操作
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// 在Debug环境下禁用优化以避免出现一些不合理的情况
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif
		ID3DBlob* errorBlob = nullptr;
		hr = D3DCompileFromFile(hlslFileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint, shaderModel,
			dwShaderFlags, 0, ppBlobOut, &errorBlob);
		if (FAILED(hr))
		{
			if (errorBlob != nullptr)
			{
				OutputDebugStringA(reinterpret_cast<const char*>(errorBlob->GetBufferPointer()));
				errorBlob->Release();
				errorBlob = nullptr;
			}

			return hr;
		}

		// 若指定了输出文件名，则将着色器二进制信息输出
		if (csoFileNameInOut)
		{
			return D3DWriteBlobToFile(*ppBlobOut, csoFileNameInOut, FALSE);
		}
	}

	return hr;
}

inline DirectX::XMMATRIX XM_CALLCONV InverseTranspose(DirectX::FXMMATRIX M)
{
	using namespace DirectX;

	// 世界矩阵的逆的转置仅针对法向量，我们也不需要世界矩阵的平移分量
	// 而且不去掉的话，后续再乘上观察矩阵之类的就会产生错误的变换结果
	XMMATRIX A = M;
	A.r[3] = g_XMIdentityR3;

	return XMMatrixTranspose(XMMatrixInverse(nullptr, A));
}

void UpdateScene(D3DINFO* d3d, float dt)
{
	ID3D11Device* m_pd3dDevice = (ID3D11Device*)Ex_GetD3DDevice1();
	ID3D11DeviceContext* m_pd3dImmediateContext = (ID3D11DeviceContext*)Ex_GetD3DDeviceContext();
	static float phi = 0.0f, theta = 0.0f;
	phi += 0.3f * dt, theta += 0.37f * dt;
	DirectX::XMMATRIX W = DirectX::XMMatrixRotationX(phi) * DirectX::XMMatrixRotationY(theta);
	d3d->m_VSConstantBuffer.world = XMMatrixTranspose(W);
	d3d->m_VSConstantBuffer.worldInvTranspose = DirectX::XMMatrixTranspose(InverseTranspose(W));

	if (m_pd3dImmediateContext)
	{
		// 更新常量缓冲区，让立方体转起来
		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_pd3dImmediateContext->Map(d3d->m_pConstantBuffers[0], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(VSConstantBuffer), &d3d->m_VSConstantBuffer, sizeof(VSConstantBuffer));
		m_pd3dImmediateContext->Unmap(d3d->m_pConstantBuffers[0], 0);

		m_pd3dImmediateContext->Map(d3d->m_pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &d3d->m_PSConstantBuffer, sizeof(PSConstantBuffer));
		m_pd3dImmediateContext->Unmap(d3d->m_pConstantBuffers[1], 0);
	}

}

bool ResetMesh(D3DINFO* d3d, const MeshData<VertexPosNormalColor>& meshData)
{
	ID3D11Device* m_pd3dDevice = (ID3D11Device*)Ex_GetD3DDevice1();
	ID3D11DeviceContext* m_pd3dImmediateContext = (ID3D11DeviceContext*)Ex_GetD3DDeviceContext();
	// 释放旧资源
	if (d3d->m_pVertexBuffer)
	{
		d3d->m_pVertexBuffer->Release();
		d3d->m_pVertexBuffer = 0;
	}
	if (d3d->m_pIndexBuffer)
	{
		d3d->m_pIndexBuffer->Release();
		d3d->m_pIndexBuffer = 0;
	}
	// 设置顶点缓冲区描述
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = (UINT)meshData.vertexVec.size() * sizeof(VertexPosNormalColor);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	// 新建顶点缓冲区
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = meshData.vertexVec.data();
	m_pd3dDevice->CreateBuffer(&vbd, &InitData, &d3d->m_pVertexBuffer);

	// 输入装配阶段的顶点缓冲区设置
	UINT stride = sizeof(VertexPosNormalColor);	// 跨越字节数
	UINT offset = 0;							// 起始偏移量

	m_pd3dImmediateContext->IASetVertexBuffers(0, 1, &d3d->m_pVertexBuffer, &stride, &offset);

	// 设置索引缓冲区描述
	d3d->m_IndexCount = (UINT)meshData.indexVec.size();
	D3D11_BUFFER_DESC ibd;
	ZeroMemory(&ibd, sizeof(ibd));
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = d3d->m_IndexCount * sizeof(DWORD);
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	// 新建索引缓冲区
	InitData.pSysMem = meshData.indexVec.data();
	m_pd3dDevice->CreateBuffer(&ibd, &InitData, &d3d->m_pIndexBuffer);
	// 输入装配阶段的索引缓冲区设置
	m_pd3dImmediateContext->IASetIndexBuffer(d3d->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	return 1;
}

LRESULT CALLBACK d3d_callback(HWND hWnd, HEXOBJ hObj, INT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* lpResult)
{
	D3DINFO* d3d = (D3DINFO*)Ex_ObjGetLong(hObj, OBJECT_LONG_OWNER);
	switch (uMsg)
	{
	case WM_CREATE:
	{
		ID3D11Device* m_pd3dDevice = (ID3D11Device*)Ex_GetD3DDevice1();
		ID3D11DeviceContext* m_pd3dImmediateContext = (ID3D11DeviceContext*)Ex_GetD3DDeviceContext();
		d3d = new D3DINFO{ 0 };

		Ex_ObjSetLong(hObj, OBJECT_LONG_OWNER, (LONG_PTR)d3d);
		d3d->m_Timer.Reset();

		ID3DBlob* blob = nullptr;
		// 创建顶点着色器
		CreateShaderFromFile(L"HLSL\\Light_VS.cso", L"HLSL\\Light_VS.hlsl", "VS", "vs_5_0", &blob);
		m_pd3dDevice->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &d3d->m_pVertexShader);
		// 创建并绑定顶点布局
		m_pd3dDevice->CreateInputLayout(VertexPosNormalColor::inputLayout, ARRAYSIZE(VertexPosNormalColor::inputLayout),
			blob->GetBufferPointer(), blob->GetBufferSize(), &d3d->m_pVertexLayout);
		blob->Release();
		blob = nullptr;
		// 创建像素着色器
		CreateShaderFromFile(L"HLSL\\Light_PS.cso", L"HLSL\\Light_PS.hlsl", "PS", "ps_5_0", &blob);
		m_pd3dDevice->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &d3d->m_pPixelShader);

		auto meshData = Geometry::CreateBox<VertexPosNormalColor>();

		ResetMesh(d3d, meshData);
		// ******************
		// 设置常量缓冲区描述
		D3D11_BUFFER_DESC cbd;
		ZeroMemory(&cbd, sizeof(cbd));
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.ByteWidth = sizeof(VSConstantBuffer);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		// 新建用于VS和PS的常量缓冲区
		m_pd3dDevice->CreateBuffer(&cbd, nullptr, &d3d->m_pConstantBuffers[0]);
		cbd.ByteWidth = sizeof(PSConstantBuffer);
		m_pd3dDevice->CreateBuffer(&cbd, nullptr, &d3d->m_pConstantBuffers[1]);

		// ******************
		// 初始化默认光照
		// 方向光
		d3d->m_DirLight.ambient = DirectX::XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
		d3d->m_DirLight.diffuse = DirectX::XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		d3d->m_DirLight.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		d3d->m_DirLight.direction = DirectX::XMFLOAT3(-0.577f, -0.577f, 0.577f);
		// 点光
		d3d->m_PointLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f);
		d3d->m_PointLight.ambient = DirectX::XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		d3d->m_PointLight.diffuse = DirectX::XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f);
		d3d->m_PointLight.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		d3d->m_PointLight.att = DirectX::XMFLOAT3(0.0f, 0.1f, 0.0f);
		d3d->m_PointLight.range = 25.0f;
		// 聚光灯
		d3d->m_SpotLight.position = DirectX::XMFLOAT3(0.0f, 0.0f, -5.0f);
		d3d->m_SpotLight.direction = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
		d3d->m_SpotLight.ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		d3d->m_SpotLight.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		d3d->m_SpotLight.specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		d3d->m_SpotLight.att = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
		d3d->m_SpotLight.spot = 12.0f;
		d3d->m_SpotLight.range = 10000.0f;
		// 初始化用于VS的常量缓冲区的值
		d3d->m_VSConstantBuffer.world = DirectX::XMMatrixIdentity();
		d3d->m_VSConstantBuffer.view = DirectX::XMMatrixTranspose(DirectX::XMMatrixLookAtLH(
			DirectX::XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f),
			DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
		));
		RECT rc;
		Ex_ObjGetRect(hObj, &rc);
		d3d->m_VSConstantBuffer.proj = DirectX::XMMatrixTranspose(DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV2,
			static_cast<float>(rc.right - rc.left) / (rc.bottom - rc.top), 1.0f, 1000.0f));
		d3d->m_VSConstantBuffer.worldInvTranspose = DirectX::XMMatrixIdentity();

		// 初始化用于PS的常量缓冲区的值
		d3d->m_PSConstantBuffer.material.ambient = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		d3d->m_PSConstantBuffer.material.diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		d3d->m_PSConstantBuffer.material.specular = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 5.0f);
		// 使用默认平行光
		d3d->m_PSConstantBuffer.dirLight = d3d->m_DirLight;
		// 注意不要忘记设置此处的观察位置，否则高亮部分会有问题
		d3d->m_PSConstantBuffer.eyePos = DirectX::XMFLOAT4(0.0f, 0.0f, -5.0f, 0.0f);


		D3D11_MAPPED_SUBRESOURCE mappedData;
		m_pd3dImmediateContext->Map(d3d->m_pConstantBuffers[1], 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		memcpy_s(mappedData.pData, sizeof(PSConstantBuffer), &d3d->m_VSConstantBuffer, sizeof(PSConstantBuffer));
		m_pd3dImmediateContext->Unmap(d3d->m_pConstantBuffers[1], 0);

		// ******************
		// 初始化光栅化状态

		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_WIREFRAME;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		m_pd3dDevice->CreateRasterizerState(&rasterizerDesc, &d3d->m_pRSWireframe);


		// ******************
		// 给渲染管线各个阶段绑定好所需资源
		//

		// 设置图元类型，设定输入布局
		m_pd3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_pd3dImmediateContext->IASetInputLayout(d3d->m_pVertexLayout);
		// 将着色器绑定到渲染管线
		m_pd3dImmediateContext->VSSetShader(d3d->m_pVertexShader, nullptr, 0);
		// VS常量缓冲区对应HLSL寄存于b0的常量缓冲区
		m_pd3dImmediateContext->VSSetConstantBuffers(0, 1, &d3d->m_pConstantBuffers[0]);
		// PS常量缓冲区对应HLSL寄存于b1的常量缓冲区
		m_pd3dImmediateContext->PSSetConstantBuffers(1, 1, &d3d->m_pConstantBuffers[1]);
		m_pd3dImmediateContext->PSSetShader(d3d->m_pPixelShader, nullptr, 0);


		Ex_ObjSetTimer(hObj, 10);
		break;
	}
	case WM_DESTROY:
	{
		if (d3d)
		{
			delete d3d;
		}
		break;
	}
	case WM_PAINT:
	{
		RECT rc;
		Ex_ObjGetRect(hObj, &rc);
		ID3D11Device* m_pd3dDevice = (ID3D11Device*)Ex_GetD3DDevice1();
		ID3D11DeviceContext* m_pd3dImmediateContext = (ID3D11DeviceContext*)Ex_GetD3DDeviceContext();
		m_pd3dImmediateContext->OMSetRenderTargets(1, &d3d->m_pRenderTargetView, d3d->m_pDepthStencilView);
		d3d->m_ScreenViewport.TopLeftX = 0;
		d3d->m_ScreenViewport.TopLeftY = 0;
		d3d->m_ScreenViewport.Width = rc.right - rc.left; //修改这里会导致d3d图像缩放
		d3d->m_ScreenViewport.Height = rc.bottom - rc.top;//
		d3d->m_ScreenViewport.MinDepth = 0.0f;
		d3d->m_ScreenViewport.MaxDepth = 1.0f;
		m_pd3dImmediateContext->RSSetViewports(1, &d3d->m_ScreenViewport);
		d3d->m_Timer.Tick();

		UpdateScene(d3d, d3d->m_Timer.DeltaTime());
		DirectX::XMVECTORF32 试试 = { { { 0.f, 0.f, 0.f, .6f } } };
		m_pd3dImmediateContext->ClearRenderTargetView(d3d->m_pRenderTargetView, reinterpret_cast<const float*>(&试试));// Colors::Black
		m_pd3dImmediateContext->ClearDepthStencilView(d3d->m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pd3dImmediateContext->DrawIndexed(d3d->m_IndexCount, 0, 0);// 绘制几何模型
		m_pd3dImmediateContext->Flush();
		EX_PAINTSTRUCT ps{ 0 };
		if (Ex_ObjBeginPaint(hObj, &ps))
		{
			if (d3d->pDXGISurface)
			{
				ID2D1DeviceContext* pD2DContext = (ID2D1DeviceContext*)_canvas_getcontext(ps.hCanvas, CANVAS_DX_D2DCONTEXT);
				D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(D2D1_BITMAP_OPTIONS_NONE, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
				ID2D1Bitmap1* pBitmap = nullptr;
				pD2DContext->CreateBitmapFromDxgiSurface(d3d->pDXGISurface, &bitmapProperties, &pBitmap);
				pD2DContext->DrawBitmap(pBitmap,
					D2D1_RECT_F{
						0, 0, (float)ps.rcPaint.right - (float)ps.rcPaint.left, (float)ps.rcPaint.bottom - (float)ps.rcPaint.top
					},
					1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, 0);
				pBitmap->Release();

			}
			Ex_ObjEndPaint(hObj, &ps);
		}
		*lpResult = 0;
		return 1;
	}
	case WM_SIZE:
	{
		if (d3d->texture)
		{
			d3d->texture->Release();
			d3d->texture = 0;
		}

		ID3D11Device1* m_pd3dDevice = (ID3D11Device1*)Ex_GetD3DDevice1();
		ID3D11DeviceContext* m_pd3dImmediateContext = (ID3D11DeviceContext*)Ex_GetD3DDeviceContext();

		D3D11_TEXTURE2D_DESC texDesc = { };
		RECT rc;
		Ex_ObjGetRect(hObj, &rc);
		texDesc.Height = rc.bottom - rc.top;
		texDesc.Width = rc.right - rc.left;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
		texDesc.CPUAccessFlags = 0;
		HRESULT hr = m_pd3dDevice->CreateTexture2D(&texDesc, NULL, &d3d->texture);
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc = { };
		renderTargetViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		hr = m_pd3dDevice->CreateRenderTargetView(d3d->texture, &renderTargetViewDesc, &d3d->m_pRenderTargetView);
		if (d3d->pDXGISurface)
		{
			d3d->pDXGISurface->Release();
			d3d->pDXGISurface = nullptr;
		}
		hr = d3d->texture->QueryInterface(__uuidof(IDXGISurface), reinterpret_cast<void**>(&d3d->pDXGISurface));



		D3D11_TEXTURE2D_DESC depthStencilDesc;
		depthStencilDesc.Height = rc.bottom - rc.top;
		depthStencilDesc.Width = rc.right - rc.left;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		
		
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags = 0;
		depthStencilDesc.MiscFlags = 0;

		// 创建深度缓冲区以及深度模板视图
		m_pd3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &d3d->m_pDepthStencilBuffer);
		m_pd3dDevice->CreateDepthStencilView(d3d->m_pDepthStencilBuffer, nullptr, &d3d->m_pDepthStencilView);


		// 将渲染目标视图和深度/模板缓冲区结合到管线
		m_pd3dImmediateContext->OMSetRenderTargets(1, &d3d->m_pRenderTargetView, d3d->m_pDepthStencilView);


		d3d->m_ScreenViewport.TopLeftX = 0;
		d3d->m_ScreenViewport.TopLeftY = 0;
		d3d->m_ScreenViewport.Width = rc.right - rc.left;//修改这里会导致d3d图像缩放
		d3d->m_ScreenViewport.Height = rc.bottom - rc.top;//
		d3d->m_ScreenViewport.MinDepth = 0.0f;
		d3d->m_ScreenViewport.MaxDepth = 1.0f;

		m_pd3dImmediateContext->RSSetViewports(1, &d3d->m_ScreenViewport);
		break;
	}

	case WM_TIMER:
	{
		Ex_ObjInvalidateRect(hObj, 0);
		return 0;
	}
	}

	return 0;
}

void test_d3d(HWND hWnd)
{
	HWND hWnd_d3d = Ex_WndCreate(hWnd, L"Ex_DirectUI", L"测试d3d", 0, 0, 500, 500, 0, 0);
	auto hExDui_d3d = Ex_DUIBindWindowEx(hWnd_d3d, 0,
		WINDOW_STYLE_NOINHERITBKG | WINDOW_STYLE_BUTTON_CLOSE |
		WINDOW_STYLE_BUTTON_MIN | WINDOW_STYLE_MOVEABLE |
		WINDOW_STYLE_CENTERWINDOW | WINDOW_STYLE_TITLE |
		WINDOW_STYLE_HASICON,
		0, 0);
	Ex_DUISetLong(hExDui_d3d, ENGINE_LONG_CRBKG, ExARGB(150, 150, 150, 255));
	auto hd3d = Ex_ObjCreateEx(-1, L"static", 0, -1, 50, 50, 400, 400, hExDui_d3d, 0, DT_VCENTER | DT_CENTER, 0, 0, d3d_callback);
	Ex_DUIShowWindow(hExDui_d3d, SW_SHOWNORMAL);
}