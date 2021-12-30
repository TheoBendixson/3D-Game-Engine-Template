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
#include "game_texture.h"
#include "game_render_commands.h"
#include "game_startup_config.h"
#include "game_sound.h"
#include "cube_map.h"
#include "platform_to_game_interface.h"

struct push_buffer
{
    vector_float_3 Translations[CUBE_MAP_SIZE];
    u32 DrawCount;
};

struct game_state
{
    memory_arena ScratchArena;

    cube_map CubeMap;

    // TODO: (Ted)  This needs to move to game render commands. It makes more sense
    //              in that memory space.
    push_buffer ColoredCubePushBuffer;
    push_buffer TexturedCubePushBuffer;
};
