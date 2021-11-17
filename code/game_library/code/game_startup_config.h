struct game_startup_config
{
    u32 SoundSamplesPerSecond;
    u32 SoundBytesPerSample;
    u32 SoundBufferSize;
};

#define GAME_GET_STARTUP_CONFIG(name) game_startup_config name()
typedef GAME_GET_STARTUP_CONFIG(game_get_startup_config);
