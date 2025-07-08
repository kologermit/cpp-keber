#pragma once

#include <iostream>
#include <optional>
#include <memory>
#include <ctime>
#include <map>

namespace Utils {
namespace Type {

// c
using const_c_string = const char*;
using const_int = const int;
using const_bool = const bool;

// std
using const_string = const std::string;
using const_map_string_string = const std::map<const_string, const_string>;
using optional_const_string = std::optional<const_string&>;

}
}