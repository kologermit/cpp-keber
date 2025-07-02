#pragma once

#include <utils/Type.cpp>
#include <cstdlib>
#include <string>

namespace Utils {
namespace Env {

using namespace Utils::Type;
using namespace std;

string 
Get(const_string variable, const_string _default="") {

    const_c_string res = getenv(variable.c_str());
    if (res == nullptr) {

        return const_string(_default);

    }

    return const_string(res);
}
}
};