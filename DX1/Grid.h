
using namespace DirectX;

struct LineVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

struct CB_WVP
{
	XMMATRIX mWorld;
	XMMATRIX mView;
	XMMATRIX mProjection;
};

ID3D11InputLayout* g_pVertexLayout_Line = nullptr;
ID3D11Buffer* g_pVertexBuffer_Line = nullptr;
ID3D11VertexShader* g_pVertexShader_Line = nullptr;
ID3D11PixelShader* g_pPixelShader_Line = nullptr;
ID3D11Buffer* g_pCB_WVP = nullptr;

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


HRESULT Init_Line()
{
    HRESULT hr;
	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"Line.fxh", "VS", "vs_4_0", &pVSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"Line.fxh file cannot be compiled.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader_Line);
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = g_pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pVertexLayout_Line);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;



	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(L"Line.fxh", "PS", "ps_4_0", &pPSBlob);
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader_Line);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	// Create vertex buffer
    LineVertex vertices[] =
	{
		{ XMFLOAT3(-5.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(-5.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(5.0f, 0.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },

		{ XMFLOAT3(-1.0f, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 0.0f, -5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(1.0f, 0.0f, 5.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },

		{ XMFLOAT3(0.0f, -10.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
		{ XMFLOAT3(0.0f, 10.0f, 0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(LineVertex) * 14;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertices;
	hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer_Line);
	if (FAILED(hr))
		return hr;


	// Create the constant buffer
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(CB_WVP);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCB_WVP);
	if (FAILED(hr))
		return hr;


    return S_OK;

}

void Draw_Line()
{
	// Set the input layout


	// Set vertex buffer
	UINT stride = sizeof(LineVertex);
	UINT offset = 0;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer_Line, &stride, &offset);

	// Set index buffer
	// g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// Set primitive topology
	g_pImmediateContext->IASetInputLayout(g_pVertexLayout_Line);
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//
	// Update variables
	//
    g_World = XMMatrixIdentity();

    CB_WVP cb;
	cb.mWorld = XMMatrixTranspose(g_World);
	cb.mView = XMMatrixTranspose(g_View);
	cb.mProjection = XMMatrixTranspose(g_Projection);
	g_pImmediateContext->UpdateSubresource(g_pCB_WVP, 0, nullptr, &cb, 0, 0);

	//
	// Renders a Line
    //
	g_pImmediateContext->VSSetShader(g_pVertexShader_Line, nullptr, 0);
	g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCB_WVP);
	g_pImmediateContext->PSSetShader(g_pPixelShader_Line, nullptr, 0);
	g_pImmediateContext->Draw(14, 0);
}
