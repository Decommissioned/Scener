#ifndef LOADER_UTILITY_HEADER
#define LOADER_UTILITY_HEADER

#include "configuration.h"
#include <vector>

size_t FileSize(const string& file);

string ReadText(const string& file);
std::vector<uint8_t> ReadBinary(const string& file);

std::vector<string> ListFiles(const string& directory, const string& extension = "");

#endif // LOADER_UTILITY_HEADER
