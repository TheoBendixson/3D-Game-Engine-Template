
#include <metal_stdlib>
#include <simd/simd.h>

typedef struct
{
    matrix_float4x4 Transform;
    matrix_float4x4 View;
    matrix_float4x4 Projection;
    float3 LightVector;
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

using namespace metal;

vertex FlatColorPSInput 
flatColorVertexShader(FlatColorVSInput in [[stage_in]],
                      constant Uniforms & uniforms [[ buffer(BufferIndexUniforms) ]])
{
    FlatColorPSInput out;

    // NOTE: (Ted)  Don't calculate light for now.
    /*
    float Light = clamp(dot(normalize(mul(float4(input.Normal, 0.0f), Transform).xyz), 
                            normalize(-LightVector)), 0.0f, 1.0f) * 0.8f + 0.2f;*/

    matrix_float4x4 ModelView = uniforms.Transform*uniforms.View;
    matrix_float4x4 MVPMatrix = ModelView*uniforms.Projection;
    out.position = float4(in.position.xyz, 1.0f)*MVPMatrix;
    out.color = float4(in.color, 1.0f);

    return out;
}

fragment float4 
flatColorFragmentShader(FlatColorPSInput in [[stage_in]])
{
    return in.color;
}
