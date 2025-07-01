#pragma once

#include <cstdlib>
#include <random>
#include <ctime>
#include <fmt/core.h>
#include <utils/Types.cpp>

namespace Utils {
namespace Functions {

using Utils::Types::ConstString;
using Utils::Types::CString;
using Utils::Types::String;
using Utils::Types::Void;
using Utils::Types::ConstInt;
using Utils::Types::Int;
using Utils::Types::TimeT;

// c
CString GetEnv(ConstString& name) { return getenv(name.c_str()); }

// std
Void SRand(ConstInt seed) { std::srand(seed); }
Int Rand() { return std::rand(); }
TimeT Time(TimeT* arg) {return std::time(arg); }

// fmt
template<typename ...Args>
String Format(ConstString& pattern, Args&... args) { return fmt::format(pattern, args...); }

}
}