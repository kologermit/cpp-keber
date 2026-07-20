#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <exception>
#include <string_view>

namespace Utils::Config {
    using std::string_view;
    using std::shared_ptr;
    using std::map;
    using std::string;
    using std::vector;
    using std::pair;
    using std::invalid_argument;

    struct Argument{

        const string name;
        const string description = "";
        const string default_value = "";
        const bool is_required = false;
        string* string_value = nullptr;
        long long* long_long_value = nullptr;
        vector<long long>* vector_long_long_value = nullptr;

    };

    pair<shared_ptr<invalid_argument>, bool> parse_config(
        const int argc,
        const char* argv[],
        vector<Argument>& arguments
        );

    string get_help_by_vector_arguments(string_view service, const vector<Argument>& arguments);
}