#include "configuration.h"
#include "mouse_server.h"
#include "loader.h"
#include "renderer.h"

#include <SDL2/SDL.h>
#include <GL/glew.h>

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

void worker_render(SDL_Window* window, const Scene& scene)
{
        auto context = SDL_GL_CreateContext(window);
        assert(context != nullptr);
        assert(glewInit() == GLEW_OK);

        Renderer renderer("../meshes", "../textures", "../programs");

        SDL_ShowWindow(window);
        while (!quit)
        {
                renderer.Draw(scene);
                SDL_GL_SwapWindow(window);
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
                        ncase SDL_QUIT: quit = true;
                        ncase SDL_MOUSEMOTION: mouse.Move(event.motion.xrel, event.motion.yrel);
                        ncase SDL_MOUSEBUTTONDOWN : mouse.PressButton(event.button.button);
                        ncase SDL_MOUSEBUTTONUP : mouse.ReleaseButton(event.button.button);
                }
        }
        worker.join();

        SDL_DestroyWindow(window);
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
        return 0;
}