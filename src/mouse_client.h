#ifndef MOUSE_CLIENT_HEADER
#define MOUSE_CLIENT_HEADER

#include "mouse_server.h"
#include "enum_inputstate.h"

class MouseClient final
{

        const MouseServer& m_server;

        MouseClient(const MouseServer& server);

        friend class MouseServer;

public:

        ~MouseClient();

        InputState LeftButton() const;
        InputState MiddleButton() const;
        InputState RightButton() const;

        int Wheel() const;

        int Dw() const;
        int Dx() const;
        int Dy() const;

        int X() const;
        int Y() const;

};

#endif // MOUSE_CLIENT_HEADER
