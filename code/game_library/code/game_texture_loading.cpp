
struct game_texture_description
{
    u32 Width;
    u32 Height;
    char *Filename;
};

internal void
LoadTexture(game_memory *Memory, memory_arena *Arena, thread_context *Thread,
            game_texture *Texture, game_texture_description Description)
{
    read_file_result Result = Memory->PlatformReadPNGFile(Description.Filename);

    if (Result.ContentsSize > 0)
    {
        Texture->Width = Description.Width;
        Texture->Height = Description.Height;
        u32 PixelCount = Texture->Width*Texture->Height;

        Texture->Data = PushArray(Arena, PixelCount, u32);
        u8 *Src = (u8 *)Result.Contents;
        u8 *Dest = (u8 *)Texture->Data;

        for(u32 ByteIndex = 0;
            ByteIndex < Result.ContentsSize;
            ByteIndex++)
        {
            *Dest++ = *Src++;
        }
    }

    Memory->PlatformFreeFileMemory(Thread, Result.Contents);
}

extern "C"
GAME_LOAD_TEXTURES(GameLoadTextures)
{
    game_state *GameState = (game_state *)Memory->PermanentStorage;
    memory_arena *ScratchArena = &GameState->ScratchArena;
    memory_partition *SecondaryPartition = &Memory->TransientPartition.SecondaryGeneric;

    InitializeArena(ScratchArena, SecondaryPartition->Size, SecondaryPartition->Data);
    TextureBuffer->Textures = PushArray(ScratchArena, TextureBuffer->Max, game_texture);

    thread_context Thread = {};

    game_texture_description Description = {};
    Description.Width = 942;
    Description.Height = 942;
    Description.Filename = "game_character.png";

    LoadTexture(Memory, ScratchArena, &Thread, &TextureBuffer->Textures[0], Description);

    //Result = Memory->PlatformReadPNGFile("grid_surface.png");
}
