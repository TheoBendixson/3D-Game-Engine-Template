
#include <metal_stdlib>
#include <simd/simd.h>

#include "mac_shader_types.h"

typedef struct
{
    matrix_float4x4 Transform;
} PerInstanceUniforms;

typedef struct
{
    matrix_float4x4 View;
    matrix_float4x4 Projection;
    vector_float3 LightVector;
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
                      uint instanceID [[ instance_id ]], 
                      constant FlatColorVSInput *vertexArray [[ buffer(BufferIndexVertices) ]],
                      constant PerInstanceUniforms *perInstanceUniforms [[ buffer(BufferIndexPerInstanceUniforms) ]],
                      constant Uniforms & uniforms [[ buffer(BufferIndexConstantUniforms) ]])
{
    FlatColorVSInput in = vertexArray[vertexID];
    FlatColorPSInput out;

    PerInstanceUniforms instanceUniform = perInstanceUniforms[instanceID];
    float Light = clamp(dot(normalize(instanceUniform.Transform*float4(in.normal, 0.0f)).xyz, 
                            normalize(-uniforms.LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;

    out.position = uniforms.Projection*uniforms.View*instanceUniform.Transform*float4(in.position.xyz, 1.0f);
    out.color = float4(in.color*Light, 1.0f);

    return out;
}

fragment float4 
flatColorFragmentShader(FlatColorPSInput in [[stage_in]])
{
    return in.color;
}

vertex TexturePSInput
textureVertexShader(uint vertexID [[ vertex_id ]],
                    uint instanceID [[ instance_id ]],
                    constant TextureVSInput *vertexArray [[ buffer(BufferIndexVertices) ]],
                    constant PerInstanceUniforms *perInstanceUniforms [[ buffer(BufferIndexPerInstanceUniforms) ]],
                    constant Uniforms & uniforms [[ buffer(BufferIndexConstantUniforms) ]])
{
    TextureVSInput in = vertexArray[vertexID];
    TexturePSInput out;
    PerInstanceUniforms instanceUniform = perInstanceUniforms[instanceID];
    out.position = uniforms.Projection*uniforms.View*instanceUniform.Transform*float4(in.position.xyz, 1.0f);
    out.uv = in.uv;
    return out;
}

// NOTE: (Ted)  Use this to draw the model's UV coordinates in the top right corner of the screen.
//              out.position = float4(in.uv.x, in.uv.y, 0.0f, 1.0f);

fragment float4
textureFragmentShader(TexturePSInput in [[stage_in]],
                      texture2d<float> texture [[ texture(0) ]])
{
    constexpr sampler textureSampler (mag_filter::nearest,
                                      min_filter::nearest);
    return texture.sample(textureSampler, in.uv);
}

// NOTE: (Ted)  Use this to draw white pixels. It's helpful when debugging UV wrapping.
//              return float4(1.0f, 1.0f, 1.0f, 1.0f);
