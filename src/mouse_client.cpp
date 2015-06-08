#include "mouse_client.h"

MouseClient::MouseClient(const MouseServer& server) : m_server(server)
{}

MouseClient::~MouseClient()
{}

InputState MouseClient::LeftButton() const
{
        return m_server.m_buttons[m_server.m_LB];
}

InputState MouseClient::MiddleButton() const
{
        return m_server.m_buttons[m_server.m_MB];
}

InputState MouseClient::RightButton() const
{
        return m_server.m_buttons[m_server.m_RB];
}

int MouseClient::Wheel() const
{
        return m_server.m_wheel;
}

int MouseClient::Dw() const
{
        return m_server.m_dw;
}

int MouseClient::Dx() const
{
        return m_server.m_dx;
}

int MouseClient::Dy() const
{
        return m_server.m_dy;
}

int MouseClient::X() const
{
        return m_server.m_x;
}

int MouseClient::Y() const
{
        return m_server.m_y;
}