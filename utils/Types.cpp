#pragma once

#include <iostream>
#include <crow.h>
#include <memory>

namespace Utils {
namespace Types {

// std
using String = std::string;
using ConstString = const String;
using ConstCString = const char*;
using ConstInt = const int;
template <typename T> using SharedPtr = std::shared_ptr<T>;

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