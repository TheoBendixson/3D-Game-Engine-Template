
#include <simd/simd.h>

typedef enum BufferIndex
{
    BufferIndexMeshPositions = 0,
    BufferIndexMeshGenerics  = 1,
    BufferIndexUniforms      = 2
}BufferIndex;

typedef enum VertexAttribute
{
    VertexAttributePosition  = 0,
    VertexAttributeTexcoord  = 1,
}VertexAttribute;

typedef enum TextureIndex
{
    TextureIndexColor    = 0,
}TextureIndex;

// NOTE: (Ted)  On Metal, all matrices are in column major order!
typedef struct
{
    matrix_float4x4 projectionMatrix;
    matrix_float4x4 modelViewMatrix;
} Uniforms;
