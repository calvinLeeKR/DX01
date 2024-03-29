#pragma once



class MESH
{
public:
    ~MESH() { Release(); }
    virtual void Render(ID3D11DeviceContext* pd3dContext) = 0;
    virtual void Release();
    void SetShader(class SHADER* shader);

	virtual HRESULT CreateLayout(ID3DBlob* pVSBlob) = 0;

    ID3D11Buffer* m_pVertexBuffer = nullptr;//버텍스 버퍼
    ID3D11Buffer* m_pIndexBuffer = nullptr; //인덱스 버퍼
    ID3D11InputLayout* m_pVertexLayout = nullptr;

    UINT m_VertexCount = 0;
    UINT m_IndexCount = 0;
    UINT m_StartIndexLocation = 0;
    UINT m_BaseVertexLocation = 0;

    class SHADER* m_Shader = nullptr;
};


class BoxMESH : public MESH
{
public:
    HRESULT Init();
    void Render(ID3D11DeviceContext* pd3dContext) override;

protected:
	HRESULT CreateLayout(ID3DBlob* pVSBlob) override;
};



