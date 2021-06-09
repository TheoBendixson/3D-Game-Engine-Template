
struct game_permanent_storage_partition
{
    u64 SoundPartitionSize;
    u8* SoundPartition;
};

struct game_transient_storage_partition
{
    u64 FileReadResultPartitionSize;
    u8* FileReadResult;

    u64 SecondaryPartitionSize;
    u8* SecondaryPartition;
};

struct game_memory 
{
    b32 IsInitialized;

    u64 PermanentStorageSize;
    void *PermanentStorage;

    u64 TransientStorageSize;
    void *TransientStorage;
    
    platform_read_entire_file *PlatformReadEntireFile;
    platform_free_file_memory *PlatformFreeFileMemory;
    platform_write_entire_file *PlatformWriteEntireFile;
    platform_log_message *PlatformLogMessage;

    game_permanent_storage_partition PermanentStoragePartition;
    game_transient_storage_partition TransientStoragePartition;
};
