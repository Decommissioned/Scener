#include "loader_utility.h"

#include <fstream>
#include <sys/stat.h>
#include <boost/filesystem.hpp>

size_t FileSize(const string& file)
{
        struct stat st;
        if (stat(file.c_str(), &st) != 0)
        {
                HandleError("File " + file + " not found or user does not have read permission", ErrorCriticality::HIGH);
        }
        return st.st_size;
}

string ReadText(const string& file)
{
        size_t length = FileSize(file);
        std::ifstream hfile(file);
        string content(length, 0);
        hfile.read(&content[0], length);
        return content.c_str(); // Converting it to a C string before converting it back into a string adds a null terminator, a bit of a hack, but very convenient
}

std::vector<uint8_t> ReadBinary(const string& file)
{
        size_t length = FileSize(file);
        std::ifstream hfile(file, std::ios::binary);
        std::vector<uint8_t> data(length);
        data.assign(std::istreambuf_iterator<char>(hfile), std::istreambuf_iterator<char>());
        return data;
}

std::vector<string> ListFiles(const string& directory, const string& extension)
{
        std::vector<string> files;
        boost::filesystem::path path = directory;
        boost::filesystem::directory_iterator end;
        for (boost::filesystem::directory_iterator itr(path); itr != end; ++itr)
        {
                if (boost::filesystem::is_regular_file(itr->path()))
                {
                        string file = itr->path().string();
                        if (!extension.empty())
                        {
                                if (file.substr(file.find_last_of('.')) == extension)
                                {
                                        files.emplace_back(file);
                                }
                        }
                        else
                        {
                                files.emplace_back(file);
                        }
                }
        }
        return files;
}
