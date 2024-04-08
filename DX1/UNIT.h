#pragma once



class Unit
{
public:
    Unit();
    ~Unit();

    class CUP_MESH* mMesh = nullptr;
    class BoxShader* mShader = nullptr;
    Vector3 mPos;
    Matrix mModelTM;

    void Update(float fElapsedTime);
    void Render(ID3D11DeviceContext* pd3dContext);
};

