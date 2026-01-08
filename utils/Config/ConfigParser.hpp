#pragma once

#include <map>
#include <string>
#include <vector>

namespace Utils::Config {
    using std::map;
    using std::string;
    using std::vector;

    struct Argument{

        const string name;
        const string default_value = "";
        const bool is_required = false;
        string* string_value = nullptr;
        long long* long_long_value = nullptr;
        vector<long long>* vector_long_long_value = nullptr;

    };

    bool parse_config(
        const int argc,
        const char* argv[],
        vector<Argument>& arguments
        );
}