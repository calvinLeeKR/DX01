#include "pch.h"
#include "CupMesh.h"

CupMesh::~CupMesh() {
    for (int i = 0; i < mSubs.size(); ++i) {
        delete mSubs[i];
        mSubs[i] = nullptr;
    }
}

bool CupMesh::LoadFromFile(const CHAR* fileName)
{
    FILE* stream = nullptr;

    if (fopen_s(&stream, fileName, "rt") != 0) {
        OutputDebugStringA("Cub.obj 파일 열기 실패 !!!");
        return false;             
    }

    char line[260];

    SUB* mesh = nullptr;

    while (fgets(line, 260, stream) != NULL)
    {
        if (line[0] == '#') 
            continue;

        char type[10];
        sscanf_s(line, "%9s", type, (UINT)_countof(type));
        
        if (strcmp("v", type) == 0) {
            if (!mesh) break;
            
            XMFLOAT3 v;
            sscanf_s(line, "%s %f %f %f", type,
                    (UINT)_countof(type), &v.x, &v.y, &v.z);
            mesh->mV.push_back(v);
        }
        else if(strcmp("vt", type) == 0) {
            if (!mesh) break;
            
            XMFLOAT2 v;
            sscanf_s(line, "%s %f %f", type,
                (UINT)_countof(type), &v.x, &v.y);
            mesh->mVT.push_back(v);
        }
        else if (strcmp("vn", type) == 0) {
            if (!mesh) break;

            XMFLOAT3 v;
            sscanf_s(line, "%s %f %f %f",type,(UINT)_countof(type), 
                &v.x, &v.y, &v.z);
            mesh->mVN.push_back(v);
        }
        else if (strcmp("f", type) == 0) {
            if (!mesh) break;

            FACE f1, f2, f3;
            sscanf_s(line, "%s %d/%d/%d %d/%d/%d %d/%d/%d",type,(unsigned)_countof(type),
                &f1.v, &f1.vt, &f1.vn, 
                &f2.v, &f2.vt, &f2.vn, 
                &f3.v, &f3.vt, &f3.vn);
            WORD v1 = mesh->AddFace(f1);
            WORD v2 = mesh->AddFace(f2);
            WORD v3 = mesh->AddFace(f3);

            mesh->mIndex.push_back(v1);
            mesh->mIndex.push_back(v2);
            mesh->mIndex.push_back(v3);
        }
        else if (strcmp("g", type) == 0) {
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

void CupMesh::Report() {
    for (int i = 0; i < mSubs.size(); ++i) {
        OutputDebugStringA("\n");

        OutputDebugStringA(mSubs[i]->mName.c_str());
        OutputDebugStringA("~~~~~~~~~~~~~~~~\n");
        mSubs[i]->Report();
    }
    OutputDebugStringA("\n");
}

WORD CupMesh::SUB::AddFace(FACE f) {

    for (int i = 0; i < mFaces.size(); ++i) {
        if (mFaces[i] == f)
            return f.idx;
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

void CupMesh::SUB::Report() {
    char str[250];
    sprintf_s(str, _countof(str), "v=%d,\n vn=%d,\n vt=%d,\n f=%d\n",
        v_count, vn_count, vt_count, f_count);
    OutputDebugStringA(str);
}
