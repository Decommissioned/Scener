#ifndef KEYBOARD_CLIENT_HEADER
#define KEYBOARD_CLIENT_HEADER

#include "keyboard_server.h"

class KeyboardClient final
{

        const KeyboardServer& m_server;

        KeyboardClient(const KeyboardServer& server);

        friend class KeyboardServer;

public:

        ~KeyboardClient();

        InputState operator[](unsigned char key) const;
        bool Pressed(unsigned char key) const;

};

#endif // KEYBOARD_CLIENT_HEADER
