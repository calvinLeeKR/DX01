#pragma once
// #include "tk\ConstantBuffer.h"
using namespace DirectX;

class SHADER
{
public:
    ~SHADER() { Release(); }
    virtual void Release();
    virtual void PreRender(ID3D11DeviceContext* pd3dContext) = 0;

    HRESULT InitFromFile(const WCHAR* fileName) {
        HRESULT hr;
        hr = Init_VertexShader(fileName); if (FAILED(hr)) return hr;
        hr = Init_PixelShader(fileName); if (FAILED(hr)) return hr;
        return S_OK;
    }

    ID3DBlob* GetVSBlob() const { return m_pVSBlob; }

protected:
	HRESULT Init_VertexShader(const WCHAR* fileName);
	HRESULT Init_PixelShader(const WCHAR* fileName);

	ID3DBlob* m_pVSBlob = nullptr;
	ID3D11VertexShader* m_pVertexShader = nullptr;//πˆ≈ÿΩ∫Ω¶¿Ã¥ı
	ID3D11PixelShader* m_pPixelShader = nullptr;//«»ºø ºŒ¿Ã¥ı

};





class BoxShader : public SHADER
{
public:
    const WCHAR* m_fileName = L"PNT.fx";

    HRESULT Init() {
        return InitFromFile(m_fileName);
    }

    void SetLightDir(XMFLOAT3 dir);
    void SetAmbientLight(float a) { m_LightDir.w = a; }
    void SetWorld(XMMATRIX& m) { m_World = m; }
    void SetViewProjection(XMMATRIX& view, XMMATRIX& proj);
    void SetDiffuseTexture(const WCHAR* texName);

    void PreRender(ID3D11DeviceContext* pd3dContext) override;
    void Release() override;

protected:
    void Update_ConstantBuffers(ID3D11DeviceContext* pd3dContext);

    struct CBLightDir
    {
        XMFLOAT4 vLightDir;
    };

    struct CBWorldViewProj
    {
        XMFLOAT4X4 WorldViewProj;
        XMFLOAT4X4 World;
    };

    XMFLOAT4 m_LightDir = { -0.577f, 0.577f, -0.577f, 0.5f };
    XMMATRIX m_World;
    XMMATRIX m_View;
    XMMATRIX m_Proj;

    ID3D11Buffer* m_XCB_LightDir = nullptr;
    ID3D11Buffer* m_XCB_WVP = nullptr;

    ID3D11ShaderResourceView* m_pTextureDiffuse = nullptr;
    ID3D11SamplerState* m_pSamplerLinear = nullptr;

    //ConstantBuffer<CBWorldViewProj, D3D11_BIND_CONSTANT_BUFFER> mCBuffer;

};




