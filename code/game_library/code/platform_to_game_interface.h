
#define GAME_LOAD_TEXTURES(name) void name(game_memory *Memory, game_texture_buffer *TextureBuffer)
typedef GAME_LOAD_TEXTURES(game_load_textures);

#define GAME_LOAD_3D_MODELS(name) void name(game_memory *Memory, game_render_commands *RenderCommands)
typedef GAME_LOAD_3D_MODELS(game_load_3D_models);

#define GAME_CLEAR_MEMORY_ARENA(name) void name(game_memory *Memory)
typedef GAME_CLEAR_MEMORY_ARENA(game_clear_memory_arena);

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_render_commands *RenderCommands)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
