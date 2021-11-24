
struct VS_INPUT                                                                
{                                                                              
    float3 Position : POSITION;         
    float3 Normal   : NORMAL;
    float3 Color    : COLOR;                                                 
};                                                                             

struct PS_INPUT                                                                
{                                                                              
    float4 Position : SV_POSITION;         
    float4 Color    : COLOR;                                                 
};                                                                             

cbuffer ModelViewProjectionConstantBuffer : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    row_major float4x4 Transform;
    row_major float4x4 View;
    row_major float4x4 Projection;
    float3 LightVector;
}                                                                              

PS_INPUT vs(VS_INPUT input)                                                    
{
    float Light = clamp(dot(normalize(mul(float4(input.Normal, 0.0f), Transform).xyz), 
                            normalize(-LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;

    PS_INPUT output;

    row_major float4x4 ModelView = mul(Transform, View);
    output.Position = mul(float4(input.Position, 1.0f), mul(ModelView, Projection));
    output.Color = float4(input.Color * Light, 1.0f);
    return output;
}                                                                              

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    return input.Color;
}
