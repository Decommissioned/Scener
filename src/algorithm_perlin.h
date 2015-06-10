#ifndef ALGORITHM_PERLIN_HEADER
#define ALGORITHM_PERLIN_HEADER

#include "structure_image.h"
#include <string>

std::vector<float> Grid(size_t width, size_t height, float xs, float xe, float ys, float ye, float min, float max, const std::string& seed);
Image Noise(size_t width, size_t height, size_t count, const std::string& seed);

#endif // ALGORITHM_PERLIN_HEADER
