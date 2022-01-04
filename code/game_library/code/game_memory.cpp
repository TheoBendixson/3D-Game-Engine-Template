
extern "C"
GAME_CLEAR_MEMORY_ARENA(GameClearMemoryArena)
{
    Assert(sizeof(game_state) <= Memory->PermanentStorageSize);
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    ClearMemoryPartition(&Memory->TransientPartition.SecondaryGeneric);
    GameState->ScratchArena = {};
}
