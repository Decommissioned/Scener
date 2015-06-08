#include "program.h"
#include "loader_shader.h"

#include <GL/glew.h>
#include <memory>
#include <iostream>

#define EXPAND(value) #value
#define STRINGFY(value) EXPAND(value)

uint32_t Program::compileShader(const string& folder, const string& file, uint32_t type)
{
        uint32_t shaderID;
        GLint success;
        GLint length[2];
        const char* src[2];

        // Define specific preprocessor macros depending on the type of shader being compiled
        if (type == GL_VERTEX_SHADER)
        {
                src[0] = "#version " STRINGFY(GLSL_VERSION) "\n" "\n#define _VERTEX_SHADER_\n";
        }
        else if (type == GL_FRAGMENT_SHADER)
        {
                src[0] = "#version " STRINGFY(GLSL_VERSION) "\n" "\n#define _FRAGMENT_SHADER_\n";
        }
        else
        {
                src[0] = "#version " STRINGFY(GLSL_VERSION) "\n";
        }

        string source = LoadShader(folder, file);
        src[1] = source.c_str();
        length[0] = (GLint) strlen(src[0]);
        length[1] = (GLint) source.length();

        shaderID = glCreateShader(type);
        glShaderSource(shaderID, 2, src, length);
        glCompileShader(shaderID);
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

        if (success == GL_FALSE)
        {
                GLint length = 0;
                glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<GLchar> log(new GLchar[length]);
                glGetShaderInfoLog(shaderID, length, &length, log.get());
                glDeleteShader(shaderID);
                HandleError("Shader [" + file + "] compilation error:\r\n" + log.get(), ErrorCriticality::HIGH);
        } 
        
        return shaderID;
}
uint32_t Program::createProgram(uint32_t vertex, uint32_t fragment)
{
        uint32_t programID = glCreateProgram();

        glAttachShader(programID, vertex);
        glAttachShader(programID, fragment);

        return programID;
}
void Program::deleteShader(uint32_t program, uint32_t shader)
{
        glDetachShader(program, shader);
        glDeleteShader(shader);
}
void Program::linkProgram(uint32_t program)
{
        GLint success;

        glLinkProgram(program);
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if (success == GL_FALSE)
        {
                GLint length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<GLchar> log(new GLchar[length]);
                glGetProgramInfoLog(program, length, &length, log.get());
                glDeleteProgram(program);
                HandleError(string("Program link error:\r\n") + log.get(), ErrorCriticality::HIGH);
        }

        glValidateProgram(program);
        glGetProgramiv(program, GL_VALIDATE_STATUS, &success);

        if (success == GL_FALSE)
        {
                GLint length = 0;
                glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
                std::unique_ptr<GLchar> log(new GLchar[length]);
                glGetProgramInfoLog(program, length, &length, log.get());
                glDeleteProgram(program);
                HandleError(string("Program validation error:\r\n") + log.get(), ErrorCriticality::HIGH);
        }
}

int32_t Program::uniformLocation(const string& variable)
{
        int32_t location = glGetUniformLocation(m_program, variable.c_str());
        if (location < 0)
        {
                HandleError("Invalid uniform location for variable: \"" + variable + "\" in program \"" + m_vertex_name + "\", \"" + m_fragment_name + '\"', ErrorCriticality::LOW);
        }
        return location;
}

void Program::loadInteger(int32_t location, int32_t value)
{
        glUniform1i(location, value);
}
void Program::loadFloat(int32_t location, float value)
{
        glUniform1f(location, value);
}
void Program::loadVec2(int32_t location, const vec2& value)
{
        glUniform2f(location, value.x, value.y);
}
void Program::loadVec3(int32_t location, const vec3& value)
{
        glUniform3f(location, value.x, value.y, value.z);
}
void Program::loadVec4(int32_t location, const vec4& value)
{
        glUniform4f(location, value.x, value.y, value.z, value.w);
}
void Program::loadMat4(int32_t location, const mat4& value)
{
        glUniformMatrix4fv(location, 1, GL_FALSE, &value[0][0]);
}

Program::Program(const string& folder, const string& vertex_file, const string& fragment_file, std::initializer_list<std::pair<uint32_t, string>> attributes)
{
        uint32_t vertexID = compileShader(folder, vertex_file, GL_VERTEX_SHADER);
        uint32_t fragmentID = compileShader(folder, fragment_file, GL_FRAGMENT_SHADER);

        m_program = createProgram(vertexID, fragmentID);
        m_vertex_name = vertex_file;
        m_fragment_name = fragment_file;

        for (auto& attribute : attributes)
        {
                glBindAttribLocation(m_program, attribute.first, attribute.second.c_str());
        }

        linkProgram(m_program);
        deleteShader(m_program, vertexID);
        deleteShader(m_program, fragmentID);
}

Program::~Program()
{
        if (m_program)
        {
                glDeleteProgram(m_program);
        }
}

uint32_t Program::ID() const
{
        return m_program;
}

void Program::Bind() const
{
        if (m_bound_program != m_program)
        {
                glUseProgram(m_program);
                m_bound_program = m_program;
        }
}

uint32_t Program::m_bound_program = 0;
