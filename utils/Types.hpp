#pragma once

#include <iostream>
#include <optional>
#include <memory>
#include <ctime>
#include <map>

namespace Utils::Types {

// c
using const_c_string = const char*;

// std
using const_string = const std::string;
using map_string_to_string = std::map<const_string, const_string>;
using const_map_string_to_string = const map_string_to_string;
using optional_const_string = std::optional<const_string>;

}
