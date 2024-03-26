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
            ++ mesh->v_count;
        }
        else if(strcmp("vt", type) == 0) {
            ++mesh->vt_count;
        }
        else if (strcmp("vn", type) == 0) {
            ++mesh->vn_count;
        }
        else if (strcmp("f", type) == 0) {
            ++mesh->f_count;
        }
        else if (strcmp("g", type) == 0) {
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

void CupMesh::Report() {
    for (int i = 0; i < mSubs.size(); ++i) {
        OutputDebugStringA("\n");

        OutputDebugStringA(mSubs[i]->mName.c_str());
        OutputDebugStringA("~~~~~~~~~~~~~~~~\n");
        mSubs[i]->Report();
    }
    OutputDebugStringA("\n");
}
