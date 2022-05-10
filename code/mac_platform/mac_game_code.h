
inline time_t
MacGetLastWriteTime(char *Filename)
{
	time_t LastWriteTime = 0;
	
	struct stat StatData = {};

    if (stat(Filename, &StatData) == 0)
    {
        LastWriteTime = StatData.st_mtime;
    }

    return(LastWriteTime);
}

mac_game_code 
MacLoadGameCode(char *SourceDLLName)
{
    mac_game_code Result = {};

    Result.DLLLastWriteTime = MacGetLastWriteTime(SourceDLLName);

    Result.GameCodeDLL = dlopen(SourceDLLName, RTLD_NOW);
    if (Result.GameCodeDLL)
    {
        Result.UpdateAndRender = (game_update_and_render *)
            dlsym(Result.GameCodeDLL, "GameUpdateAndRender");

        Result.Load3DModels = (game_load_3D_models *)
            dlsym(Result.GameCodeDLL, "GameLoad3DModels");

        Result.LoadTextures = (game_load_textures *)
            dlsym(Result.GameCodeDLL, "GameLoadTextures");

        Result.SetupMemoryPartitions = (game_setup_memory_partitions *)
            dlsym(Result.GameCodeDLL, "GameSetupMemoryPartitions");

        Result.ClearMemoryArena = (game_clear_memory_arena *)
            dlsym(Result.GameCodeDLL, "GameClearMemoryArena");

        if (Result.UpdateAndRender &&
            Result.Load3DModels &&
            Result.LoadTextures &&
            Result.SetupMemoryPartitions &&
            Result.ClearMemoryArena)
        {
            Result.IsValid = true;
        } else
        {
            Result.IsValid = false;
        }
    }

    if(!Result.IsValid)
    {
        printf("Dynamic Library Load Error: %s", dlerror());
        Result.UpdateAndRender = 0;
    }

    return Result;
}

void
MacUnloadGameCode(mac_game_code *GameCode)
{
    if(GameCode->GameCodeDLL)
    {
    	dlclose(GameCode->GameCodeDLL);
        GameCode->GameCodeDLL = 0;
    }

    GameCode->IsValid = false;
    GameCode->UpdateAndRender = 0;
    GameCode->Load3DModels = 0;
    GameCode->LoadTextures = 0;
    GameCode->SetupMemoryPartitions = 0;
    GameCode->ClearMemoryArena = 0;
}
