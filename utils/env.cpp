#pragma once

#include <cstdlib>

namespace utils {
const char* get_env(const char* variable, const char* _default="") {
    const char* res = getenv(variable);
    if (res == nullptr) {
        return _default;
    }
    return res;
}

};