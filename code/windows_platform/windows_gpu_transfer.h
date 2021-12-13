
void
TransferVertexBufferContents(ID3D11DeviceContext* DeviceContext, ID3D11Buffer* D3DVertexBuffer, 
                             game_flat_color_vertex *GameVertexBuffer, u32 VertexBufferSize)
{
    D3D11_MAPPED_SUBRESOURCE Mapped;
    HRESULT HR = DeviceContext->Map((ID3D11Resource*)D3DVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &Mapped);
    AssertHR(HR);
    memcpy(Mapped.pData, GameVertexBuffer, VertexBufferSize);
    DeviceContext->Unmap((ID3D11Resource*)D3DVertexBuffer, 0);
}
