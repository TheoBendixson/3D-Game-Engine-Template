
struct VS_INPUT                                                                
{                                                                              
    float3 position : POSITION;         
    float3 color    : COLOR;                                                 
};                                                                             

struct PS_INPUT                                                                
{                                                                              
    float4 position : SV_POSITION;         
    float4 color    : COLOR;                                                 
};                                                                             

cbuffer ModelViewProjectionConstantBuffer : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    row_major float4x4 Transform;
    row_major float4x4 Projection;
}                                                                              

PS_INPUT vs(VS_INPUT input)                                                    
{
    PS_INPUT output;
    output.position = mul(float4(input.position, 1.0f), mul(Transform, Projection));
    output.color = float4(input.color, 1.0f);
    return output;
}                                                                              

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    return input.color;
}
