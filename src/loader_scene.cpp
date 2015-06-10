#include "loader_scene.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <cassert>

#define ncase break; case /* no fall-through case */
#define read_function(tree, key, field, reader) { auto _sub_ = tree.get_child_optional(key); if (_sub_) { field = reader (*_sub_); } }
#define read_scalar(tree, key, field) { auto _sub_ = tree.get_child_optional(key); if (_sub_) { field = _sub_->get_value<decltype(field)>(); } }

using namespace boost::property_tree;

// To add a new component:
// 1. Add a default value to the default function, if it's a new structure entirely, define the default value for it
// 2. Use the read_scalar macro to load a value to it, if it's a new structure entirely use the read_function macro

static Skybox default_Skybox()
{
        Skybox skybox;
        skybox.top = "none";
        skybox.bottom = "none";
        skybox.left = "none";
        skybox.right = "none";
        skybox.front = "none";
        skybox.back = "none";
        return skybox;
}
static Camera default_Camera()
{
        Camera camera;
        camera.eye = {0.0f, 0.0f, -3.0f};
        camera.lookAt = {0.0f, 0.0f, 0.0f};
        camera.up = {0.0f, 1.0f, 0.0f};
        return camera;
}
static Window default_Window()
{
        Window window;
        window.width = 800;
        window.height = 600;
        window.fov = 45.0f;
        window.title = "Default Scene";
        return window;
}
static Terrain default_Terrain()
{
        Terrain terrain;
        terrain.width = 50;
        terrain.height = 50;
        terrain.tileSize = 10.0f;
        terrain.minimumHeight = 0.0f;
        terrain.maximumHeight = 0.0f;
        terrain.spacing = 1.0f;
        terrain.seed = "default seed";
        terrain.texture = "none";
        return terrain;
}
static DirectionalLight default_DirectionalLight()
{
        DirectionalLight light;
        light.direction = {1.0f, 1.0f, 0.0f};
        light.color = {1.0f, 1.0f, 1.0f};
        return light;
}
static PointLight default_PointLight()
{
        PointLight light;
        light.position = {0.0f, 0.0f, 0.0f};
        light.color = {1.0f, 1.0f, 1.0f};
        light.attenuation = {0.0f, 0.0f, 1.0f};
        return light;
}
static Object default_Object()
{
        Object object;
        object.mesh = "cube";
        object.texture = "none";
        object.translate = {0.0f, 0.0f, 0.0f};
        object.rotate = {0.0f, 0.0f, 0.0f};
        object.scale = {1.0f, 1.0f, 1.0f};
        object.ka = {0.25f, 0.25f, 0.25f};
        object.kd = {0.4f, 0.4f, 0.4f};
        object.ks = {0.4f, 0.4f, 0.4f};
        object.driver = Driver::NONE;
        object.driverSpeed = {0.0f, 0.0f, 0.0f};
        return object;
}
static Scene default_Scene()
{
        Scene scene;
        scene.perspective = true;
        scene.ambientLight = {0.5f, 0.5f, 0.5f};
        scene.clearColor = {0.0f, 0.0f, 0.0f};
        scene.skybox = default_Skybox();
        scene.camera = default_Camera();
        scene.window = default_Window();
        scene.terrain = default_Terrain();
        scene.directionalLights = {default_DirectionalLight()};
        scene.pointLights = {default_PointLight()};
        scene.objects = {default_Object()};
        return scene;
}

