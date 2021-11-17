struct game_sound_output_buffer 
{
    u32 SamplesPerSecond;
    u32 SamplesToWriteThisFrame;
    u32 SamplesWrittenThisFrame;
    s16 *Samples;
};
