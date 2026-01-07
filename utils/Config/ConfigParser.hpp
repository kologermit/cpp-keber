#pragma once

#include <map>
#include <string>
#include <vector>

namespace Utils::Config {
    using std::map;
    using std::string;
    using std::vector;

    enum ArgumentType {
        LONG_LONG,
        STRING,
        VECTOR_LONG_LONG,
    };

    struct Argument{

        const string name;
        const string default_value = "";
        const bool is_required = false;
        const ArgumentType type = ArgumentType::STRING;
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