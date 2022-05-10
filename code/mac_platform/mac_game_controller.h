void
MacProcessGameControllerButton(game_button_state *oldState, game_button_state *newState,
                               b32 isDown) 
{
    newState->EndedDown = isDown;
    newState->HalfTransitionCount += ((newState->EndedDown == oldState->EndedDown)?0:1);
}
