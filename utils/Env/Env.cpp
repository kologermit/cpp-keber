#include <utils/Env/Env.hpp>
#include <cstdlib>
#include <string>
#include <string_view>

namespace Utils::Env {

using std::getenv;
using std::string;
using std::string_view;

string Get(string_view variable, string_view _default) {
    const char* res = getenv(variable.data());
    if (res == nullptr) {
        return string(_default);
    }
    return string(res);
}
}
