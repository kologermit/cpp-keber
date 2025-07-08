#pragma once

#include <utils/Type.cpp>
#include <optional>

namespace Utils {
namespace TGBotApi {
namespace User {

using namespace Utils::Type;

struct InterfaceUser {
    virtual int get_id() const = 0;
    virtual const_string& get_name() const = 0;
    virtual const_string& get_username() const = 0;
};

}
}
}