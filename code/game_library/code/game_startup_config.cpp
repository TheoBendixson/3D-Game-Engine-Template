extern "C"
GAME_GET_STARTUP_CONFIG(GameGetStartupConfig)
{
    game_startup_config Config = {};
    Config.SoundSamplesPerSecond = 44100;
    Config.SoundBytesPerSample = sizeof(s16)*2;

    // NOTE: (Ted)  Two seconds of audio.
    Config.SoundBufferSize = Config.SoundSamplesPerSecond*Config.SoundBytesPerSample*2;
    return Config;
}
