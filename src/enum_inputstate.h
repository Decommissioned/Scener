#ifndef ENUMERATION_INPUTSTATE_HEADER
#define ENUMERATION_INPUTSTATE_HEADER

enum class InputState
{
        INACTIVE = 0,   // Initial state
        DOWN,           // Just pressed, transitions to PRESSED
        PRESSED,        // Held down
        RELEASED        // Just released, transitions to INACTIVE
};

#endif // ENUMERATION_INPUTSTATE_HEADER
