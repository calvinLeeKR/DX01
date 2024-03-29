#pragma once
#include"MESH.h"
using namespace DirectX;
struct PNTVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT2 Tex;
};

class ObjModel
{
public:
	~ObjModel()
	{
		for (int i = 0; i < mSubs.size(); i++)
		{
			delete mSubs[i];
			mSubs[i] = nullptr;
		}
	}
	bool LoadFromFile(const CHAR* fileName);
	void Report();
	struct FACE {
		int v, vt, vn;
		int idx;
		bool operator==(const FACE& o) const
		{
			return(v == o.v) && (vt == o.vt) && (vn == o.vn);
		}

	};
	std::vector< XMFLOAT3 > gV;
	std::vector< XMFLOAT2 > gVT;
	std::vector< XMFLOAT3 > gVN;
	std::vector<PNTVertex>  gVertexs;
	std::vector< FACE > mFaces;

	WORD AddFace(FACE f);

	struct SUB
	{
		std::vector<WORD> mIndex;
		std::string mName;
		void Report();
	};

	std::vector< SUB*> mSubs;
};

class CUP_MESH    //실제로 init하는 함수
{
public:
	class SUB_MESH : public MESH {
	public:
		HRESULT InitIndexBuffer(ObjModel::SUB* sub);
		virtual HRESULT CreateLayout(ID3DBlob* pVSBlob) override;
		virtual void Render(ID3D11DeviceContext* pd3dContext) override;
	};

	std::vector< SUB_MESH*> m_SubMeshes;

	UINT          m_VertexCount = 0;
	ID3D11Buffer* m_pVertexBuffer = nullptr;//버텍스 버퍼
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	class SHADER* m_Shader = nullptr;

	HRESULT Init();
	HRESULT InitVertexBuffer(ObjModel& obj);
	void Render(ID3D11DeviceContext* pd3dContext);
	void SetShader(SHADER* shader);

protected:
	HRESULT CreateLayout(ID3DBlob* pVSBlob);
};
