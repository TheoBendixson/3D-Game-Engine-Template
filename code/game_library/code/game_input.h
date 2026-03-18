
struct button_state
{
    int TransitionCount;
    b32 EndedDown;
};

b32
ButtonWentDown(button_state *Button)
{
    b32 Result = (Button->EndedDown && (Button->TransitionCount % 2));
    return Result;
}

b32
ButtonWentUp(button_state *Button)
{
    b32 Result = (!Button->EndedDown && (Button->TransitionCount % 2));
    return Result;
}

struct game_controller_input
{
    union
    {
        button_state Buttons[14];
        struct
        {
            button_state Up;
            button_state Down;
            button_state Left;
            button_state Right;
            button_state A;
            button_state B;
            button_state X;
            button_state Y;
            button_state LeftShoulder1;
            button_state LeftShoulder2;
            button_state RightShoulder1;
            button_state RightShoulder2;
            button_state Start;
            button_state Select;
        };
    };

    v2 RightThumb;
};

struct keyboard_input
{
    union
    {
        button_state Buttons[26 + 10 + 20 + 13];
        struct
        {
            button_state Letters[26];
            button_state Numbers[10];
            button_state F[20]; // [0] unused for convenience; F1=F[1] .. F15=F[15]

            button_state Up;
            button_state Down;
            button_state Left;
            button_state Right;

            button_state Space;
            button_state Enter;
            button_state Delete;
            button_state Backspace;
            button_state Tab;
            button_state Shift;
            button_state Alt;
            button_state Control;
            button_state Escape;
        };
    };
};

enum game_input_mode
{
    GameInputMode_KeyboardMouse,
    GameInputMode_GameController
};

struct game_input
{
    game_input_mode PrimaryInputMode;
    keyboard_input Keyboard;
    game_controller_input Controller;
    r32 dtForFrame;
    r32 FrameRateMultiplier;

    button_state MouseButtons[5]; // 0=left 1=middle 2=right 3=prev 4=next
    v2 MousePos;
    v2 WindowDim;
    s32 MouseWheel;

    b32 HasFocus;
    b32 HasFocusPrev;
};

inline void
ProcessButton(button_state *Button, b32 IsDown)
{
    if (!Button->EndedDown != !IsDown)
    {
        Button->EndedDown = IsDown;
        Button->TransitionCount++;
    }
}
