#pragma once

#include <utils/Type.cpp>
#include <cstdlib>
#include <string>
#include <string_view>

namespace Utils::Env {

using Utils::Type::const_string;
using Utils::Type::const_c_string;
using std::getenv;
using std::string;
using std::string_view;

string 
Get(string_view variable, string_view _default="") {

    const_c_string res = getenv(variable.data());
    if (res == nullptr) {
        return const_string(_default);
    }
    return const_string(res);
}
}