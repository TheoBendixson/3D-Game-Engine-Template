
#define GAME_LOAD_TEXTURES(name) void name(game_memory *Memory, game_texture *Texture)
typedef GAME_LOAD_TEXTURES(game_load_textures);

#define GAME_LOAD_3D_MODELS(name) void name(game_memory *Memory, game_render_commands *RenderCommands)
typedef GAME_LOAD_3D_MODELS(game_load_3D_models);

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_render_commands *RenderCommands)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
