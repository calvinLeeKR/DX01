#pragma once
//using Microsoft::WRL::ComPtr;

class ResourceFactory
{
public:
    wchar_t mPath[MAX_PATH];

    void CreateTexture(const wchar_t* texture, ID3D11ShaderResourceView** textureView);

private:
    using TextureCache = std::map< std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> >;
    using ShaderCache = std::map< std::wstring, Microsoft::WRL::ComPtr<ID3D11PixelShader> >;


    TextureCache mTextureCache;

};

