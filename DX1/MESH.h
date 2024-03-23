#pragma once

class MESH
{
public:
    ~MESH() { Release(); }
    virtual HRESULT Render(ID3D11DeviceContext* pd3dContext) = 0;
    virtual void Release();
    void SetShader(class SHADER* shader);


protected:
	virtual HRESULT CreateLayout(ID3DBlob* pVSBlob) = 0;

    ID3D11Buffer* m_pVertexBuffer = nullptr;//���ؽ� ����
    ID3D11Buffer* m_pIndexBuffer = nullptr; //�ε��� ����
    ID3D11InputLayout* m_pVertexLayout = nullptr;

    INT m_VertexCount = 0;
    INT m_IndexCount = 0;

    class SHADER* m_Shader = nullptr;
};


class BoxMESH : public MESH
{
public:
    HRESULT Init();
    HRESULT Render(ID3D11DeviceContext* pd3dContext) override;

protected:
	HRESULT CreateLayout(ID3DBlob* pVSBlob) override;
};

