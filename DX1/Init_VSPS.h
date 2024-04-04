
ID3D11VertexShader* g_pVertexShader = nullptr;//버텍스쉐이더
ID3D11InputLayout* g_pVertexLayout = nullptr;//버텍스레이아웃
ID3D11PixelShader* g_pPixelShader = nullptr;//픽셀 셰이더

ID3D11VertexShader* g_pGridVertexShader = nullptr;//그리드 버텍스쉐이더
ID3D11InputLayout* g_pGridVertexLayout = nullptr;//그리드 버텍스레이아웃
ID3D11PixelShader* g_pGridPixelShader = nullptr;//그리드 픽셀 셰이더

HRESULT CompileShaderFromFile(const WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);






