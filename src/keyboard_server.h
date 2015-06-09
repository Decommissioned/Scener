#ifndef KEYBOARD_SERVER_HEADER
#define KEYBOARD_SERVER_HEADER

#include "enum_inputstate.h"

#include <array>

class KeyboardClient; /* Forward declaration for friend */

class KeyboardServer final
{

        const static size_t m_keys_count = 256;
        std::array<InputState, m_keys_count> m_keys;

        const static size_t m_buffer_size = 16;
        std::array<std::pair<unsigned char, bool>, m_buffer_size> m_buffer;
        size_t m_read_index, m_write_index;

        friend class KeyboardClient;

public:

        KeyboardServer();
        ~KeyboardServer();

        void Update();
        void Reset();

        void PressButton(unsigned char key);
        void ReleaseButton(unsigned char key);

        KeyboardClient CreateClient() const;

};

#endif // KEYBOARD_SERVER_HEADER
