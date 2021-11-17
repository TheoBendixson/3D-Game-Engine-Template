struct game_vertex
{
    r32 Position[3];
    r32 Color[4];
};

struct game_vertex_buffer
{
    game_vertex *Vertices;
    u32 VertexCount;
};

struct game_render_commands
{
    s32 ViewportWidth;
    s32 ViewportHeight;

    game_vertex_buffer VertexBuffer;
};
