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
