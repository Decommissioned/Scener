#ifndef MOUSE_SERVER_HEADER
#define MOUSE_SERVER_HEADER

#include "enum_inputstate.h"

#include <array>

class MouseClient; /* Forward declaration for friend */

class MouseServer final
{
        int m_dx, m_dy;
        int m_x, m_y;
        int m_wheel;
        int m_dw;

        const static int m_LB = 0;
        const static int m_MB = 1;
        const static int m_RB = 2;

        const static size_t m_buttons_count = 3;
        std::array<InputState, m_buttons_count + 1> m_buttons;

        const static size_t m_buffer_size = 16;
        std::array<std::pair<unsigned char, bool>, m_buffer_size> m_buffer;
        size_t m_read_index, m_write_index;

        friend class MouseClient;

public:

        MouseServer();
        ~MouseServer();

        void Move(int dx, int dy);
        void SetPosition(int x, int y);
        void Wheel(int dir);

        void Update();
        void Reset();

        void PressButton(unsigned char button);
        void ReleaseButton(unsigned char button);

        MouseClient CreateClient() const;

};

#endif // MOUSE_SERVER_HEADER