static vec3 read_vec3(const ptree& tree)
{
        assert(tree.size() == 3);
        vec3 data;
        size_t index = 0;
        for (auto& prop : tree)
        {
                data[index] = prop.second.get_value<float>();
                index++;
        }
        return data;
}
static bool read_Projection(const ptree& tree)
{
        return tree.get_value<string>() != "ortho";
}
static Skybox read_Skybox(const ptree& tree)
{
        assert(tree.size() == 6);
        Skybox skybox;
        size_t index = 0;
        for (auto& prop : tree)
        {
                string value = prop.second.get_value<string>();
                switch (index)
                {
                        ncase 0: skybox.top = value;
                        ncase 1: skybox.bottom = value;
                        ncase 2: skybox.left = value;
                        ncase 3: skybox.right = value;
                        ncase 4: skybox.front = value;
                        ncase 5: skybox.back = value;
                }
                index++;
        }
        return skybox;
}
static Window read_Window(const ptree& tree)
{
        Window window = default_Window();
        read_scalar(tree, "Width", window.width);
        read_scalar(tree, "Height", window.height);
        read_scalar(tree, "Fov", window.fov);
        read_scalar(tree, "Title", window.title);
        return window;
}
static Camera read_Camera(const ptree& tree)
{
        Camera camera = default_Camera();
        read_function(tree, "Eye", camera.eye, read_vec3);
        read_function(tree, "LookAt", camera.lookAt, read_vec3);
        read_function(tree, "Up", camera.up, read_vec3);
        return camera;
}
static Terrain read_Terrain(const ptree& tree)
{
        Terrain terrain = default_Terrain();
        read_scalar(tree, "Width", terrain.width);
        read_scalar(tree, "Height", terrain.height);
        read_scalar(tree, "TileSize", terrain.tileSize);
        read_scalar(tree, "MinimumHeight", terrain.minimumHeight);
        read_scalar(tree, "MaximumHeight", terrain.maximumHeight);
        read_scalar(tree, "Spacing", terrain.spacing);
        read_scalar(tree, "Seed", terrain.seed);
        read_scalar(tree, "Texture", terrain.texture);
        return terrain;
}
static std::vector<DirectionalLight> read_DirectionalLights(const ptree& tree)
{
        std::vector<DirectionalLight> directionalLights;
        directionalLights.reserve(tree.size());
        for (auto& sub : tree)
        {
                DirectionalLight light = default_DirectionalLight();
                read_function(sub.second, "Color", light.color, read_vec3);
                read_function(sub.second, "Direction", light.direction, read_vec3);
                directionalLights.emplace_back(std::forward<DirectionalLight>(light));
        }
        return directionalLights;
}
static std::vector<PointLight> read_PointLights(const ptree& tree)
{
        std::vector<PointLight> pointLights;
        pointLights.reserve(tree.size());
        for (auto& sub : tree)
        {
                PointLight light = default_PointLight();
                read_function(sub.second, "Color", light.color, read_vec3);
                read_function(sub.second, "Position", light.position, read_vec3);
                read_function(sub.second, "Attenuation", light.attenuation, read_vec3);
                pointLights.emplace_back(std::forward<PointLight>(light));
        }
        return pointLights;
}
static Driver read_Driver(const ptree& tree)
{
        Driver driver = Driver::NONE;
        string content = tree.get_value<string>();
        if (content == "rotate")
        {
                driver = Driver::ROTATE;
        }
        return driver;
}
static std::vector<Object> read_Objects(const ptree& tree)
{
        std::vector<Object> objects;
        objects.reserve(tree.size());
        for (auto& sub : tree)
        {
                Object object = default_Object();
                read_scalar(sub.second, "Mesh", object.mesh);
                read_scalar(sub.second, "Texture", object.texture);
                read_function(sub.second, "Translate", object.translate, read_vec3);
                read_function(sub.second, "Rotate", object.rotate, read_vec3);
                read_function(sub.second, "Scale", object.scale, read_vec3);
                read_function(sub.second, "Ka", object.ka, read_vec3);
                read_function(sub.second, "Kd", object.kd, read_vec3);
                read_function(sub.second, "Ks", object.ks, read_vec3);
                read_function(sub.second, "Driver", object.driver, read_Driver);
                read_function(sub.second, "DriverSpeed", object.driverSpeed, read_vec3);
                objects.emplace_back(std::forward<Object>(object));
        }
        return objects;
}

Scene LoadScene(const string& path)
{
        try
        {
                Scene scene = default_Scene();
                ptree tree;
                read_json(path, tree);
                
                read_function(tree, "Projection", scene.perspective, read_Projection);
                read_function(tree, "AmbientLight", scene.ambientLight, read_vec3);
                read_function(tree, "ClearColor", scene.clearColor, read_vec3);
                read_function(tree, "Skybox", scene.skybox, read_Skybox);
                read_function(tree, "Window", scene.window, read_Window);
                read_function(tree, "Camera", scene.camera, read_Camera);
                read_function(tree, "Terrain", scene.terrain, read_Terrain);
                read_function(tree, "DirectionalLights", scene.directionalLights, read_DirectionalLights);
                read_function(tree, "PointLights", scene.pointLights, read_PointLights);
                read_function(tree, "Objects", scene.objects, read_Objects);

                return scene;
        }
        catch (const json_parser_error& error)
        {
                std::cerr << path << '[' << error.line() << "]: " << error.message() << std::endl;
                throw;
        }
}