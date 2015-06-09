#include "mouse_server.h"
#include "mouse_client.h"

#include <memory>

#define ncase break; case /* no fall-through case */

MouseServer::MouseServer()
{
        Reset();
}

MouseServer::~MouseServer()
{}

void MouseServer::Move(int dx, int dy)
{
        m_dx += dx;
        m_dy += dy;
        m_x += dx;
        m_y += dy;
}

void MouseServer::SetPosition(int x, int y)
{
        m_dx = 0;
        m_dy = 0;
        m_x = x;
        m_y = y;
}

void MouseServer::Wheel(int dir)
{
        m_wheel += dir;
        m_dw += dir;
}

void MouseServer::Update()
{
        m_dx = m_dy = m_dw = 0;
        for (auto& state : m_buttons)
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
                m_buttons[action.first] = action.second ? InputState::DOWN : InputState::RELEASED;
                m_read_index = (m_read_index + 1) % m_buffer_size;
        }
}

void MouseServer::Reset()
{
        std::memset(&m_buttons, 0x00, m_buttons_count * sizeof(InputState));
        std::memset(&m_buffer, 0x00, m_buffer_size * sizeof(std::pair<int, bool>));
        m_read_index = m_write_index = 0;
        m_dx = m_dy = m_dw = 0;
        m_x = m_y = 0;
}

void MouseServer::PressButton(unsigned char button)
{
        m_buffer[m_write_index] = {button, true};
        m_write_index = (m_write_index + 1) % m_buffer_size;
}

void MouseServer::ReleaseButton(unsigned char button)
{
        m_buffer[m_write_index] = {button, false};
        m_write_index = (m_write_index + 1) % m_buffer_size;
}

MouseClient MouseServer::CreateClient() const
{
        return MouseClient(*this);
}
