
struct memory_partition
{
    u64 Size;
    u8* Data; 
};

struct game_permanent_storage_partition
{
    memory_partition Sound;
};

struct game_transient_storage_partition
{
    memory_partition FileReadResult;
    memory_partition SecondaryGeneric;
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
    platform_read_png_file *PlatformReadPNGFile;
    platform_write_entire_file *PlatformWriteEntireFile;
    platform_log_message *PlatformLogMessage;

    game_permanent_storage_partition PermanentPartition;
    game_transient_storage_partition TransientPartition;
};

void 
ClearMemoryPartition(memory_partition *Partition)
{
    u8* Byte = (u8 *)Partition->Data;
    for (u32 Index = 0; Index < Partition->Size; 
         Index++)
    {
        *Byte++ = 0;
    }
}
