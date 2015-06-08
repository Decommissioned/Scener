#include "loader_shader.h"
#include "loader_utility.h"

#include <regex>
#include <stack>
#include <tuple>

#define MAX_INCLUDE_RECURSION_DEPTH 100

static string LoadShader(const string& folder, const string& file, size_t depth)
{
        if (depth > MAX_INCLUDE_RECURSION_DEPTH)
        {
                HandleError("Maximum include depth reached: " + file, ErrorCriticality::HIGH);
        }
        // Load the actual file's content
        string separator = folder.back() != '/' ? "/" : "";
        string content = ReadText(folder + separator + file);
        // Search for include directives and replace them with their file's content
        static const std::regex rgx_include(R"(^[ \t]*#pragma[ \t]+include[ \t]*\([ \t]*"(\S+))"R"("[ \t]*\)[ \t]*$)");
        std::match_results<std::string::const_iterator> match;
        std::stack<std::tuple<size_t, size_t, string>> replaces;
        for (std::string::const_iterator start = content.begin();
             std::regex_search(start, content.cend(), match, rgx_include);
             start = match[0].second)
        { // Recursively load subsequently included files
                string directive = match[0];
                replaces.emplace(match.position(), directive.length(), LoadShader(folder, match[1], depth + 1));
        }
        while (!replaces.empty())
        {
                auto tuple = replaces.top();
                size_t offset = std::get<0>(tuple);
                size_t length = std::get<1>(tuple);
                string included_content = std::get<2>(tuple);
                content.replace(offset, length, included_content);
                replaces.pop();
        }
        return content;
}

string LoadShader(const string& folder, const string& file)
{
        return LoadShader(folder, file, 0);
}
