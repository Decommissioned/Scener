#ifndef CONFIGURATION_HEADER
#define CONFIGURATION_HEADER

#include <cstdint>
#include <string>

using std::string;

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;

#define OPENGL_MAJOR_VERSION 2
#define OPENGL_MINOR_VERSION 0
#define GLSL_VERSION 140

// If the hardware does not support something, setting this to 1 may resolve it
#define OPENGL_EXTENSIONS 1

enum class ErrorCriticality
{
        LOW, MEDIUM, HIGH
};

// Must be defined by the user
void HandleError(const string& msg, ErrorCriticality criticality);

#endif // CONFIGURATION_HEADER
