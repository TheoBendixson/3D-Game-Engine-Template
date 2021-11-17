
struct VS_INPUT                                                                
{                                                                              
    float3 position : POSITION;         
    float4 color    : COLOR;                                                 
};                                                                             

struct PS_INPUT                                                                
{                                                                              
    float4 position : POSITION;         
    float4 color    : COLOR;                                                 
};                                                                             

cbuffer cbuffer0 : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    float2 ViewportSize;                                                       
}                                                                              

sampler sampler0 : register(s0);                 // s0 = sampler bound to slot 0

PS_INPUT vs(VS_INPUT input)                                                    
{
    PS_INPUT output;
    output.position = float4(input.position.xyz, 1.f);                               
    output.color = input.color;                                                      
    return output;                                                             
}                                                                              

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    return float4(input.color[0], input.color[1], input.color[2], input.color[3]);
}
