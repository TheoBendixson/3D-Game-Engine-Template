
// ---- 2D sprite types ----

enum texture_atlas_type
{
    TextureAtlasType_Tile24,
    TextureAtlasType_Tile32
};

struct float_color
{
    r32 Red, Green, Blue, Alpha;
};

// 2D quad vertex (Position and UV are 2-component floats)
struct sprite_vertex
{
    v2 Position;
    v2 UV;
};

struct texture_draw_command_instance_uniforms
{
    v2  vMin;
    v2  vMax;
    r32 Rotation;
    u32 TextureID;
    r32 Alpha;
    r32 Progress;
};

struct texture_draw_command_instance_buffer
{
    texture_draw_command_instance_uniforms *InstanceUniforms;
    u32 InstanceCount;
    u32 InstanceMax;
};

struct sprite_vertex_buffer
{
    sprite_vertex *Vertices;
    u32 Size;
    u32 VertexCount;
    u32 Max;
};

struct game_texture_draw_command
{
    v2 vMin;
    v2 vMax;
    u32 TextureID;
    r32 Alpha;
    r32 Rotation;
    u32 ZLayer;
    texture_atlas_type TextureAtlasType;
    r32 Progress;
};

struct render_commands_array
{
    game_texture_draw_command *Commands;
    u32 CommandCount;
    u32 MaxCommands;
    texture_atlas_type TextureAtlasType;
    s32 DrawnCount;
    b32 BufferDrawn;
};

#define BOTTOM_LAYER_COUNT 35

struct game_texture_draw_command_buffer
{
    render_commands_array BottomLayers[BOTTOM_LAYER_COUNT];
};

// ---- 3D geometry types ----

#if WINDOWS
struct game_constants
{
    matrix Transform;
    matrix View;
    matrix Projection;
    vector_float_3 LightVector;
};

struct game_flat_color_vertex
{
    r32 Position[3];
    r32 Normal[3];
    r32 Color[3];
};

struct game_texture_vertex
{
    r32 Position[3];
    r32 Normal[3];
    r32 UV[2];
};

#elif MACOS
struct game_constants
{
    matrix_float4x4 View;
    matrix_float4x4 Projection;
    vector_float3 LightVector;
};

struct instance_uniforms
{
    matrix_float4x4 Transform;
};

struct game_flat_color_vertex
{
    vector_float3 Position;
    vector_float3 Normal;
    vector_float3 Color;
};

struct game_texture_vertex
{
    vector_float3 Position;
    vector_float3 Normal;
    vector_float2 UV;
};

#endif

#define MAX_MODELS 3

struct model_range
{
    u32 StartVertex;
    u32 VertexCount;
};

struct model_range_indexed
{
    u32 StartIndex;
    u32 IndexCount;
};

struct game_vertex_buffer
{
    void *Vertices;
    u32 VertexCount;

    model_range ModelRanges[MAX_MODELS];
    u32 ModelCount;
};

struct game_indexed_vertex_buffer
{
    void *Vertices;
    u32 VertexCount;

    void *Indices;
    u32 IndexCount;

    r32 ModelHeight;
};

struct clear_color
{
    r32 RGBA[4];
};

struct mesh_instance
{
#if WINDOWS
    game_constants Constants;
#elif MACOS
    instance_uniforms Uniforms;
#endif
    u32 ModelIndex;
};

struct mesh_instance_buffer
{
    mesh_instance *Meshes;
    u32 MeshCount;
    u32 MeshMax;
};

struct push_buffer
{
    vector_float_3 Translations[CUBE_MAP_SIZE];
    u32 DrawCount;
};

// ---- Combined render commands ----

struct game_render_commands
{
#if MACOS
    u32 FrameIndex;
    r32 ScreenScaleFactor;
    game_constants Constants;
#endif

    s32 ViewportWidth;
    s32 ViewportHeight;
    clear_color ClearColor;

    // 2D sprite pipeline
    memory_arena DrawCommandsArena;
    game_texture_draw_command_buffer DrawCommandsBuffer;
    sprite_vertex_buffer SpriteVertexBuffer;
    texture_draw_command_instance_buffer InstanceBuffer;

    // 3D mesh pipeline
    game_vertex_buffer FlatColorVertexBuffer;
    game_vertex_buffer TextureVertexBuffer;
    game_indexed_vertex_buffer LoadedModelVertexBuffer;

    mesh_instance_buffer FlatColorMeshInstances;
    mesh_instance_buffer TexturedMeshInstances;
    mesh_instance_buffer LoadedModelMeshInstances;

    push_buffer ColoredCubePushBuffer;
    push_buffer TexturedCubePushBuffer;
};
