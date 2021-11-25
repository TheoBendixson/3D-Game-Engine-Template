
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
    r32 Normal[3];
    r32 Color[3];
};

#define MAX_MODELS  3

struct model_range
{
    u32 StartVertex;
    u32 VertexCount;
};

struct game_vertex_buffer
{
    game_vertex *Vertices;
    u32 VertexCount;

    model_range ModelRanges[MAX_MODELS];
    u32 ModelCount;
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
