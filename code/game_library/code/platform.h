
struct read_file_result
{
    void *Contents;
    u64 ContentsSize;
    char *Filename;
};

#define PLATFORM_READ_PNG_FILE(name) read_file_result name(char *Filename)
typedef PLATFORM_READ_PNG_FILE(platform_read_png_file);

#define PLATFORM_READ_ENTIRE_FILE(name) read_file_result name(char *Filename)
typedef PLATFORM_READ_ENTIRE_FILE(platform_read_entire_file);

#define PLATFORM_FREE_FILE_MEMORY(name) void name(void *Memory)
typedef PLATFORM_FREE_FILE_MEMORY(platform_free_file_memory);

#define PLATFORM_WRITE_ENTIRE_FILE(name) b32 name(char *Filename, u64 FileSize, void *Memory)
typedef PLATFORM_WRITE_ENTIRE_FILE(platform_write_entire_file);

#define PLATFORM_OPEN_FILE_DIALOG(name) void name(void *TransientStorage)
typedef PLATFORM_OPEN_FILE_DIALOG(platform_open_file_dialog);

#define PLATFORM_SAVE_FILE_DIALOG(name) void name(u64 FileSize, void *Memory)
typedef PLATFORM_SAVE_FILE_DIALOG(platform_save_file_dialog);

#define PLATFORM_LOG_MESSAGE(name) void name(char *Message)
typedef PLATFORM_LOG_MESSAGE(platform_log_message);

#define PLATFORM_QUIT_GAME(name) void name()
typedef PLATFORM_QUIT_GAME(platform_quit_game);

struct async_file_io
{
    volatile b32 IsActive;
    volatile b32 IsComplete;
    volatile b32 HasError;
    volatile r32 Progress;
    b32          IsSave;
    char         FilePath[260];
    void        *DataBuffer;
    u64          DataSize;
};

#define PLATFORM_LAUNCH_ASYNC_SAVE(name) void name(async_file_io *IO)
typedef PLATFORM_LAUNCH_ASYNC_SAVE(platform_launch_async_save);

#define PLATFORM_LAUNCH_ASYNC_LOAD(name) void name(async_file_io *IO)
typedef PLATFORM_LAUNCH_ASYNC_LOAD(platform_launch_async_load);

internal void
PrepareAsyncFileIO(async_file_io *IO, b32 IsSave)
{
    IO->IsActive   = 1;
    IO->IsComplete = 0;
    IO->HasError   = 0;
    IO->Progress   = 0.0f;
    IO->IsSave     = IsSave;
}
