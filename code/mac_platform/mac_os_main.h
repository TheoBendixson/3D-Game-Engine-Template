#include "../game_library/code/vario.h"

const unsigned short LeftArrowKeyCode = 0x7B;
const unsigned short RightArrowKeyCode = 0x7C;
const unsigned short DownArrowKeyCode = 0x7D;
const unsigned short UpArrowKeyCode = 0x7E;
const unsigned short AKeyCode = 0x00;
const unsigned short SKeyCode = 0x01;
const unsigned short DKeyCode = 0x02;
const unsigned short FKeyCode = 0x03;
const unsigned short RKeyCode = 0x0F;
const unsigned short LKeyCode = 0x25;
const unsigned short WKeyCode = 0x0D;
const unsigned short ReturnKeyCode = 0x24;
const unsigned short F1KeyCode = 0x7A;
const unsigned short SpacebarKeyCode = 0x31;
const unsigned short DeleteKeyCode = 0x33;
const unsigned short ZKeyCode = 0x06;
const unsigned short YKeyCode = 0x10;

struct mac_game_controller
{
    s32 LeftThumbXUsageID;
    s32 LeftThumbYUsageID;
    s32 ButtonAUsageID;
    s32 ButtonBUsageID;
    s32 ButtonXUsageID;
    s32 ButtonYUsageID;

    s32 ButtonLeftShoulder1UsageID;
    s32 ButtonRightShoulder1UsageID;

    s32 ButtonLeftShoulder2UsageID;
    s32 ButtonRightShoulder2UsageID;
    
    s32 ButtonStartUsageID;
    s32 ButtonSelectUsageID;

    // Values
    r32 LeftThumbstickX;
    r32 LeftThumbstickY;
    b32 UsesHatSwitch;

    s32 DPadX;
    s32 DPadY;

    b32 ButtonAState;
    b32 ButtonBState;
    b32 ButtonXState;
    b32 ButtonYState;

    b32 ButtonLeftShoulder1State;
    b32 ButtonRightShoulder1State;

    b32 ButtonLeftShoulder2State;
    b32 ButtonRightShoulder2State;

    b32 ButtonStartState;
    b32 ButtonSelectState;
};

