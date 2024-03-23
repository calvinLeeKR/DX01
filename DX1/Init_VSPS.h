
ID3D11VertexShader* g_pVertexShader = nullptr;//버텍스쉐이더
ID3D11InputLayout* g_pVertexLayout = nullptr;//버텍스레이아웃
ID3D11PixelShader* g_pPixelShader = nullptr;//픽셀 셰이더

ID3D11VertexShader* g_pGridVertexShader = nullptr;//그리드 버텍스쉐이더
ID3D11InputLayout* g_pGridVertexLayout = nullptr;//그리드 버텍스레이아웃
ID3D11PixelShader* g_pGridPixelShader = nullptr;//그리드 픽셀 셰이더

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);



HRESULT Init_GridVertexShader()
{
	HRESULT hr;

	// Compile the vertex shader
	//쉐이더를 프로그램 실행할때마다 컴파일 하지않고 밑에 함수 호출때 컴파일
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(L"Line.fx", "VS", "vs_4_0", &pVSBlob);//컴파일 pVSBlob에 버텍스쉐이더데이터 입력선언 Shader02.fx 를 여기서만 컴파일함
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The Line.fx file cannot be compiled.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pGridVertexShader);//데이터 
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
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &g_pGridVertexLayout);
	pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;
}

HRESULT Init_GridPixelShader()
{
	HRESULT hr;

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
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pGridPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;

}

void Render_GridShader()
{
	// Set the input layout
	pd3dContext->IASetInputLayout(g_pGridVertexLayout);         //그리드 레이아웃 인풋
	pd3dContext->VSSetShader(g_pGridVertexShader, nullptr, 0);  //그리드 버텍스 쉐이더 
	pd3dContext->PSSetShader(g_pGridPixelShader, nullptr, 0);   //그리드 버텍스 쉐이더
}