#pragma once

#include <utils/Env/Env.hpp>
#include <utils/Types.hpp>
#include <cstdlib>

namespace Utils::Env {

using Utils::Types::const_string;
using Utils::Types::const_c_string;
using std::getenv;
using std::string;
using std::string_view;

string Get(string_view variable, string_view _default) {

    const_c_string res = getenv(variable.data());
    if (res == nullptr) {
        return const_string(_default);
    }
    return const_string(res);
}
}
