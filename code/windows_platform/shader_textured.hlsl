struct VS_INPUT                                                                
{                                                                              
    float3 Position : POSITION;         
    float3 Normal   : NORMAL;
    float2 UV       : UV;                                                 
};                                                                             

struct PS_INPUT                                                                
{                                                                              
    float4 Position : SV_POSITION;         
    float2 UV       : UV;                                                 
};                                                                             

cbuffer ModelViewProjectionConstantBuffer : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    row_major float4x4 Transform;
    row_major float4x4 View;
    row_major float4x4 Projection;
    float3 LightVector;
}                                                                              

Texture2D Texture: register(t0);
SamplerState Sampler: register(s0);

PS_INPUT vs(VS_INPUT input)                                                    
{
    float Light = clamp(dot(normalize(mul(float4(input.Normal, 0.0f), Transform).xyz), 
                            normalize(-LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;

    PS_INPUT output;

    row_major float4x4 ModelView = mul(Transform, View);
    row_major float4x4 MVPMatrix = mul(ModelView, Projection);
    output.Position = mul(float4(input.Position.xyz, 1.0f), MVPMatrix);
    output.UV = input.UV;
    return output;
}                                                                              

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    return Texture.Sample(Sampler, input.UV);
}
