#include "pch.h"
#include "ObjModel.h"
#include "SHADER.h"
#include "UNIT.h"

#define SAFE_DELETE(x) if(x){ delete (x); (x)=nullptr;}


Unit::Unit()
{
    mModelTM = Matrix::Identity;
    mPos = Vector3(0, 0, 0);

    mShader = new BoxShader;
    mShader->Init();
    mShader->SetAmbientLight(0.2f);
    mShader->SetLightDir(XMFLOAT3(-0.577f, 0.577f, -0.577f));
    mShader->SetDiffuseTexture(L"cup.jpg");
    mShader->SetWorld(Matrix::Identity);

    mMesh = new CUP_MESH;
    mMesh->Init();
    mMesh->SetShader(mShader);
}

Unit::~Unit()
{
    SAFE_DELETE(mShader);
    SAFE_DELETE(mMesh);
}

void Unit::Update(float fElapsedTime)
{
}

void Unit::Render(ID3D11DeviceContext* d3dContext)
{
    Matrix w = Matrix::CreateTranslation(mPos);

    mShader->SetWorld(mModelTM * w);
    mShader->Apply(d3dContext);
    mMesh->Render(d3dContext);
}
