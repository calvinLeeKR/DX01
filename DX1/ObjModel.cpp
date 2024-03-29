#include "pch.h"
#include "ObjModel.h"
#include "SHADER.h"

using namespace DirectX;

ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();

bool ObjModel::LoadFromFile(const CHAR* fileName)
{
	FILE* stream = nullptr;

	if (fopen_s(&stream, fileName, "rt") != 0)
	{
		OutputDebugStringA("Cub.obj 파일 열기 실패!!!");
		return false;
	}
	
	char line[260];

	SUB* mesh = nullptr;


	
	while (fgets(line, 260, stream) != NULL)
	{
		if (line[0] == '#') 
		{ continue; }
		char type[10];

		sscanf_s(line, "%9s", type, (UINT)_countof(type));
		
		if (strcmp("v", type) == 0)
		{
			if (!mesh) {break;}
			XMFLOAT3 v;
			sscanf_s(line, "%s %f %f %f", type, (UINT)_countof(type), &v.x, &v.y, &v.z);
			gV.push_back(v);
		}
		else if (strcmp("vt", type) == 0)
		{
			if (!mesh) { break; }
			XMFLOAT2 v;
			sscanf_s(line, "%s %f %f ", type, (UINT)_countof(type), &v.x, &v.y);
			gVT.push_back(v);
		}
		else if (strcmp("vn", type) == 0)
		{
			if (!mesh) { break; }
			XMFLOAT3 v;
			sscanf_s(line, "%s %f %f %f", type, (UINT)_countof(type), &v.x, &v.y, &v.z);
			gVN.push_back(v);			
		}
		else if (strcmp("f", type) == 0)
		{
			if (!mesh) { break; }
			
			FACE f1, f2, f3;
			sscanf_s(line, " %s %d/%d/%d %d/%d/%d %d/%d/%d", type, (unsigned)_countof(type), &f1.v, &f1.vt, &f1.vn, &f2.v, &f2.vt, &f2.vn, &f3.v, &f3.vt, &f3.vn);
			WORD v1 = AddFace(f1);
			WORD v2 = AddFace(f2);
			WORD v3 = AddFace(f3);

			if (mesh->m_StartIndexLocation == WORD_MAX) mesh->m_StartIndexLocation = mIndex.size();
			mIndex.push_back(v1);
			mIndex.push_back(v2);
			mIndex.push_back(v3);
			mesh->m_IndexCount += 3;
							
		}
		else if (strcmp("g", type) == 0)
		{
			char name[64] = {};
			sscanf_s(line, "%s %63s", type, (UINT)_countof(type), name, (UINT)_countof(name));

			mesh = new SUB;
			mesh->mName = name;
			
			mSubs.push_back(mesh);

		}
	}

	fclose(stream);

	
	return true;
}


WORD ObjModel::AddFace(FACE f)
{
	
 	for (int i = 0; i < mFaces.size(); i++)
 	{
 		if (mFaces[i] == f)
 		{
 			return mFaces[i].idx;
 		}
 	}
	
	f.idx = gVertexs.size();

	PNTVertex v;
	v.Pos = gV[f.v - 1];
	v.Norm = gVN[f.vn - 1];
	v.Tex = gVT[f.vt - 1];
	
	gVertexs.push_back(v);

	mFaces.push_back(f);
	return f.idx;
}


HRESULT CUP_MESH::SUB_MESH::Init(ObjModel::SUB* sub)
{
	m_IndexCount = sub->m_IndexCount;
	m_StartIndexLocation = sub->m_StartIndexLocation;
	m_BaseVertexLocation = sub->m_BaseVertexLocation;
	mName = sub->mName;

	return S_OK;
}

void CUP_MESH::Render(ID3D11DeviceContext* pd3dContext)
{
	if (m_pVertexLayout == nullptr && m_Shader)//레이아웃이 안만들어 있을경우 
		CreateLayout(m_Shader->GetVSBlob());

	if (m_Shader)
	    m_Shader->PreRender(pd3dContext);

	UINT stride = sizeof(PNTVertex);
	UINT offset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dContext->IASetInputLayout(m_pVertexLayout);         //레이아웃 인풋

	for (SUB_MESH* sub : m_SubMeshes) {
		sub->Render(pd3dContext);
	}

}

void CUP_MESH::SetShader(SHADER* shader)
{
	m_Shader = shader;

	if (m_Shader->GetVSBlob())
		CreateLayout(m_Shader->GetVSBlob());
}

HRESULT CUP_MESH::CreateLayout(ID3DBlob* pVSBlob)
{

	ID3D11Device* pd3dDevice = GetD3DDevice();
	HRESULT hr;
	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{  "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{"TEXCOORD", 0,    DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = pd3dDevice->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &m_pVertexLayout);
	if (FAILED(hr)) {
		MessageBoxA(0, "CUP_MESH::CreateLayout CreateInputLayout 실패!!", "Error", MB_OK);
		return hr;
	}

	return S_OK;
}

HRESULT CUP_MESH::Init()
{
	ObjModel objModel;
	objModel.LoadFromFile("cup.txt");
	InitVertexBuffer(objModel);
	InitIndexBuffer(objModel);

	for (int i = 0; i < objModel.mSubs.size(); ++i) {
		SUB_MESH* subM = new SUB_MESH;
		subM->Init(objModel.mSubs[i]);

		m_SubMeshes.push_back(subM);
	}
	return S_OK;
}

HRESULT CUP_MESH::InitVertexBuffer(ObjModel& obj)
{
	ID3D11Device* pd3dDevice = GetD3DDevice();
	HRESULT hr;

	m_VertexCount = obj.gVertexs.size();
	PNTVertex* PNTs = new PNTVertex[m_VertexCount];

	for (int i = 0; i < m_VertexCount; ++i) {
		PNTs[i] = obj.gVertexs[i];
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNTVertex) * m_VertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = PNTs;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	delete[] PNTs;

	if (FAILED(hr)) {
		MessageBoxA(0, "CUP_MESH::InitVertexBuffer VertexBuffer 생성 실패!!", "Error", MB_OK);
	}

	return hr;

}

HRESULT CUP_MESH::InitIndexBuffer(ObjModel& obj)
{
	ID3D11Device* pd3dDevice = GetD3DDevice();
	HRESULT hr;

	m_IndexCount = obj.mIndex.size();
	WORD* idxs = new WORD[m_IndexCount];
	for (int i = 0; i < m_IndexCount; ++i) {
		idxs[i] = obj.mIndex[i];
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_IndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = idxs;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	delete[] idxs;

	if (FAILED(hr)) {
		MessageBoxA(0, "CUP_MESH:: IndexBuffer 생성 실패!!", "Error", MB_OK);
	}

	return hr;
}


void CUP_MESH::SUB_MESH::Render(ID3D11DeviceContext* pd3dContext)
{
	pd3dContext->DrawIndexed(m_IndexCount, m_StartIndexLocation, 0);
}
