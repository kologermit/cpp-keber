#pragma once

#include <utils/Types.cpp>
#include <utils/Functions.cpp>

namespace Utils {
namespace Env {

using Utils::Types::ConstString;
using Utils::Types::ConstCString;
using Utils::Functions::GetEnv;

ConstString Get(ConstString variable, ConstString _default="") {
    ConstCString res = GetEnv(variable.c_str());
    if (res == nullptr) {
        return ConstString(_default);
    }
    return ConstString(res);
}
}
};