#ifndef PROGRAM_UNIFORMBUFFER_HEADER
#define PROGRAM_UNIFORMBUFFER_HEADER

#include "configuration.h"

class UniformBuffer final
{
        void update(const void* data, uint32_t length);

        uint32_t m_buffer;
        uint32_t m_binding_point;

        static uint32_t m_next_binding_point;
        static uint32_t m_binding_points;

public:

        UniformBuffer();

        ~UniformBuffer();

        void AddPrograms(std::initializer_list<uint32_t> programs, const string& name);

        template<typename block>
        void Update(const block& data)
        {
                update(&data, sizeof(block));
        }
};

#endif // PROGRAM_UNIFORMBUFFER_HEADER
