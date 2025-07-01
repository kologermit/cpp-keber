#pragma once

#include <iostream>
#include <crow.h>
#include <memory>
#include <ctime>

namespace Utils {
namespace Types {

// c
using CString = char*;
using ConstCString = const CString;
using Int = int;
using ConstInt = const Int;
using Void = void;

// std
using String = std::string;
using ConstString = const String;
template <typename T> using SharedPtr = std::shared_ptr<T>;
using TimeT = std::time_t;

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