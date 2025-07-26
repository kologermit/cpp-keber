#pragma once

#include <string>

namespace Utils::TGBotApi::JSONEncode {

using std::string;

struct InterfaceJSONEncode {

    virtual string get_json() const noexcept = 0;

};

}