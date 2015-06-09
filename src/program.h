#ifndef PROGRAM_HEADER
#define PROGRAM_HEADER

#include "configuration.h"
#include "structure_directionallight.h"
#include "structure_pointlight.h"

class Program
{
        static uint32_t compileShader(const string& folder, const string& file, uint32_t type, std::initializer_list<string> pre_processor = {});
        static uint32_t createProgram(uint32_t vertexID, uint32_t fragmentID);
        static void deleteShader(uint32_t programID, uint32_t shaderID);
        static void linkProgram(uint32_t programID);

protected:

        static uint32_t m_bound_program;

        int32_t uniformLocation(const string& variable);

        static void loadInteger(int32_t location, int32_t value);
        static void loadFloat(int32_t location, float value);

        static void loadVec2(int32_t location, const vec2& value);
        static void loadVec3(int32_t location, const vec3& value);
        static void loadVec4(int32_t location, const vec4& value);
        static void loadMat4(int32_t location, const mat4& value);

        uint32_t m_program;
        string m_vertex_name;
        string m_fragment_name;

        Program() = delete;
        Program(const string& folder, const string& vertex_file, const string& fragment_file, std::initializer_list<std::pair<uint32_t, string>> attributes = {}, std::initializer_list<string> pre_processor = {});
        Program(const Program&) = delete;
        const Program& operator=(const Program&) = delete;
        virtual ~Program();

public:

        uint32_t ID() const;
        void Bind() const;

};

#endif // PROGRAM_HEADER
