#pragma once

#include <memory>
#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>

namespace Utils {
namespace TGBotApi {
namespace Bot {

using namespace std;
using namespace Utils::TGBotApi::User;

struct InterfaceBot : virtual InterfaceUser {

    virtual bool get_can_join_groups() const = 0;
    virtual bool get_can_read_all_group_messages() const = 0;
    virtual bool get_supports_inline_queries() const = 0;
    virtual bool get_can_connect_to_business() const = 0;
    virtual bool get_has_main_web_app() const = 0;
    
    virtual void delete_webhook() const = 0;
    virtual void set_webhook(const_string&) const = 0;
};

}
}
}