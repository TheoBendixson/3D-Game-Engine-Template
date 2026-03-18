
PLATFORM_FREE_FILE_MEMORY(PlatformFreeFileMemory)
{
    if (Memory)
    {
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

        if (WriteFile(FileHandle, Memory, (DWORD)FileSize, &BytesWritten, 0))
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
            // IMPORTANT: This will fail if any file is greater than four gigabytes.
            u32 FileSize32 = (u32)FileSize.QuadPart;
            void *FileMemory = VirtualAlloc(0, FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

            DWORD BytesRead = 0;

            if (ReadFile(FileHandle, FileMemory, FileSize32, &BytesRead, 0) &&
                BytesRead == FileSize32)
            {
                Result.Contents = FileMemory;
                Result.ContentsSize = (u64)FileSize32;
            }
            else
            {
                VirtualFree(FileMemory, 0, MEM_RELEASE);
            }
        }

        CloseHandle(FileHandle);
    }

    return Result;
}

PLATFORM_READ_PNG_FILE(PlatformReadPNGFile)
{
    read_file_result Result = {};

    int X, Y, N;
    u32 *ImageData = (u32 *)stbi_load(Filename, &X, &Y, &N, 0);

    if (X > 0 && Y > 0 && ImageData != NULL)
    {
        Result.Contents = ImageData;
        Result.ContentsSize = X * Y * sizeof(u32);
    }

    return Result;
}

PLATFORM_OPEN_FILE_DIALOG(PlatformOpenFileDialog)
{
    IFileOpenDialog *pFileOpen;

    HRESULT HR = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                                  IID_IFileOpenDialog, reinterpret_cast<void**>(&pFileOpen));
    AssertHR(HR);

    ShowSystemCursor();
    HR = pFileOpen->Show(NULL);

    IShellItem *pItem;
    HR = pFileOpen->GetResult(&pItem);

    if (pItem != NULL)
    {
        PWSTR pszFilePath;
        HR = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        AssertHR(HR);

        read_file_result *ReadFileResult = (read_file_result *)TransientStorage;
        ReadFileResult->ContentsSize = 0;

        char *AdjustedFilePath = (char *)malloc(sizeof(char) * (wcslen(pszFilePath) + 1));
        wsprintfA(AdjustedFilePath, "%S", pszFilePath);
        *ReadFileResult = PlatformReadEntireFile(AdjustedFilePath);

        CoTaskMemFree(pszFilePath);
        free(AdjustedFilePath);

        pItem->Release();
        pFileOpen->Release();
    }

    HideSystemCursor();
}

PLATFORM_SAVE_FILE_DIALOG(PlatformSaveFileDialog)
{
    if (FileSaveDialog == NULL)
    {
        HRESULT HR = CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
                                      IID_IFileSaveDialog, reinterpret_cast<void**>(&FileSaveDialog));
        AssertHR(HR);
    }

    ShowSystemCursor();

    FILEOPENDIALOGOPTIONS Options = {};
    FileSaveDialog->SetOptions(Options);

    HRESULT HR = FileSaveDialog->Show(NULL);

    IShellItem *pItem;
    HR = FileSaveDialog->GetResult(&pItem);

    if (SUCCEEDED(HR))
    {
        PWSTR pszFilePath;
        HR = pItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath);
        AssertHR(HR);

        char *AdjustedFilePath = (char *)malloc(sizeof(char) * (wcslen(pszFilePath) + 1));
        wsprintfA(AdjustedFilePath, "%S", pszFilePath);

        PlatformWriteEntireFile(AdjustedFilePath, FileSize, Memory);

        CoTaskMemFree(pszFilePath);
        free(AdjustedFilePath);
        pItem->Release();
    }

    HideSystemCursor();
}
