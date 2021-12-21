

#if WINDOWS
struct game_constants
{
    matrix Transform;
    matrix View;
    matrix Projection;
    vector_float_3 LightVector;
};
#elif MACOS
struct game_constants
{
    matrix_float4x4 Transform;
    matrix_float4x4 View;
    matrix_float4x4 Projection;
    vector_float3 LightVector;
};
#endif

#if WINDOWS
struct game_flat_color_vertex
{
    r32 Position[3];
    r32 Normal[3];
    r32 Color[3];
};
#elif MACOS
struct game_flat_color_vertex
{
    vector_float3 Position;
    vector_float3 Normal;
    vector_float3 Color;
};
#endif

struct game_texture_vertex
{
    r32 Position[3];
    r32 Normal[3];
    r32 UV[2];
};

#define MAX_MODELS  3

struct model_range
{
    u32 StartVertex;
    u32 VertexCount;
};

struct game_vertex_buffer
{
    void *Vertices;
    u32 VertexCount;

    model_range ModelRanges[MAX_MODELS];
    u32 ModelCount;
};

struct clear_color
{
    r32 RGBA[4];
};

struct mesh_instance
{
    game_constants Constants;
    u32 ModelIndex;
};

struct mesh_instance_buffer
{
    mesh_instance *Meshes;
    u32 MeshCount;
    u32 MeshMax;
};

struct game_render_commands
{

#if MACOS
    u32 FrameIndex;
    r32 ScreenScaleFactor; 
#endif

    s32 ViewportWidth;
    s32 ViewportHeight;
    clear_color ClearColor;

    game_vertex_buffer FlatColorVertexBuffer;
    game_vertex_buffer TextureVertexBuffer;

    mesh_instance_buffer FlatColorMeshInstances;
    mesh_instance_buffer TexturedMeshInstances;
};
