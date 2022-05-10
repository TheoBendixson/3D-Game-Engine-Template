
typedef size_t memory_index;

struct memory_arena
{
    memory_index Size;
    u8* Base;
    memory_index Used; 
};

internal void
InitializeArena(memory_arena *Arena, memory_index Size, u8* Base)
{
    Arena->Size = Size;
    Arena->Base = Base;
    Arena->Used = 0; 
}

#define PushStruct(Arena, type) (type *)PushSize_(Arena, sizeof(type)) 
#define PushArray(Arena, Count, type) (type *)PushSize_(Arena, (Count)*sizeof(type)) 

void *
PushSize_(memory_arena *Arena, memory_index Size)
{
    Assert((Arena->Used + Size) <= Arena->Size);
    void *Result = Arena->Base + Arena->Used;
    Arena->Used += Size;
    return (Result);
}


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
