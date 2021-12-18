
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

        if (Result.UpdateAndRender)
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
}
