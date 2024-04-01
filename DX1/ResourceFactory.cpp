#include "pch.h"
#include "ResourceFactory.h"
#include"tk\DDSTextureLoader.h"
#include"tk\WICTextureLoader.h"
using namespace DirectX;
using namespace DirectX::DX11;

ID3D11Device* GetD3DDevice();
ID3D11DeviceContext* GetD3DContext();

void ResourceFactory::CreateTexture(const wchar_t* name, ID3D11ShaderResourceView** textureView)
{
    ID3D11Device* pd3dDevice = GetD3DDevice();

    auto it = mTextureCache.find(name);

    if (it != mTextureCache.end())
    {
        ID3D11ShaderResourceView* srv = it->second.Get();
        srv->AddRef();
        *textureView = srv;
    }
    else
    {
        wchar_t fullName[MAX_PATH] = {};
        wcscpy_s(fullName, mPath);
        wcscat_s(fullName, name);

        wchar_t ext[_MAX_EXT] = {};
        _wsplitpath_s(name, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT);
        const bool isdds = _wcsicmp(ext, L".dds") == 0;

        if (isdds)
        {
            HRESULT hr = CreateDDSTextureFromFileEx(
                pd3dDevice, fullName, 0,
                D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
                DDS_LOADER_DEFAULT, nullptr, textureView);
            if (FAILED(hr))
            {
                ErrMessageBoxF(L"ERROR: CreateDDSTextureFromFile failed (%08X) for '%ls'\n",
                    static_cast<unsigned int>(hr), fullName);
            }
        }
        else
        {
            HRESULT hr = CreateWICTextureFromFileEx(
                pd3dDevice, fullName, 0,
                D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
                WIC_LOADER_DEFAULT, nullptr, textureView);
            if (FAILED(hr))
            {
                ErrMessageBoxF(L"ERROR: CreateWICTextureFromFile failed (%08X) for '%ls'\n",
                    static_cast<unsigned int>(hr), fullName);
            }
        }

        if (*name && it == mTextureCache.end())
        {
            TextureCache::value_type v(name, *textureView);
            mTextureCache.insert(v);
        }
    }
}
