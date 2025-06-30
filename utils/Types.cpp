#pragma once

#include <iostream>
#include <crow.h>

namespace Utils {
namespace Types {

// std
using ConstString = const std::string;
using ConstCString = const char*;
using ConstInt = const int;

// crow
using Request = crow::request;
using ConstRequest = const Request;
using Response = crow::response;
using ConstResponse = const Response;
using JSON = crow::json::wvalue;
using crow::App;
using crow::LogLevel;
using crow::HTTPMethod;

}
}