
extern "C"
GAME_LOAD_TEXTURES(GameLoadTextures)
{
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    memory_arena *ScratchArena = &GameState->ScratchArena;
    memory_partition *SecondaryPartition = &Memory->TransientPartition.SecondaryGeneric;

    InitializeArena(ScratchArena, SecondaryPartition->Size, SecondaryPartition->Data);
    game_texture *Textures = PushArray(ScratchArena, 2, game_texture);

    thread_context Thread = {};
    read_file_result Result = Memory->PlatformReadPNGFile("game_character.png");

    if (Result.ContentsSize > 0)
    {
        game_texture *Texture = Textures;
        Texture->Width = 942;
        Texture->Height = 942;
        u32 PixelCount = Texture->Width*Texture->Height;

        Texture->Data = PushArray(ScratchArena, PixelCount, u32);
        u8 *Src = (u8 *)Result.Contents;
        u8 *Dest = (u8 *)Texture->Data;

        for(u32 ByteIndex = 0;
            ByteIndex < Result.ContentsSize;
            ByteIndex++)
        {
            *Dest++ = *Src++;
        }
    }

    Memory->PlatformFreeFileMemory(&Thread, Result.Contents);

    return Textures;
}
