#include "base_types.h"
#include "thread_context.h"
#include "platform.h"
#include "game_memory.h"
#include "game_input.h"

// NOTE: (Ted)  On the Mac platform, the game is compiled as a dynamic library and the math library
//              isn't loaded from the platform layer.
#if MACOS
#import "math.h"
#include <simd/simd.h>
#endif

#include "game_math.h"
#include "game_render_commands.h"
#include "game_startup_config.h"
#include "game_sound.h"
#include "cube_map.h"

#define PLATFORM_READ_ENTIRE_FILE(name) read_file_result name(thread_context *Thread, char *Filename)
typedef PLATFORM_READ_ENTIRE_FILE(platform_read_entire_file);

#define PLATFORM_WRITE_ENTIRE_FILE(name) b32 name(thread_context *Thread, char *Filename, u64 FileSize, void *Memory)
typedef PLATFORM_WRITE_ENTIRE_FILE(platform_write_entire_file);

#define PLATFORM_QUIT_GAME(name) void name()
typedef PLATFORM_QUIT_GAME(platform_quit_game);

#define GAME_LOAD_3D_MODELS(name) void name(game_memory *Memory, game_render_commands *RenderCommands)
typedef GAME_LOAD_3D_MODELS(game_load_3D_models);

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_render_commands *RenderCommands)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

struct push_buffer
{
    vector_float_3 Translations[CUBE_MAP_SIZE];
    u32 DrawCount;
};

struct game_state
{
    cube_map CubeMap;

    // TODO: (Ted)  This needs to move to game render commands. It makes more sense
    //              in that memory space.
    push_buffer ColoredCubePushBuffer;
    push_buffer TexturedCubePushBuffer;
};
