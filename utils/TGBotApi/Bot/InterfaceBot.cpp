#pragma once

#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>

namespace Utils {
namespace TGBotApi {
namespace Bot {

using namespace Utils::TGBotApi::User;

class InterfaceBot : public virtual InterfaceUser {
    public:
    virtual const InterfaceUser& get_me() const = 0;
    virtual void delete_webhook() const = 0;
    virtual void set_webhook(const_string&) const = 0;
};

}
}
}