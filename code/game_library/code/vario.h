#include "base_types.h"
#include "thread_context.h"
#include "platform.h"
#include "game_memory.h"
#include "game_input.h"
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

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_render_commands *RenderCommands)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);

struct game_state
{
    cube_map CubeMap;

    // NOTE: (Ted)  This isn't a part of the state per se. It's for the rendering phase.
    vector_float_3 ModelTranslations[CUBE_MAP_SIZE];
};
