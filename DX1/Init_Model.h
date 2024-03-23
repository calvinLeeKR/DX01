using namespace DirectX;

ID3D11Buffer* g_pGridVertexBuffer = nullptr;//그리드버텍스 버퍼

#define GRIDGAP 1.0f

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT2 Tex;
};

struct LineVertex 
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};



HRESULT Init_GridModel() {
    HRESULT hr;
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

    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(LineVertex) * 46;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData = {};
    InitData.pSysMem = vertices;
    hr = pd3dDevice->CreateBuffer(&bd, &InitData, &g_pGridVertexBuffer);
    if (FAILED(hr))
        return hr;

	return S_OK;
}

void Render_GridModel()
{
    // Model
    UINT stride = sizeof(LineVertex);
    UINT offset = 0;
    pd3dContext->IASetVertexBuffers(0, 1, &g_pGridVertexBuffer, &stride, &offset);
    pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

    DirectX::SimpleMath::Vector3 v3;
}


#include "MESH.h"

