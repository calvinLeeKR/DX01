#include "pch.h"
#include "SHADER.h"
#include"tk\DDSTextureLoader.h"
#include"tk\WICTextureLoader.h"

using namespace DirectX;

ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();



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
			OutputDebugStringA(reinterpret_cast<const char*>(pErrorBlob->GetBufferPointer()));
			pErrorBlob->Release();
		}
		return hr;
	}
	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}



HRESULT Create_ConstantBuffer(ID3D11Buffer** pcb, const void* src, UINT size)
{
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = src;
	//return pd3dDevice->CreateBuffer(&bd, &initData, pcb);
	return pd3dDevice->CreateBuffer(&bd, (src) ? &initData : nullptr, pcb);
}


HRESULT SHADER::Init_VertexShader(const WCHAR* fileName)
{
	HRESULT hr;

	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Compile the vertex shader
	//쉐이더를 프로그램 실행할때마다 컴파일 하지않고 밑에 함수 호출때 컴파일
	ID3DBlob* pVSBlob = nullptr;
	hr = CompileShaderFromFile(fileName, "VS", "vs_4_0", &pVSBlob);//컴파일 pVSBlob에 버텍스쉐이더데이터 입력선언 Shader02.fx 를 여기서만 컴파일함
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader);//데이터 
	if (FAILED(hr))
	{
		pVSBlob->Release();
		return hr;
	}

	m_pVSBlob = pVSBlob;
	//pVSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;
}



HRESULT SHADER::Init_PixelShader(const WCHAR* fileName)
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(fileName, "PS", "ps_4_0", &pPSBlob);//컴파일 pVSBlob에 픽셀쉐이더데이터 입력
	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &m_pPixelShader);
	pPSBlob->Release();
	if (FAILED(hr))
		return hr;

	return S_OK;

}

void SHADER::Release() 
{
	SAFE_RELESE(m_pVSBlob);
	SAFE_RELESE(m_pVertexShader);
	SAFE_RELESE(m_pPixelShader);
}

void BoxShader::SetLightDir(XMFLOAT3 dir) 
{
	m_LightDir.x = dir.x;
	m_LightDir.y = dir.y;
	m_LightDir.z = dir.z;

	CBLightDir cb;
	cb.vLightDir = m_LightDir;
	cb.EyePosition = m_EyePos;

	if (m_XCB_LightDir==nullptr) {
		Create_ConstantBuffer(&m_XCB_LightDir, &cb, sizeof(CBLightDir));
	}
}

void BoxShader::SetViewProjection(XMMATRIX& view, XMMATRIX& proj)
{
	m_View = view;
	m_Proj = proj;

	if (m_XCB_WVP == nullptr)
		Create_ConstantBuffer(&m_XCB_WVP, nullptr, sizeof(CBWorldViewProj));
}

void BoxShader::SetDiffuseTexture(const WCHAR* texName)
{
	HRESULT hr;
	ID3D11Device* pd3dDevice = GetD3DDevice();

	if (m_pTextureDiffuse == nullptr) {	
		int len = wcslen(texName);
		if ( wcscmp(&texName[len - 3], L"dds") == 0 or
			 wcscmp(&texName[len - 3], L"DDS") == 0) 
		{
			hr = CreateDDSTextureFromFile(pd3dDevice, texName, nullptr, &m_pTextureDiffuse);
		}
		else {
			hr = CreateWICTextureFromFile(pd3dDevice, texName, nullptr, &m_pTextureDiffuse);
		}
		if (FAILED(hr)) return;
	}

	if (m_pSamplerLinear == nullptr) {
		D3D11_SAMPLER_DESC sampDesc = {};
		sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sampDesc.MinLOD = 0;
		sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
		hr = pd3dDevice->CreateSamplerState(&sampDesc, &m_pSamplerLinear);
		if (FAILED(hr)) return;
	}
}


void BoxShader::PreRender(ID3D11DeviceContext* pd3dContext)
{
	Update_ConstantBuffers(pd3dContext);


	pd3dContext->VSSetShader(m_pVertexShader, nullptr, 0);  //버텍스 쉐이더 
	if (m_XCB_LightDir)   pd3dContext->VSSetConstantBuffers(0, 1, &m_XCB_LightDir);
	if (m_XCB_WVP) pd3dContext->VSSetConstantBuffers(1, 1, &m_XCB_WVP);

	pd3dContext->PSSetShader(m_pPixelShader, nullptr, 0);   //버텍스 쉐이더
	pd3dContext->PSSetConstantBuffers(0, 1, &m_XCB_LightDir);
	pd3dContext->PSSetConstantBuffers(1, 1, &m_XCB_WVP);
	if (m_pTextureDiffuse) pd3dContext->PSSetShaderResources(0, 1, &m_pTextureDiffuse);
	if (m_pSamplerLinear) pd3dContext->PSSetSamplers(0, 1, &m_pSamplerLinear);

}

void BoxShader::Release() 
{
	SAFE_RELESE(m_XCB_LightDir);
	SAFE_RELESE(m_XCB_WVP);
	SAFE_RELESE(m_pTextureDiffuse);
	SAFE_RELESE(m_pSamplerLinear);

	__super::Release();
}

void BoxShader::Update_ConstantBuffers(ID3D11DeviceContext* pd3dContext)
{
	HRESULT hr;

	if (m_XCB_LightDir) {
		D3D11_MAPPED_SUBRESOURCE MappedResource;
		hr = pd3dContext->Map(m_XCB_LightDir, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		{
			auto pCB = reinterpret_cast<CBLightDir*>(MappedResource.pData);
			XMStoreFloat4(&pCB->EyePosition, XMLoadFloat4(&m_EyePos));
			XMStoreFloat4(&pCB->vLightDir, XMLoadFloat4(&m_LightDir));
			pCB->vLightDir.w = m_LightDir.w;

		}
		pd3dContext->Unmap(m_XCB_LightDir, 0);
	}

	if (m_XCB_WVP) {
		XMMATRIX WVP = m_World * m_View * m_Proj;
		XMMATRIX worldInverse = XMMatrixInverse(nullptr, m_World);

		D3D11_MAPPED_SUBRESOURCE MappedResource;
		hr = pd3dContext->Map(m_XCB_WVP, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
		{
			auto pCB = reinterpret_cast<CBWorldViewProj*>(MappedResource.pData);
			XMStoreFloat4x4(&pCB->WorldViewProj, XMMatrixTranspose(WVP));
			XMStoreFloat4x4(&pCB->World, XMMatrixTranspose(m_World));

			XMStoreFloat4(&pCB->worldInverseTranspose[0], worldInverse.r[0]);
			XMStoreFloat4(&pCB->worldInverseTranspose[1], worldInverse.r[1]);
			XMStoreFloat4(&pCB->worldInverseTranspose[2], worldInverse.r[2]);
		}
		pd3dContext->Unmap(m_XCB_WVP, 0);
	}
}
