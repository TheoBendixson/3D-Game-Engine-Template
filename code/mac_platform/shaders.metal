
#include <metal_stdlib>
#include <simd/simd.h>

typedef struct
{
    matrix_float4x4 Transform;
    matrix_float4x4 View;
    matrix_float4x4 Projection;
} Uniforms;

typedef enum FlatColorVSAttribute
{
    FlatColorVSPosition  = 0,
    FlatColorVSNormal = 1,
    FlatColorVSColor = 2
} FlatColorVSAttribute;

typedef enum FlatColorPSAttribute
{
    FlatColorPSPosition  = 0,
    FlatColorPSColor = 1
} PSVertexAttribute;

typedef enum TextureVSAttribute
{
    TextureVSPosition  = 0,
    TextureVSNormal = 1,
    TextureVSUV = 2
} TextureVSAttribute;

typedef enum TexturePSAttribute
{
    TexturePSPosition  = 0,
    TexturePSColor = 1
} TexturePSAttribute;

typedef enum BufferIndex
{
    BufferIndexUniforms = 0
}BufferIndex;

typedef struct
{
    float3 position [[attribute(FlatColorVSPosition)]];
    float3 normal   [[attribute(FlatColorVSNormal)]];
    float3 color    [[attribute(FlatColorVSColor)]];
} FlatColorVSInput;

typedef struct
{
    float4 position [[position]];
    float4 color;                                                 
} FlatColorPSInput;

typedef struct
{
    float3 position [[attribute(TextureVSPosition)]];
    float3 normal   [[attribute(TextureVSNormal)]];
    float2 uv       [[attribute(TextureVSUV)]];
} TextureVSInput;

typedef struct
{
    float4 position [[position]];
    float2 uv;                                                 
} TexturePSInput;

using namespace metal;

vertex FlatColorPSInput 
flatColorVertexShader(uint vertexID [[ vertex_id ]],
                      constant FlatColorVSInput *vertexArray [[ buffer(0) ]],
                      constant Uniforms & uniforms [[ buffer(1) ]])
{
    FlatColorVSInput in = vertexArray[vertexID];
    FlatColorPSInput out;

    // NOTE: (Ted)  Don't calculate light for now.
    /*
    float Light = clamp(dot(normalize(mul(float4(input.Normal, 0.0f), Transform).xyz), 
                            normalize(-LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;*/
    // Projection*View*Model*Point 

    out.position = uniforms.Projection*uniforms.View*uniforms.Transform*float4(in.position.xyz, 1.0f);
    out.color = float4(in.color, 1.0f);

    return out;
}

fragment float4 
flatColorFragmentShader(FlatColorPSInput in [[stage_in]])
{
    return in.color;
}

vertex TexturePSInput
textureVertexShader(TextureVSInput in [[stage_in]],
                    constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]])
{
    /*
    float Light = clamp(dot(normalize(mul(float4(input.Normal, 0.0f), Transform).xyz), 
                            normalize(-LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;*/

    TexturePSInput out;
    matrix_float4x4 ModelView = uniforms.Transform*uniforms.View;
    matrix_float4x4 MVPMatrix = ModelView*uniforms.Projection;
    out.position = float4(in.position.xyz, 1.0f)*MVPMatrix;
    out.uv = in.uv;
    return out;
}

fragment float4
textureFragmentShader(TexturePSInput in [[stage_in]],
                      texture2d<float> texture [[ texture(0) ]])
{
    constexpr sampler textureSampler (mag_filter::linear,
                                      min_filter::linear);
    return texture.sample(textureSampler, in.uv);
}
