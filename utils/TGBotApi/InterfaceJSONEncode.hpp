#pragma once

#include <utils/Types.hpp>

namespace Utils::TGBotApi::JSONEncode {

using Utils::Types::const_string;

struct InterfaceJSONEncode {

    virtual const_string get_json() const noexcept = 0;

};

}