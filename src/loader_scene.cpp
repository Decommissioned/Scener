#include "loader_scene.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <iostream>
#include <cassert>

#define ncase break; case /* no fall-through case */
#define reader_optional(tree, key, field, reader) { auto _sub_ = tree.get_child_optional(key); if (_sub_) { field = reader (*_sub_); } }
#define scalar_optional(tree, key, field) { auto _sub_ = tree.get_child_optional(key); if (_sub_) { field = _sub_->get_value<decltype(field)>(); } }

using namespace boost::property_tree;

static Skybox default_Skybox()
{
        return {"none", "none", "none", "none", "none", "none"};
}
static Camera default_Camera()
{
        return {
                {0.0f, 0.0f, -1.0f},
                {0.0f, 0.0f, 0.0f},
                {0.0f, 1.0f, 0.0f}
        };
}
static Window default_Window()
{
        return {
                800,
                600,
                45.0f,
                "Default Scene"
        };
}
static Terrain default_Terrain()
{
        return {
                50,
                50,
                10.0f,
                0.0f,
                0.0f,
                1.0f,
                "default",
                "none"
        };
}
static DirectionalLight default_DirectionalLight()
{
        return {
                {1.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 1.0f}
        };
}
static PointLight default_PointLight()
{
        return {
                {1.0f, 1.0f, 1.0f},
                {0.0f, 1.0f, 0.0f}
        };
}
static Object default_Object()
{
        return {

                "cube",
                "none",
                {0.0f, 0.0f, 0.0f},
                {0.0f, 0.0f, 0.0f},
                {1.0f, 1.0f, 1.0f},
                {0.25f, 0.25f, 0.25f},
                {0.6f, 0.6f, 0.6f},
                {0.7f, 0.7f, 0.7f},
                Driver::NONE,
                {0.0f, 0.0f, 0.0f}

        };
}
static Scene default_Scene()
{
        Scene scene;
        scene.attenuationFactors = {1.0f, 0.0f, 0.0f};
        scene.ambientLight = {0.5f, 0.5f, 0.5f};
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
        scalar_optional(tree, "Width", window.width);
        scalar_optional(tree, "Height", window.height);
        scalar_optional(tree, "Fov", window.fov);
        scalar_optional(tree, "Title", window.title);
        return window;
}
static Camera read_Camera(const ptree& tree)
{
        Camera camera = default_Camera();
        reader_optional(tree, "Eye", camera.eye, read_vec3);
        reader_optional(tree, "LookAt", camera.lookAt, read_vec3);
        reader_optional(tree, "Up", camera.up, read_vec3);
        return camera;
}
static Terrain read_Terrain(const ptree& tree)
{
        Terrain terrain = default_Terrain();
        scalar_optional(tree, "Width", terrain.width);
        scalar_optional(tree, "Height", terrain.height);
        scalar_optional(tree, "TileSize", terrain.tileSize);
        scalar_optional(tree, "MinimumHeight", terrain.minimumHeight);
        scalar_optional(tree, "MaximumHeight", terrain.maximumHeight);
        scalar_optional(tree, "Spacing", terrain.spacing);
        scalar_optional(tree, "Seed", terrain.seed);
        scalar_optional(tree, "Texture", terrain.texture);
        return terrain;
}
static std::vector<DirectionalLight> read_DirectionalLights(const ptree& tree)
{
        std::vector<DirectionalLight> directionalLights;
        directionalLights.reserve(tree.size());
        for (auto& sub : tree)
        {
                DirectionalLight light = default_DirectionalLight();
                reader_optional(sub.second, "Color", light.color, read_vec3);
                reader_optional(sub.second, "Direction", light.direction, read_vec3);
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
                reader_optional(sub.second, "Color", light.color, read_vec3);
                reader_optional(sub.second, "Position", light.position, read_vec3);
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
                scalar_optional(sub.second, "Mesh", object.mesh);
                scalar_optional(sub.second, "Texture", object.texture);
                reader_optional(sub.second, "Translate", object.translate, read_vec3);
                reader_optional(sub.second, "Rotate", object.rotate, read_vec3);
                reader_optional(sub.second, "Scale", object.scale, read_vec3);
                reader_optional(sub.second, "Ka", object.ka, read_vec3);
                reader_optional(sub.second, "Kd", object.kd, read_vec3);
                reader_optional(sub.second, "Ks", object.ks, read_vec3);
                reader_optional(sub.second, "Driver", object.driver, read_Driver);
                reader_optional(sub.second, "DriverSpeed", object.driverSpeed, read_vec3);
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
                reader_optional(tree, "AttenuationFactors", scene.attenuationFactors, read_vec3);
                reader_optional(tree, "AmbientLight", scene.ambientLight, read_vec3);
                reader_optional(tree, "Skybox", scene.skybox, read_Skybox);
                reader_optional(tree, "Window", scene.window, read_Window);
                reader_optional(tree, "Camera", scene.camera, read_Camera);
                reader_optional(tree, "Terrain", scene.terrain, read_Terrain);
                reader_optional(tree, "DirectionalLights", scene.directionalLights, read_DirectionalLights);
                reader_optional(tree, "PointLights", scene.pointLights, read_PointLights);
                reader_optional(tree, "Objects", scene.objects, read_Objects);
                return scene;
        }
        catch (json_parser_error error)
        {
                std::cerr << path << '[' << error.line() << "]: " << error.message() << std::endl;
                throw;
        }
}