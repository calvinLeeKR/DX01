#pragma once

#include "targetver.h"
//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <d3d11.h>//Direct 헤더
#include <d3dcompiler.h>//셰이더를 이용하기위한 헤더
#include <DirectXMath.h>//3차원 계산
#include <DirectXColors.h>//컬러

#include "tk\SimpleMath.h"

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "dxguid.lib")

// using namespace DirectX::SimpleMath;

typedef DirectX::SimpleMath::Vector2   Vector2;
typedef DirectX::SimpleMath::Vector3   Vector3;
typedef DirectX::SimpleMath::Vector4   Vector4;
typedef DirectX::SimpleMath::Matrix    Matrix;
typedef DirectX::SimpleMath::Quaternion   Quaternion;
typedef DirectX::SimpleMath::Plane   XPlane;

#define SAFE_RELESE(x) if(x){(x)->Release(); (x)=nullptr;}

