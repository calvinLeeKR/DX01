#include"tk\DDSTextureLoader.h"
#include"tk\WICTextureLoader.h"
using namespace DirectX;

HRESULT Init_BlendState()

{

	D3D11_BLEND_DESC desc = {};

	ZeroMemory(&desc, sizeof(D3D11_BLEND_DESC));

	desc.RenderTarget[0].BlendEnable = TRUE;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;//합성

	//알파값 정의
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;	//
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;	//
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;	//

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	HRESULT hr = pd3dDevice->CreateBlendState(&desc, &g_pBlendState);

	return hr;

}

