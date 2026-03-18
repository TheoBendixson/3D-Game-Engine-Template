
void
ResetInput(game_input *Input)
{
    for (s32 Index = 0;
         Index < ArrayCount(Input->Keyboard.Buttons);
         Index += 1)
    {
        Input->Keyboard.Buttons[Index].TransitionCount = 0;
    }

    for (s32 Index = 0;
         Index < ArrayCount(Input->Controller.Buttons);
         Index += 1)
    {
        Input->Controller.Buttons[Index].TransitionCount = 0;
    }

    for (s32 Index = 0;
         Index < ArrayCount(Input->MouseButtons);
         Index += 1)
    {
        Input->MouseButtons[Index].TransitionCount = 0;
    }

    Input->MouseWheel = 0;
}
