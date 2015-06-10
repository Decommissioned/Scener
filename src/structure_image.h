#ifndef STRUCTURE_IMAGE_HEADER
#define STRUCTURE_IMAGE_HEADER

#include <cstdint>
#include <string>
#include <vector>

struct Image
{
        int width;
        int height;
        std::vector<uint8_t> data;
        std::string name;
};

#endif // STRUCTURE_IMAGE_HEADER
