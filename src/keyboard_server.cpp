#include "keyboard_server.h"
#include "keyboard_client.h"

#include <memory>

#define ncase break; case /* no fall-through case */

KeyboardServer::KeyboardServer()
{
        Reset();
}

KeyboardServer::~KeyboardServer()
{}

void KeyboardServer::Update()
{
        for (auto& state : m_keys)
        {
                switch (state)
                {
                        ncase InputState::DOWN: state = InputState::PRESSED;
                        ncase InputState::RELEASED: state = InputState::INACTIVE;
                }
        }
        while (m_read_index != m_write_index)
        {
                auto action = m_buffer[m_read_index];
                m_keys[action.first] = action.second ? InputState::DOWN : InputState::RELEASED;
                m_read_index = (m_read_index + 1) % m_buffer_size;
        }
}

void KeyboardServer::Reset()
{
        std::memset(&m_keys[0], 0x00, m_keys_count * sizeof(InputState));
        std::memset(&m_buffer[0], 0x00, m_buffer_size * sizeof(std::pair<unsigned char, bool>));
        m_read_index = 0;
        m_write_index = 0;
}

void KeyboardServer::PressButton(unsigned char key)
{
        m_buffer[m_write_index] = {key, true};
        m_write_index = (m_write_index + 1) % m_buffer_size;
}

void KeyboardServer::ReleaseButton(unsigned char key)
{
        m_buffer[m_write_index] = {key, false};
        m_write_index = (m_write_index + 1) % m_buffer_size;
}

KeyboardClient KeyboardServer::CreateClient() const
{
        return KeyboardClient(*this);
}
