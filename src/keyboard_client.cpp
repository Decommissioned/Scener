#include "keyboard_client.h"

KeyboardClient::KeyboardClient(const KeyboardServer& server) : m_server(server)
{}

KeyboardClient::~KeyboardClient()
{}

InputState KeyboardClient::operator[](unsigned char key) const
{
        return m_server.m_keys[key];
}

bool KeyboardClient::Pressed(unsigned char key) const
{
        auto state =m_server.m_keys[key];
        return state == InputState::DOWN || state == InputState::PRESSED;
}
