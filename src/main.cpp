#include "configuration.h"
#include "input.h"
#include "loader.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>
#include <cassert>
#include <thread>

#define ncase break; case /* no fall-through case */

void HandleError(const string& msg, ErrorCriticality criticality)
{
        std::cerr << msg << std::endl;
        if (criticality != ErrorCriticality::LOW)
        {
                throw std::runtime_error(msg);
        }
}

//////////////////////////////////////////////////////////////////////////

static bool quit = false;
static MouseServer mouse;
static KeyboardServer keyboard;

void handleControls(Camera& camera)
{
        static MouseClient mc = mouse.CreateClient();
        static KeyboardClient kc = keyboard.CreateClient();
        static float rotation = 0.0f;
        static float pitch = 0.0f;
        static float velocity = 0.0f;
        static bool air = false;

        const float speed = 0.1f;
        const float sensitivity = 0.005f;
        const float dt = 1.0f / 60.0f;
        const float gravity = 20.81f;
        const float jump = 5.0f;

        float cx = glm::sin(rotation);
        float cz = glm::cos(rotation);
        float cy = glm::tan(pitch);

        // Movement handling
        if (kc.Pressed('w'))
        {
                camera.eye.z += speed * cz;
                camera.eye.x += speed * cx;
        }
        if (kc.Pressed('s'))
        {
                camera.eye.z -= speed * cz;
                camera.eye.x -= speed * cx;
        }
        if (kc.Pressed('a'))
        {
                camera.eye.x += speed * cz;
                camera.eye.z -= speed * cx;
        }
        if (kc.Pressed('d'))
        {
                camera.eye.x -= speed * cz;
                camera.eye.z += speed * cx;
        }

        // Quit on escape
        if (kc.Pressed(SDLK_ESCAPE))
        {
                quit = true;
        }

        if (kc[SDLK_SPACE] == InputState::DOWN && !air)
        {
                velocity = jump;
        }

        if (camera.eye.y > 0.0f)
        {
                velocity -= gravity * dt;
        }

        camera.eye.y += velocity * dt;
        air = camera.eye.y > 0.0f;

        if (camera.eye.y < 0.0f)
        {
                camera.eye.y = 0.0f;
        }

        rotation -= mc.Dx() * sensitivity;
        pitch -= mc.Dy() * sensitivity;
        if (pitch > glm::half_pi<float>() - 0.05f) pitch = glm::half_pi<float>() - 0.05f;
        if (pitch < -glm::half_pi<float>() + 0.05f) pitch = -glm::half_pi<float>() + 0.05f;

        camera.lookAt = camera.eye + vec3(cx, cy, cz);
}

void worker_render(SDL_Window* window, Scene& scene)
{
        auto context = SDL_GL_CreateContext(window);
        assert(context != nullptr);
        glewExperimental = OPENGL_EXTENSIONS;
        assert(glewInit() == GLEW_OK);

        Renderer renderer("../meshes", "../textures", "../programs");

        SDL_ShowWindow(window);
        while (!quit)
        {
                auto now = std::chrono::system_clock::now();

                handleControls(scene.camera);
                mouse.Update();
                keyboard.Update();

                renderer.Draw(scene);
                SDL_GL_SwapWindow(window);

                auto delta = std::chrono::system_clock::now() - now;
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(delta);
                std::this_thread::sleep_for(std::chrono::microseconds(16667) - duration);
        }
        SDL_GL_DeleteContext(context);
}

void initialize_SDL()
{
        assert(SDL_InitSubSystem(SDL_INIT_VIDEO) == 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VERSION);
        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        // Enable vsync
        SDL_GL_SetSwapInterval(1);
        // Trap mouse cursor
        SDL_ShowCursor(0);
        SDL_SetRelativeMouseMode(SDL_TRUE);
}

int main(int argc, char**argv)
{
        if (argc < 2)
        {
                std::cout << "No scene file provided to the renderer" << std::endl;
                return 0;
        }

        initialize_SDL();
        Scene scene = LoadScene(argv[1]);

        SDL_Event event;
        SDL_Window* window = SDL_CreateWindow(scene.window.title.c_str(), 50, 50, scene.window.width, scene.window.height, SDL_WINDOW_OPENGL);
        assert(window != nullptr);

        std::thread worker(worker_render, window, scene);
        while (SDL_WaitEvent(&event) && !quit)
        {
                switch (event.type)
                {
                        ncase SDL_QUIT : quit = true;
                        ncase SDL_MOUSEMOTION : mouse.Move(event.motion.xrel, event.motion.yrel);
                        ncase SDL_MOUSEBUTTONDOWN : mouse.PressButton(event.button.button);
                        ncase SDL_MOUSEBUTTONUP : mouse.ReleaseButton(event.button.button);
                        ncase SDL_MOUSEWHEEL : mouse.Wheel(event.wheel.y);
                        ncase SDL_KEYDOWN : if (!event.key.repeat) keyboard.PressButton(event.key.keysym.sym);
                        ncase SDL_KEYUP : keyboard.ReleaseButton(event.key.keysym.sym);
                }
        }
        worker.join();

        SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return 0;
}