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
#include "cube_map.h"
#include "game_render_commands.h"
#include "game_startup_config.h"
#include "game_sound.h"
#include "platform_to_game_interface.h"

enum game_demo_mode
{
    GameDemoModeStandard,
    GameDemoModeRotateWorld,
    GameDemoModeScalePlayer,
    GameDemoModeRotatePlayer
};

struct game_state
{
    memory_arena ScratchArena;
    cube_map CubeMap;
    cube_map_position PlayerP;
    u32 ActionSlopFrames;
    game_demo_mode DemoModes[4];
    u32 DemoModeIndex;
    r32 PersonScaleMultiplier;
    b32 ReverseScale;
    r32 PersonRotation;
};
