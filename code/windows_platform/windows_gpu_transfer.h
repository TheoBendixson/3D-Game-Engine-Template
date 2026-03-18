
// Transfer a raw byte buffer to a D3D11 dynamic vertex buffer.
void
TransferVertexBufferContents(ID3D11DeviceContext* DeviceContext, ID3D11Buffer* D3DVertexBuffer,
                             void *GameVertexBuffer, u32 VertexBufferSize)
{
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)D3DVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, GameVertexBuffer, VertexBufferSize);
    DeviceContext->Unmap((ID3D11Resource*)D3DVertexBuffer, 0);
}

// Transfer the 2D sprite vertex buffer (6 quad vertices) to GPU.
internal void
CopyVertexBuffer(ID3D11DeviceContext* DeviceContext, ID3D11Buffer* WindowsVertexBuffer,
                 sprite_vertex_buffer *VertexBuffer)
{
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)WindowsVertexBuffer, 0,
                                     D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, VertexBuffer->Vertices, VertexBuffer->Size);
    DeviceContext->Unmap((ID3D11Resource*)WindowsVertexBuffer, 0);
}

// Transfer the per-instance uniforms for 2D sprite batching.
void
TransferInstanceBufferToGPU(ID3D11DeviceContext* DeviceContext,
                            ID3D11Buffer *WindowsInstanceBuffer,
                            texture_draw_command_instance_buffer *GameInstanceBuffer)
{
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)WindowsInstanceBuffer, 0,
                                     D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, GameInstanceBuffer->InstanceUniforms,
           GameInstanceBuffer->InstanceCount * sizeof(texture_draw_command_instance_uniforms));
    DeviceContext->Unmap((ID3D11Resource*)WindowsInstanceBuffer, 0);
}

// Update the viewport-size constant buffer used by the 2D sprite vertex shader.
void
CopyD3D11ShaderConstantsToGPU(ID3D11DeviceContext* DeviceContext, ID3D11Buffer* D3D11ConstantsBuffer,
                              r32 WindowWidth, r32 WindowHeight, v2 TextureSize)
{
    float RenderPassConstants[] = { WindowWidth, WindowHeight,
                                    TextureSize.X, TextureSize.Y };
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)D3D11ConstantsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, RenderPassConstants, sizeof(RenderPassConstants));
    DeviceContext->Unmap((ID3D11Resource*)D3D11ConstantsBuffer, 0);
}

// Update the texture-size constant buffer used by the 2D sprite pixel shader.
void
TransferTextureSizeToGPU(ID3D11DeviceContext* DeviceContext, ID3D11Buffer* TextureSizeConstantBuffer,
                         v2 TextureSize)
{
    float Constants[] = { TextureSize.X, TextureSize.Y };
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)TextureSizeConstantBuffer,
                                     0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, Constants, sizeof(Constants));
    DeviceContext->Unmap((ID3D11Resource*)TextureSizeConstantBuffer, 0);
}
