
PLATFORM_FREE_FILE_MEMORY(PlatformFreeFileMemory) 
{
    if (Memory) {
        VirtualFree(Memory, 0, MEM_RELEASE);
    }
}

PLATFORM_WRITE_ENTIRE_FILE(PlatformWriteEntireFile)
{
    b32 FileWritten = false;

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);

    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten;

        if (WriteFile(FileHandle, Memory, FileSize, &BytesWritten, 0))
        {
            FileWritten = true;
        } 

        CloseHandle(FileHandle);
    }

    return FileWritten;
}

PLATFORM_READ_ENTIRE_FILE(PlatformReadEntireFile) 
{
    read_file_result Result = {};

    HANDLE FileHandle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            // IMPORTANT: (Ted) This will fail if any file loaded is greater than four gigabytes!
            u32 FileSize32 = (u32)FileSize.QuadPart;
            void *FileMemory = VirtualAlloc(0, FileSize32, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

            DWORD BytesRead = 0;

            if (ReadFile(FileHandle, FileMemory, FileSize32, &BytesRead, 0) &&
                BytesRead == FileSize32)
            {
                Result.Contents = FileMemory;
                Result.ContentsSize = (u64)FileSize32;
                Result.Filename = (char *)VirtualAlloc(0, 200*sizeof(char), 
                                                       MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);

                char *Dest = Result.Filename;
                char *Scan = Filename;

                while (*Scan != '\0')
                {
                    *Dest++ = *Scan++;
                }

                *Dest++ = '\0';

            } else
            {
                Result.Contents = 0;
                Result.ContentsSize = 0;
            }
        }

        CloseHandle(FileHandle);

    } 

    return (Result);
}
