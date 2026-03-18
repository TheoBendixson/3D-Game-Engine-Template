
void
ResetDrawCommandBuffersBeforePlatformRender(game_render_commands *RenderCommands)
{
    game_texture_draw_command_buffer *DrawCommandsBuffer = &RenderCommands->DrawCommandsBuffer;

    for (u32 LayerIndex = 0;
         LayerIndex < BOTTOM_LAYER_COUNT;
         LayerIndex += 1)
    {
        render_commands_array *LayerCommands = &DrawCommandsBuffer->BottomLayers[LayerIndex];
        LayerCommands->DrawnCount = 0;
        LayerCommands->BufferDrawn = false;
    }
}
