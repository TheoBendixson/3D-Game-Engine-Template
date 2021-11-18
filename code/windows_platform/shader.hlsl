
struct VS_INPUT                                                                
{                                                                              
    float3 position : POSITION;         
    float3 color    : COLOR;                                                 
};                                                                             

struct PS_INPUT                                                                
{                                                                              
    float4 position : POSITION;         
    float4 color    : COLOR;                                                 
};                                                                             

cbuffer ModelViewProjectionConstantBuffer : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    matrix World;
    matrix View;
    matrix Projection;
}                                                                              

PS_INPUT vs(VS_INPUT input)                                                    
{
    PS_INPUT output;

    float4 position = float4(input.position, 1.0f);
    //position = mul(position, World);
    position = mul(position, View);
    position = mul(position, Projection);
    output.position = position;
    output.color = float4(input.color, 1.0f);

    return output;
}                                                                              

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    return input.color;
}
