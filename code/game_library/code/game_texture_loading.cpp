


extern "C"
GAME_LOAD_TEXTURES(GameLoadTextures)
{
    thread_context Thread = {};
    read_file_result Result = Memory->PlatformReadPNGFile("game_character.png");

    if (Result.ContentsSize > 0)
    {
        Texture->Data = Memory->TransientPartition.SecondaryGeneric.Data;
        u8 *Src = (u8 *)Result.Contents;
        u8 *Dest = (u8 *)Texture->Data;

        for(u32 ByteIndex = 0;
            ByteIndex < Result.ContentsSize;
            ByteIndex++)
        {
            *Dest++ = *Src++;
        }

        Texture->Width = 942;
        Texture->Height = 942;
    }

    Memory->PlatformFreeFileMemory(&Thread, Result.Contents);
}
