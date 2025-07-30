#include <utils/UUID4/UUID4.hpp>
#include <uuid/uuid.h>
#include <string>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::UUID4 {

using std::string;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

string generate_str() {
    uuid_t uuid;
    uuid_generate_random(uuid);
    
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);

    #ifndef NDEBUG
    get_logger()->debug(__FILE__, __LINE__, "UUID::generate_str", uuid_str);
    #endif
    
    return string(uuid_str);
}

}
