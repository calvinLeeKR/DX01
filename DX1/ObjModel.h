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

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	std::vector< XMFLOAT3 > gV;
	std::vector< XMFLOAT2 > gVT;
	std::vector< XMFLOAT3 > gVN;
	std::vector<PNTVertex>  gVertexs;
	std::vector<WORD> mIndex;

	struct FACE {
		int v, vt, vn;
		int idx;
		bool operator==(const FACE& o) const { return(v == o.v) && (vt == o.vt) && (vn == o.vn); }

	};
	std::vector< FACE > mFaces;

	WORD AddFace(FACE f);

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct SUB
	{
		UINT m_IndexCount = 0;
		UINT m_StartIndexLocation = WORD_MAX;
		UINT m_BaseVertexLocation = 0;

		std::string mName;
	};

	std::vector< SUB*> mSubs;
};

class CUP_MESH    //실제로 init하는 함수
{
public:
	struct SUB_MESH  {
		UINT m_IndexCount = 0;
		UINT m_StartIndexLocation = 0;
		UINT m_BaseVertexLocation = 0;
		std::string mName;

		HRESULT Init(ObjModel::SUB* sub);
		void Render(ID3D11DeviceContext* pd3dContext);
	};

	std::vector< SUB_MESH*> m_SubMeshes;

	UINT          m_VertexCount = 0;
	UINT m_IndexCount = 0;

	ID3D11Buffer* m_pVertexBuffer = nullptr;//버텍스 버퍼
	ID3D11Buffer* m_pIndexBuffer = nullptr; //인덱스 버퍼
	ID3D11InputLayout* m_pVertexLayout = nullptr;
	class SHADER* m_Shader = nullptr;

	HRESULT Init();
	HRESULT InitVertexBuffer(ObjModel& obj);
	HRESULT InitIndexBuffer(ObjModel& obj);
	void Render(ID3D11DeviceContext* pd3dContext);
	void SetShader(SHADER* shader);

protected:
	HRESULT CreateLayout(ID3DBlob* pVSBlob);
};
