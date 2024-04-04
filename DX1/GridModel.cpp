#include "pch.h"
#include "GridModel.h"
using namespace DirectX;

ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();
HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

struct LineVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};



#define GRIDGAP 1.0f


HRESULT GridModel::Create_VB() {
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Create vertex buffer
	LineVertex vertices[] =
	{
		{ XMFLOAT3(-5.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (1) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (1) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (2) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (2) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (3) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (3) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (4) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (4) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (5) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (5) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (-1) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (-1) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (-2) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (-2) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (-3) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (-3) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (-4) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (-4) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-5.0f, 0.0f, (-5) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, (-5) * GRIDGAP), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		//--------------------------------------------------------------------------//

		{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((1) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((1) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((2) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((2) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((3) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((3) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((4) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((4) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((5) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((5) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((-1) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((-1) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((-2) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((-2) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((-3) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((-3) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((-4) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((-4) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3((-5) * GRIDGAP, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3((-5) * GRIDGAP, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 5.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};

	m_VertexCount = 46;

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(LineVertex) * m_VertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, m_VertexBuffer.GetAddressOf());
	if (FAILED(hr))
		return hr;

	return S_OK;
}


void GridModel::Render(ID3D11DeviceContext* pd3dContext)
{
	pd3dContext->IASetInputLayout(m_InputLayout.Get());
	pd3dContext->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	pd3dContext->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	pd3dContext->Draw(m_VertexCount, 0);
}

void GridModel::SetMatrix(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj)
{
}



HRESULT GridModel::Create_VShader()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Compile the vertex shader
	//쉐이더를 프로그램 실행할때마다 컴파일 하지않고 밑에 함수 호출때 컴파일
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"Line.fx", "VS", "vs_4_0", &pVSBlob);//컴파일 pVSBlob에 버텍스쉐이더데이터 입력선언 Shader02.fx 를 여기서만 컴파일함
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"The Line.fx file cannot be compiled.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, m_VertexShader.GetAddressOf());//데이터 
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), m_InputLayout.GetAddressOf());
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT GridModel::Create_PShader()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"Line.fx", "PS", "ps_4_0", &pPSBlob);//컴파일 pVSBlob에 픽셀쉐이더데이터 입력
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The Line.fx file cannot be compiled.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, m_PixelShader.GetAddressOf());
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;

}




HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;

	// Disable optimizations to further improve shader debugging
	dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);
	if (FAILED(hr))
	{
		if (pErrorBlob)
		{
			MessageBoxA(nullptr, 
				reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()), "Error", MB_OK);
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}

HRESULT WVPConstantBuffer::Init()
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	
	// bd.Usage = D3D11_USAGE_DYNAMIC;
	// bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	hr = pd3dDevice->CreateBuffer(&bd, nullptr, m_DxCB.GetAddressOf() );
	if (FAILED(hr)) {
		MessageBox(nullptr, L"WVPConstantBuffer::Init Error", L"Error", MB_OK);
	}

	return hr;
}

void WVPConstantBuffer::Apply(ID3D11DeviceContext* pd3dContext)
{
	XMMATRIX World = XMMatrixIdentity();
	CB cb;
	cb = mCB;
	cb.World = XMMatrixTranspose(mCB.World);
	cb.View = XMMatrixTranspose(mCB.View);
	cb.Proj = XMMatrixTranspose(mCB.Proj);
	pd3dContext->UpdateSubresource(m_DxCB.Get(), 0, nullptr, &cb, 0, 0);
	pd3dContext->VSSetConstantBuffers(0, 1, m_DxCB.GetAddressOf() );
}
