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
        get_logger()->debug(__FILE__, __LINE__, "Env::Get::"+string(variable), _default);
        #endif
        return string(_default);
    }
    #ifndef NDEBUG
    get_logger()->debug(__FILE__, __LINE__, "Env::Get::"+string(variable), res);
    #endif
    return string(res);
}
}
