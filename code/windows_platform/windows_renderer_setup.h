
// SetupInstanceBuffer — creates a D3D11 dynamic vertex buffer for 2D sprite instance data.
ID3D11Buffer *
SetupInstanceBuffer(ID3D11Device* D11Device, texture_draw_command_instance_buffer *GameInstanceBuffer)
{
    u32 InstanceBufferSize = sizeof(texture_draw_command_instance_uniforms)*GameInstanceBuffer->InstanceMax;
    GameInstanceBuffer->InstanceUniforms =
        (texture_draw_command_instance_uniforms *)VirtualAlloc(0, InstanceBufferSize,
                                                               MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    ID3D11Buffer *WindowsInstanceBuffer;
    {
        D3D11_BUFFER_DESC Desc = {};
        Desc.Usage = D3D11_USAGE_DYNAMIC;
        Desc.ByteWidth = InstanceBufferSize;
        Desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

        D3D11_SUBRESOURCE_DATA Initial = {};
        Initial.pSysMem = GameInstanceBuffer->InstanceUniforms;
        HRESULT HR = D11Device->CreateBuffer(&Desc, &Initial, &WindowsInstanceBuffer);
        AssertHR(HR);
    }

    return WindowsInstanceBuffer;
}

// SetupShaderResourceView — creates a D3D11 texture array SRV for 3D textures (single slice).
internal ID3D11ShaderResourceView*
SetupShaderResourceViewFromTexture(ID3D11Device* D11Device, game_texture *Texture)
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
        Data.pSysMem     = Texture->Data;
        Data.SysMemPitch = Texture->Width * 4;

        ID3D11Texture2D* Tex2D;
        HRESULT HR = D11Device->CreateTexture2D(&Desc, &Data, &Tex2D);
        AssertHR(HR);

        HR = D11Device->CreateShaderResourceView((ID3D11Resource*)Tex2D, NULL, &TextureView);
        AssertHR(HR);

        Tex2D->Release();
    }
    return TextureView;
}

// SetupVertexBufferFromSize — creates a D3D11 dynamic vertex buffer of the given byte size.
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
    return VertexBuffer;
}
