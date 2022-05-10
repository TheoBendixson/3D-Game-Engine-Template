
struct game_button_state 
{
    int HalfTransitionCount;
    b32 EndedDown;
};

struct game_controller_input 
{
    union 
    {
        game_button_state Buttons[14];
        struct 
        {
            game_button_state Up;
            game_button_state Down;
            game_button_state Left;
            game_button_state Right;
            game_button_state A;
            game_button_state B;
            game_button_state X;
            game_button_state Y;
            game_button_state LeftShoulder1;
            game_button_state LeftShoulder2;
            game_button_state RightShoulder1;
            game_button_state RightShoulder2;
            game_button_state Start;
            game_button_state Select;
        };
    };
};

struct game_input 
{
    game_button_state MouseButtons[5];
    s32 MouseX, MouseY, MouseZ;
    r32 dtForFrame;
    game_controller_input Controller;
    r32 FrameRateMultiplier;
};
