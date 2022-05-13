
internal ID3D11ShaderResourceView*
SetupShaderResourceView(ID3D11Device* D11Device, game_texture *Texture)
{
    ID3D11ShaderResourceView* TextureView; 
    {
        D3D11_TEXTURE2D_DESC Desc = {};
        Desc.Width = Texture->Width;
        Desc.Height = Texture->Height;
        Desc.MipLevels = 1;
        Desc.ArraySize = 1;
        Desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        Desc.SampleDesc = { 1, 0 };
        Desc.Usage = D3D11_USAGE_IMMUTABLE;
        Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        D3D11_SUBRESOURCE_DATA Data = {};
        Data.pSysMem            = Texture->Data;
        Data.SysMemPitch        = Texture->Width * 4; // 4 bytes per pixel

        ID3D11Texture2D* Texture;
        HRESULT HR = D11Device->CreateTexture2D(&Desc, &Data, &Texture);
        AssertHR(HR);

        HR = D11Device->CreateShaderResourceView((ID3D11Resource*)Texture, NULL, &TextureView);
        AssertHR(HR);

        Texture->Release();
    }

    return (TextureView);
}

internal ID3D11Buffer*
SetupVertexBufferFromGameVertexBuffer(ID3D11Device* D11Device, u32 VertexBufferSize, 
                                      void *GameVertexBuffer)
{
    ID3D11Buffer* VertexBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.ByteWidth = VertexBufferSize;
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA Initial = {};
        Initial.pSysMem = GameVertexBuffer;
        HRESULT HR = D11Device->CreateBuffer(&Desc, &Initial, &VertexBuffer);
        AssertHR(HR);
    }

    return (VertexBuffer);
}
