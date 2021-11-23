
struct game_constants
{
    matrix Transform;
    matrix View;
    matrix Projection;
    vector_float_3 LightVector;
};

struct game_vertex
{
    r32 Position[3];
    r32 Color[3];
};

struct game_vertex_buffer
{
    game_vertex *Vertices;
    u32 VertexCount;

    u32 *Indices;
    u32 IndexCount;
};

struct clear_color
{
    r32 RGBA[4];
};

struct game_render_commands
{
    s32 ViewportWidth;
    s32 ViewportHeight;
    clear_color ClearColor;

    game_vertex_buffer VertexBuffer;
    game_constants *Constants;
    u32 InstancedMeshCount;
};
