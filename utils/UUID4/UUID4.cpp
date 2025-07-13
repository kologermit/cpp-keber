#include <utils/UUID4/UUID4.hpp>
#include <uuid/uuid.h>
#include <string>

namespace Utils::UUID4 {

using std::string;

string generate_str() {
    uuid_t uuid;
    uuid_generate_random(uuid);
    
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    
    return string(uuid_str);
}

}
