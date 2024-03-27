#pragma once
#include"MESH.h"
using namespace DirectX;
struct PNTVertex
{
	XMFLOAT3 Pos;
	XMFLOAT3 Norm;
	XMFLOAT2 Tex;
};

class CupMesh
{
public:
	~CupMesh()
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
			return(v == o.v && vt == o.vt && vn == o.vn);
		}

	};
	std::vector< XMFLOAT3 > gV;
	std::vector< XMFLOAT2 > gVT;
	std::vector< XMFLOAT3 > gVN;

	struct SUB
	{
		std::vector< XMFLOAT3 >& mV;
		std::vector< XMFLOAT2 >& mVT;
		std::vector< XMFLOAT3 >& mVN;

		std::vector< FACE > mFaces;

		std::vector< PNTVertex > mVertexs;
		std::vector< WORD > mIndex;

		std::string mName;

		int v_count = 0;
		int vt_count = 0;
		int vn_count = 0;
		int f_count = 0;

		SUB(std::vector< XMFLOAT3 >& v,
			std::vector< XMFLOAT2 >& vt,
			std::vector< XMFLOAT3 >& vn) :
			mV(v), mVT(vt), mVN(vn) {}


		WORD AddFace(FACE f);

		void Report();

		
	};

	std::vector< SUB*> mSubs;
};

class CUP_MESH    //실제로 init하는 함수
{
public:
	class SUB : public MESH {
	public:
		HRESULT Init(CupMesh::SUB* sub);
		virtual HRESULT CreateLayout(ID3DBlob* pVSBlob) override;
		virtual HRESULT Render(ID3D11DeviceContext* pd3dContext) override;
	};

	std::vector< SUB*> m_Subs;
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	class SHADER* m_Shader = nullptr;

	HRESULT Init();
	HRESULT Render(ID3D11DeviceContext* pd3dContext) override;
	void SetShader(SHADER* shader);

protected:
	HRESULT CreateLayout(ID3DBlob* pVSBlob) override;
};
