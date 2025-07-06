#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>

namespace Utils {
namespace TGBotApi {
namespace User {

using namespace std;
using namespace Type;
using namespace JSONKeys;
using namespace nlohmann;

class User : public virtual InterfaceUser {
    protected:
        int _id;
        string _name;
        string _username;
        bool _can_join_groups;
        bool _can_read_all_group_messages;
        bool _supports_inline_queries;
        bool _can_connect_to_business;
        bool _has_main_web_app;
        
    public:

        User(
            const_int& id=0,
            const_string& name="", 
            const_string& username="",
            const_bool& can_join_groups=false,
            const_bool& can_read_all_group_messages=false,
            const_bool& supports_inline_queries=false,
            const_bool& can_connect_to_business=false,
            const_bool& has_main_web_app=false
        ): 
            _id(id), 
            _name(name), 
            _username(username),
            _can_join_groups(can_join_groups),
            _can_read_all_group_messages(can_read_all_group_messages),
            _supports_inline_queries(supports_inline_queries),
            _can_connect_to_business(can_connect_to_business),
            _has_main_web_app(has_main_web_app)
        {}

        User(InterfaceUser& user):
            _id(user.get_id()), 
            _name(user.get_name()), 
            _username(user.get_username()),
            _can_join_groups(user.get_can_join_groups()),
            _can_read_all_group_messages(user.get_can_read_all_group_messages()),
            _supports_inline_queries(user.get_supports_inline_queries()),
            _can_connect_to_business(user.get_can_connect_to_business()),
            _has_main_web_app(user.get_has_main_web_app())
        {}

        User(json json_result): 
            _id(json_result.contains(ID_KEY) ? int(json_result[ID_KEY]) : 0), 
            _name(json_result.contains(FIRST_NAME_KEY) ? string(json_result[FIRST_NAME_KEY]) : ""), 
            _username(json_result.contains(USERNAME_KEY) ? string(json_result[USERNAME_KEY]) : ""),
            _can_join_groups(json_result.contains(CAN_JOIN_GROUPS_KEY) ? bool(json_result[CAN_JOIN_GROUPS_KEY]) : false),
            _can_read_all_group_messages(json_result.contains(CAN_READ_ALL_GROUP_MESSAGES_KEY) ? bool(json_result[CAN_READ_ALL_GROUP_MESSAGES_KEY]) : false),
            _supports_inline_queries(json_result.contains(SUPPORTS_INLINE_QUERIES_KEY) ? bool(json_result[SUPPORTS_INLINE_QUERIES_KEY]) : false),
            _can_connect_to_business(json_result.contains(CAN_CONNECT_TO_BUSINESS_KEY) ? bool(json_result[CAN_CONNECT_TO_BUSINESS_KEY]) : false),
            _has_main_web_app(json_result.contains(HAS_MAIN_WEB_APP_KEY) ? bool(json_result[HAS_MAIN_WEB_APP_KEY]) : false)
        {}

        int get_id() const override { return _id; }
        const_string& get_name() const override { return _name; }
        const_string& get_username() const override { return _username; }
        bool get_can_join_groups() const override { return _can_join_groups; }
        bool get_can_read_all_group_messages() const override { return _can_read_all_group_messages; }
        bool get_supports_inline_queries() const override { return _supports_inline_queries; }
        bool get_can_connect_to_business() const override { return _can_connect_to_business; }
        bool get_has_main_web_app() const override { return _has_main_web_app; }
};

}
}
}