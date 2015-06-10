#include "algorithm_perlin.h"

#include <functional>
#include <random>

#include <glm/gtc/noise.hpp>

std::vector<float> Grid(size_t width, size_t height, float xs, float xe, float ys, float ye, float min, float max, const std::string& seed)
{
        static std::hash<std::string> hash;
        static std::uniform_real_distribution<float> distribute(min, max);
        std::mt19937 rd(hash(seed));

        std::vector<float> data;

        // Step values for {x} and {y}
        float sx = width / (xe - xs);
        float sy = height / (ye - ys);

        float y = ys;
        for (size_t j = 0; j < height; j++, y+=sy)
        {
                float x = xs;
                for (size_t i = 0; i < width; i++, x+=sx)
                {
                        data.emplace_back(x);
                        data.emplace_back(y);
                        data.emplace_back(distribute(rd));
                }
        }

        return data;
}

Image Noise(size_t width, size_t height, size_t count, const std::string& seed)
{
        Image image;
        image.width = width;
        image.height = height;
        image.data.reserve(width * height * 4); // RGBA

        auto grid = Grid(count, count, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, seed);

        // Scale conversion coefficients
        float sx = float(count) / float(width);
        float sy = float(count) / float(height);

        for (size_t y = 0; y < height; y++)
        {
                for (size_t x = 0; x < width; x++)
                {
                        
                }
        }

        float n = glm::perlin(glm::vec2(1.0f, 1.0f));
}
