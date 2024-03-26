#pragma once
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
    ~CupMesh();

    bool LoadFromFile(const CHAR* fileName);
    void Report();

    struct SUB {
        std::vector< XMFLOAT3 > mV;
        std::vector< XMFLOAT2 > mVT;
        std::vector< XMFLOAT3 > mVN;
        std::vector< PNTVertex > mVertexs;
        std::vector< WORD > mFACE;
        std::string  mName;

        int v_count = 0;
        int vn_count = 0;
        int vt_count = 0;
        int f_count = 0;

        void Report() {
            char str[250];
            sprintf_s(str, _countof(str), "v=%d,\n vn=%d,\n vt=%d,\n f=%d\n",
                v_count, vn_count, vt_count, f_count);
            OutputDebugStringA(str);
        }
    };

    std::vector< SUB* > mSubs;

};

