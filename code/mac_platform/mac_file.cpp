
// NOTE: (Ted)  For some reason, this is the only way to load a level from a file
//              dialog without any data corruption. I think this is because it bypasses
//              some sort of finder entitlements nonsense.
read_file_result 
MacReadEntireFileFromDialog(char *Filename)
{
    read_file_result Result = {};

    FILE *FileHandle = fopen(Filename, "r+");

    if(FileHandle != NULL)
    {
		fseek(FileHandle, 0, SEEK_END);
		u64 FileSize = ftell(FileHandle);
        if(FileSize)
        {
        	rewind(FileHandle);
            Result.Contents = malloc(FileSize);

            if(Result.Contents)
            {
                u64 BytesRead = fread(Result.Contents, 1, FileSize, FileHandle);
                if(FileSize == BytesRead)
                {
                    Result.ContentsSize = FileSize;
                    Result.Filename = (char *)malloc(200*sizeof(char));

                    char *Dest = Result.Filename;
                    char *Scan = Filename;

                    while (*Scan != '\0')
                    {
                        *Dest++ = *Scan++;
                    }

                    *Dest++ = '\0';
                }
                else
                {                    
                    NSLog(@"File loaded size mismatch. FileSize: %llu, BytesRead: %llu",
                          FileSize, BytesRead);

                    if (Result.Contents)
                    {
                        free(Result.Contents);
                    }

                    Result.Contents = 0;
                }
            }
            else
            {
                NSLog(@"Missing Result Contents Pointer from file load.");
            }
        }
        else
        {
            NSLog(@"Missing File Size from file load");
        }

        fclose(FileHandle);
    }
    else
    {
        NSLog(@"Unable to acquire File handle");
    }

    return (Result);
}

read_file_result 
MacReadEntireFile(char *Filename)
{
    read_file_result Result = {};

    NSString *Filepath = [[NSString alloc] initWithCString: Filename encoding: NSUTF8StringEncoding];
    NSData *FileData = [[NSFileManager defaultManager] contentsAtPath: Filepath];
  
    Result.Contents = (void *)FileData.bytes;
    Result.ContentsSize = (u64)FileData.length;

    return (Result);
}

PLATFORM_READ_ENTIRE_FILE(PlatformReadEntireFile) 
{
    mac_app_path Path = {};
    MacBuildAppFilePath(&Path);

    char SandboxFilename[MAC_MAX_FILENAME_SIZE];
    char LocalFilename[MAC_MAX_FILENAME_SIZE];
    sprintf(LocalFilename, "Contents/Resources/%s", Filename);
    MacBuildAppPathFileName(&Path, LocalFilename,
                            sizeof(SandboxFilename), SandboxFilename);

    read_file_result Result = MacReadEntireFile(SandboxFilename);

    if (Result.ContentsSize > 0)
    {
        Result.Filename = (char *)malloc(200*sizeof(char));

        char *Dest = Result.Filename;
        char *Scan = Filename;

        while (*Scan != '\0')
        {
            *Dest++ = *Scan++;
        }

        *Dest++ = '\0';
    } else
    {
        NSLog(@"No contents loaded");
    }

    return(Result);
}

b32 MacWriteEntireFile(char *Filename, u64 FileSize, void *Memory)
{
    NSString *Filepath = [[NSString alloc] initWithCString: Filename encoding: NSUTF8StringEncoding];
    NSData *FileData = [NSData dataWithBytes: Memory length: FileSize];
    b32 Written = [[NSFileManager defaultManager] createFileAtPath: Filepath
                                                             contents: FileData
                                                           attributes: nil];

    return (Written);
}

PLATFORM_WRITE_ENTIRE_FILE(PlatformWriteEntireFile)
{
    b32 Result = false;

    mac_app_path Path = {};
    MacBuildAppFilePath(&Path);

    char BundleFilename[MAC_MAX_FILENAME_SIZE];
    char LocalFilename[MAC_MAX_FILENAME_SIZE];

    sprintf(LocalFilename, "Contents/%s", Filename);

    MacBuildAppPathFileName(&Path, LocalFilename,
                            sizeof(BundleFilename), BundleFilename);

    Result = MacWriteEntireFile(BundleFilename, FileSize, Memory);

    return(Result);
}

PLATFORM_FREE_FILE_MEMORY(PlatformFreeFileMemory) 
{
    if (Memory) {
        free(Memory);
    }
}

PLATFORM_READ_PNG_FILE(PlatformReadPNGFile)
{
    mac_app_path Path = {};
    MacBuildAppFilePath(&Path);

    char SandboxFilename[MAC_MAX_FILENAME_SIZE];
    char LocalFilename[MAC_MAX_FILENAME_SIZE];
    sprintf(LocalFilename, "Contents/Resources/%s", Filename);
    MacBuildAppPathFileName(&Path, LocalFilename,
                            sizeof(SandboxFilename), SandboxFilename);

    read_file_result Result = {};

    int X,Y,N;
    u32 *ImageData = (u32 *)stbi_load(SandboxFilename, &X, &Y, &N, 0);

    if (X > 0 && Y > 0 && ImageData != NULL)
    {
        Result.Contents = ImageData;
        Result.ContentsSize = X*Y*sizeof(u32); 
    }

    return Result;
}
