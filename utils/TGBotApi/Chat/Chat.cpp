#pragma once

#include <utils/TGBotApi/Chat/InterfaceChat.cpp>
#include <utils/TGBotApi/User/User.cpp>
#include <utils/TGBotApi/JSONKeys.cpp>
#include <utils/Type.cpp>
#include <string_view>
#include <string>
#include <map>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::Type::const_string;
using Utils::TGBotApi::User::User;
using Utils::TGBotApi::JSONKeys::TYPE_KEY;
using std::string_view, std::map, std::string;
using nlohmann::json;

struct Chat : virtual InterfaceChat, User {
    EnumChatType get_type() const noexcept override;
    Chat(const json&);

    protected:
    EnumChatType _type;
};

Chat::Chat(const json& json_chat): User(json_chat), 
_type(
    map<string, EnumChatType>{
        {"private", EnumChatType::PRIVATE}
    }
    [json_chat[TYPE_KEY]]) {}

EnumChatType Chat::get_type() const noexcept {
    return _type;
}

}