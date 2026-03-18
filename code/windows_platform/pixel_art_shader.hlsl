
struct VS_INPUT                                                                
{                                                                              
    float2 pos      : POSITION;       
    float2 uv       : TEXCOORD;                                                 
};

struct PS_INPUT
{
    float4 pos      : SV_POSITION;
    float2 uv       : TEXCOORD;
    uint textureID  : TEXTUREID;
    float alpha     : ALPHA;
    float progress  : PROGRESS;
};

struct VS_INSTANCE
{
    float2 vMin      : VMIN;
    float2 vMax      : VMAX;
    float  Rotation  : ROTATION;
    uint   textureID : TEXTUREID;
    float  alpha     : ALPHA;
    float  progress  : PROGRESS;
};

cbuffer cbuffer0 : register(b0)      // b0 = constant buffer bound to slot 0
{                                                                              
    float2 ViewportSize;                                                       
}                                                                              

cbuffer cbuffer1 
{
    float2 TextureSize;                                                       
}

sampler sampler0 : register(s0);                
Texture2DArray<float4> texture0 : register(t0);

PS_INPUT vs(VS_INPUT input, VS_INSTANCE instance)
{
    PS_INPUT output;
    float2 scale   = instance.vMax - instance.vMin;
    float2 scaled  = input.pos * scale;
    float  c       = cos(instance.Rotation);
    float  s       = sin(instance.Rotation);
    float2 rotated = float2(c * scaled.x + s * scaled.y,
                           -s * scaled.x + c * scaled.y);
    float2 center  = float2(instance.vMin.x + 0.5 * scale.x,
                            (ViewportSize.y - instance.vMin.y) - 0.5 * scale.y);
    float2 worldPos = rotated + center;
    output.pos = float4(worldPos, 0.f, 1.f);
    output.pos.xy = (output.pos.xy / (ViewportSize / 2.0)) - 1;
    output.uv = input.uv;
    output.textureID = instance.textureID;
    output.alpha = instance.alpha;
    output.progress = instance.progress;
    return output;
}

float4 ps(PS_INPUT input) : SV_TARGET                                          
{                                                                              
    float2 Pixel = input.uv*TextureSize;
    float2 Seam = floor(Pixel + 0.5);
    float2 DuDv = fwidth(Pixel);
    Pixel = Seam + clamp((Pixel - Seam)/DuDv, -0.5, 0.5);
    float2 ModifiedTextureCoordinate = Pixel/TextureSize;
    float4 tex = texture0.Sample(sampler0, float3(ModifiedTextureCoordinate, input.textureID)); 

    if (tex.a < .95)
    {
        discard;
    }

    if (input.progress < 1.0f)
    {
        float2 c = input.uv - float2(0.5f, 0.5f);
        float angle = atan2(c.x, c.y);
        float clockwiseAngle = angle < 0.0f ? angle + 6.2831853f : angle;
        if (clockwiseAngle >= input.progress * 6.2831853f)
        {
            discard;
        }
    }

    tex = float4(tex[0], tex[1], tex[2], input.alpha*tex[3]);
    return tex;
}                                                                              

