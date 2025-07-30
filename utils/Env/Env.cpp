#include <utils/Env/Env.hpp>
#include <utils/Types.hpp>
#include <cstdlib>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Env {

using Utils::Types::const_c_string;
using std::getenv;
using std::string;
using std::string_view;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

string Get(string_view variable, string_view _default) {
    const_c_string res = getenv(variable.data());
    if (res == nullptr) {
        #ifndef NDEBUG
        get_logger()->debug(variable, _default, __FILE__, __LINE__);
        #endif
        return string(_default);
    }
    #ifndef NDEBUG
    get_logger()->debug(variable, res, __FILE__, __LINE__);
    #endif
    return string(res);
}
}
