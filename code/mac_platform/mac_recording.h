
internal mac_replay_buffer *
MacGetReplayBuffer(mac_state *MacState, int unsigned Index)
{
    Assert(Index < ArrayCount(MacState->ReplayBuffers));
    mac_replay_buffer *ReplayBuffer = &MacState->ReplayBuffers[Index];
    return ReplayBuffer;
}

internal void
MacBeginRecordingInput(thread_context *Thread, mac_state *MacState, int InputRecordingIndex)
{
    mac_replay_buffer *ReplayBuffer = MacGetReplayBuffer(MacState, InputRecordingIndex);
    if (ReplayBuffer->MemoryBlock)
    {
        MacState->InputRecordingIndex = InputRecordingIndex;
        MacState->RecordingHandle = ReplayBuffer->FileHandle;
        fseek(MacState->RecordingHandle, MacState->PermanentStorageSize, SEEK_SET);
        memcpy(ReplayBuffer->MemoryBlock, MacState->GameMemoryBlock, MacState->PermanentStorageSize);
    }
}

internal void
MacEndRecordingInput(mac_state *MacState)
{
    MacState->InputRecordingIndex = 0;
}

internal void
MacBeginInputPlayback(thread_context *Thread, mac_state *MacState, int InputPlayingIndex)
{
    mac_replay_buffer *ReplayBuffer = MacGetReplayBuffer(MacState, InputPlayingIndex);
    if (ReplayBuffer->MemoryBlock)
    {
        MacState->InputPlayingIndex = InputPlayingIndex;
        MacState->PlaybackHandle = ReplayBuffer->FileHandle;
        fseek(MacState->PlaybackHandle, MacState->PermanentStorageSize, SEEK_SET);
        memcpy(MacState->GameMemoryBlock, ReplayBuffer->MemoryBlock, MacState->PermanentStorageSize);
    }
}

internal void
MacEndInputPlayback(mac_state *MacState)
{
    MacState->InputPlayingIndex = 0;
}

internal void
MacRecordInput(mac_state *MacState, game_input *NewInput)
{
    size_t BytesWritten = fwrite(NewInput, sizeof(char), sizeof(*NewInput), MacState->RecordingHandle);
    if (BytesWritten <= 0)
    {
        NSLog(@"Mac Record input for live looped code editing did not write to file.");
    }
}

internal void
MacPlaybackInput(thread_context *Thread, mac_state *MacState, game_input *NewInput)
{
    uint64 BytesRead = fread(NewInput, sizeof(char), sizeof(*NewInput), MacState->PlaybackHandle);
    if (BytesRead <= 0) 
    {
        int PlayingIndex = MacState->InputPlayingIndex;
        MacEndInputPlayback(MacState); 
        MacBeginInputPlayback(Thread, MacState, PlayingIndex);
    }
}
