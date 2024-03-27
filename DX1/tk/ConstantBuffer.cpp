#include "pch.h"
#include "ConstantBuffer.h"

using namespace DirectX;

//--------------------------------------------------------------------------------------
ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();

HRESULT Create_ConstantBuffer1(ID3D11Buffer** pcb, UINT type, const void* src, UINT size)
{
	ID3D11Device* pd3dDevice = GetD3DDevice();

	// Create the constant buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = size;
	bd.BindFlags = type;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = src;
	//return pd3dDevice->CreateBuffer(&bd, &initData, pcb);
	return pd3dDevice->CreateBuffer(&bd, (src) ? &initData : nullptr, pcb);
}

