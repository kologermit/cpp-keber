#pragma once

#include <cstdlib>
#include <utils/Types.cpp>

using Utils::Types::ConstString;
using Utils::Types::ConstCString;

namespace Utils {
namespace Env {

ConstString get(ConstString variable, ConstString _default="") {
    ConstCString res = getenv(variable.c_str());
    if (res == nullptr) {
        return ConstString(_default);
    }
    return ConstString(res);
}
}
};