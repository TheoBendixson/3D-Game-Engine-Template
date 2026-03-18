
static void
FailAsyncFileIO(async_file_io *IO)
{
    _InterlockedExchange((long *)&IO->HasError, 1);
    _InterlockedExchange((long *)&IO->IsComplete, 1);
}

static void
LaunchFileIOWorker(async_file_io *IO, b32 IsSave, LPTHREAD_START_ROUTINE WorkerProc)
{
    PrepareAsyncFileIO(IO, IsSave);
    CreateThread(0, 0, WorkerProc, IO, 0, 0);
}

DWORD WINAPI
SaveFileWorkerProc(void *Parameter)
{
    async_file_io *IO = (async_file_io *)Parameter;

    HANDLE File = CreateFileA(IO->FilePath, GENERIC_WRITE, 0, 0,
                              CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (File == INVALID_HANDLE_VALUE)
    {
        FailAsyncFileIO(IO);
        return 1;
    }

    u8   *Cursor    = (u8 *)IO->DataBuffer;
    u64   Remaining = IO->DataSize;
    u64   Written   = 0;
    u32   ChunkSize = 65536;

    while (Remaining > 0)
    {
        u32   ToWrite = (Remaining < (u64)ChunkSize) ? (u32)Remaining : ChunkSize;
        DWORD BytesWritten = 0;
        if (!WriteFile(File, Cursor, ToWrite, &BytesWritten, 0) || BytesWritten != ToWrite)
        {
            CloseHandle(File);
            FailAsyncFileIO(IO);
            return 1;
        }
        Cursor    += BytesWritten;
        Written   += BytesWritten;
        Remaining -= BytesWritten;
        IO->Progress = (r32)Written / (r32)IO->DataSize;
    }

    CloseHandle(File);
    _InterlockedExchange((long *)&IO->IsComplete, 1);
    return 0;
}

DWORD WINAPI
LoadFileWorkerProc(void *Parameter)
{
    async_file_io *IO = (async_file_io *)Parameter;

    HANDLE File = CreateFileA(IO->FilePath, GENERIC_READ, FILE_SHARE_READ, 0,
                              OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
    if (File == INVALID_HANDLE_VALUE)
    {
        FailAsyncFileIO(IO);
        return 1;
    }

    LARGE_INTEGER FileSize;
    if (!GetFileSizeEx(File, &FileSize))
    {
        CloseHandle(File);
        FailAsyncFileIO(IO);
        return 1;
    }

    u32  TotalBytes = (u32)FileSize.QuadPart;
    u8  *Dest       = (u8 *)IO->DataBuffer;
    u64  ReadSoFar  = 0;
    u32  ChunkSize  = 65536;
    u32  Remaining  = TotalBytes;

    while (Remaining > 0)
    {
        u32   ToRead = (Remaining < ChunkSize) ? Remaining : ChunkSize;
        DWORD BytesRead = 0;
        if (!ReadFile(File, Dest + ReadSoFar, ToRead, &BytesRead, 0) || BytesRead != ToRead)
        {
            CloseHandle(File);
            FailAsyncFileIO(IO);
            return 1;
        }
        ReadSoFar  += BytesRead;
        Remaining  -= BytesRead;
        IO->Progress = (r32)ReadSoFar / (r32)TotalBytes;
    }

    IO->DataSize = (u64)TotalBytes;
    CloseHandle(File);
    _InterlockedExchange((long *)&IO->IsComplete, 1);
    return 0;
}

PLATFORM_LAUNCH_ASYNC_SAVE(PlatformLaunchAsyncSave)
{
    LaunchFileIOWorker(IO, true, SaveFileWorkerProc);
}

PLATFORM_LAUNCH_ASYNC_LOAD(PlatformLaunchAsyncLoad)
{
    LaunchFileIOWorker(IO, false, LoadFileWorkerProc);
}
