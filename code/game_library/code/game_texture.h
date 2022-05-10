
struct game_texture
{
    void *Data;
    u32 Width;
    u32 Height;
};

struct game_texture_buffer
{
    game_texture *Textures;
    u32 Count;
    u32 Max;
};
