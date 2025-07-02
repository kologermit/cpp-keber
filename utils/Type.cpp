#pragma once

#include <iostream>
#include <crow.h>
#include <memory>
#include <ctime>

namespace Utils {
namespace Type {

// c
using const_c_string = const char*;
using const_int = const int;

// std
using const_string = const std::string;

// crow
using const_request = const crow::request;
using const_response = const crow::response;
using JSON = crow::json::wvalue;

}
}