#include "pch.h"
#include "CupMesh.h"
#include "SHADER.h"

using namespace DirectX;

ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();

bool CupMesh::LoadFromFile(const CHAR* fileName)
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
			mesh->mV.push_back(v);

			mesh->v_count++;
		}
		else if (strcmp("vt", type) == 0)
		{
			if (!mesh) { break; }
			XMFLOAT2 v;
			sscanf_s(line, "%s %f %f ", type, (UINT)_countof(type), &v.x, &v.y);
			mesh->mVT.push_back(v);
			
			mesh->vt_count++;
		}
		else if (strcmp("vn", type) == 0)
		{
			if (!mesh) { break; }
			XMFLOAT3 v;
			sscanf_s(line, "%s %f %f %f", type, (UINT)_countof(type), &v.x, &v.y, &v.z);
			mesh->mVN.push_back(v);
			
			mesh->vn_count++;
		}
		else if (strcmp("f", type) == 0)
		{
			if (!mesh) { break; }
			
			FACE f1, f2, f3;
			sscanf_s(line, " %s %d/%d/%d %d/%d/%d %d/%d/%d", type, (unsigned)_countof(type), &f1.v, &f1.vt, &f1.vn, &f2.v, &f2.vt, &f2.vn, &f3.v, &f3.vt, &f3.vn);
			WORD v1 = mesh->AddFace(f1);
			WORD v2 = mesh->AddFace(f2);
			WORD v3 = mesh->AddFace(f3);

			mesh->mIndex.push_back(v1);
			mesh->mIndex.push_back(v2);
			mesh->mIndex.push_back(v3);
							
		}
		else if (strcmp("g", type) == 0)
		{
			char name[64] = {};
			sscanf_s(line, "%s %63s", type, (UINT)_countof(type), name, (UINT)_countof(name));

			mesh = new SUB(gV,gVT,gVN);
			mesh->mName = name;
			
			mSubs.push_back(mesh);

		}
	}

	fclose(stream);

	
	return true;
}

void CupMesh::Report()
{
	for (int i = 0; i < mSubs.size(); i++)
	{
		
		OutputDebugStringA(mSubs[i]->mName.c_str());
		OutputDebugStringA("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		mSubs[i]->Report();
		
	}
	OutputDebugStringA("\n");
}

WORD CupMesh::SUB::AddFace(FACE f)
{
	
	for (int i = 0; i < mFaces.size(); i++)
	{
		if (mFaces[i] == f)
		{
			return f.idx;
		}
	}

	f.idx = mVertexs.size();

	PNTVertex v;
	v.Pos = mV[f.v - 1];
	v.Norm = mVN[f.vn - 1];
	v.Tex = mVT[f.vt - 1];
	
	mVertexs.push_back(v);

	mFaces.push_back(f);
	return f.idx;
}

void CupMesh::SUB::Report()
{
	char str[250];
	sprintf_s(str, _countof(str), "v=%d, vt=%d, vn=%d, f=%d\n", v_count, vt_count, vn_count, f_count);
	OutputDebugStringA(str);
}

HRESULT CUP_MESH::SUB::Init(CupMesh::SUB* sub)
{
	ID3D11Device* pd3dDevice = GetD3DDevice();
	HRESULT hr;

	m_VertexCount = sub->mVertexs.size();
	PNTVertex* vertexes = new PNTVertex[m_VertexCount];

	for (int i = 0; i < m_VertexCount; ++i) {
		vertexes[i] = sub->mVertexs[i];
	}

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(PNTVertex) * m_VertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData = {};
	InitData.pSysMem = vertexes;
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pVertexBuffer);
	if (FAILED(hr))
		return hr;

	delete[] vertexes;
	// Create index buffer
	// Create vertex buffer
	

	m_IndexCount = sub->mIndex.size();

	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(WORD) * m_IndexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	InitData.pSysMem = &sub->mIndex[0];
	hr = pd3dDevice->CreateBuffer(&bd, &InitData, &m_pIndexBuffer);
	if (FAILED(hr))
		return hr;


	return S_OK;
}

HRESULT CUP_MESH::Render(ID3D11DeviceContext* pd3dContext)
{
	if (m_pVertexLayout == nullptr && m_Shader)//레이아웃이 안만들어 있을경우 
		CreateLayout(m_Shader->GetVSBlob());
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
	if (FAILED(hr))
		return hr;

	for (SUB* sub : m_Subs) {
		sub->m_pVertexLayout = m_pVertexLayout;
		sub->m_pVertexLayout->AddRef();
	}

	return S_OK;
}

HRESULT CUP_MESH::SUB::CreateLayout(ID3DBlob* pVSBlob)
{
	return E_NOTIMPL;
}

HRESULT CUP_MESH::SUB::Render(ID3D11DeviceContext* pd3dContext)
{


	//if (m_Shader)
	//    m_Shader->PreRender(pd3dContext);

	UINT stride = sizeof(PNTVertex);
	UINT offset = 0;
	pd3dContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	pd3dContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
	pd3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pd3dContext->IASetInputLayout(m_pVertexLayout);         //레이아웃 인풋

	pd3dContext->DrawIndexed(m_IndexCount, 0, 0);

	return S_OK;
}
