#pragma once

#include <utils/Type.cpp>
#include <optional>

namespace Utils {
namespace TGBotApi {
namespace User {

using namespace Utils::Type;

class InterfaceUser {
    public:
        virtual int get_id() const = 0;
        virtual const_string& get_name() const = 0;
        virtual const_string& get_username() const = 0;
        virtual bool get_can_join_groups() const = 0;
        virtual bool get_can_read_all_group_messages() const = 0;
        virtual bool get_supports_inline_queries() const = 0;
        virtual bool get_can_connect_to_business() const = 0;
        virtual bool get_has_main_web_app() const = 0;
};

}
}
}