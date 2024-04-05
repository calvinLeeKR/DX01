#pragma once

using namespace Microsoft;
using namespace DirectX;

class GridModel
{
protected:
    HRESULT Create_VB();
    HRESULT Create_VShader();
    HRESULT Create_PShader();

    WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
    UINT m_VertexCount = 0;

    WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
    WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
    WRL::ComPtr<ID3D11InputLayout> m_InputLayout;

public:
    void Init() {
        Create_VB();
        Create_VShader();
        Create_PShader();
    }
    void SetMatrix(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj);
    void Render(ID3D11DeviceContext* pd3dContext);
};

class WVPConstantBuffer
{
public:
    HRESULT Init();
    void Apply(ID3D11DeviceContext* pd3dContext);

    struct CB
    {
        XMMATRIX World;
        XMMATRIX View;
        XMMATRIX Proj;
    } mCB;

    WRL::ComPtr<ID3D11Buffer> m_DxCB;

};

