#include <utils/Env/Env.hpp>
#include <utils/Types.hpp>
#include <cstdlib>

#ifdef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Env {

using Utils::Types::const_c_string;
using std::getenv;
using std::string;
using std::string_view;
#ifdef NDEBUG
using Utils::Logger::get_logger;
#endif

string Get(string_view variable, string_view _default) {
    const_c_string res = getenv(variable.data());
    if (res == nullptr) {
        #ifdef NDEBUG
        get_logger()->debug("Env::Get::"+string(variable), _default);
        #endif
        return string(_default);
    }
    #ifdef NDEBUG
    get_logger()->debug("Env::Get::"+string(res), _default);
    #endif
    return string(res);
}
}
