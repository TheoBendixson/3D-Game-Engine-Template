#include "base_types.h"
#include "thread_context.h"
#include "platform.h"
#include "game_memory.h"
#include "game_input.h"
#include "game_render_commands.h"

#define GAME_UPDATE_AND_RENDER(name) void name(thread_context *Thread, game_memory *Memory, game_input *Input, game_render_commands *RenderCommands)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
