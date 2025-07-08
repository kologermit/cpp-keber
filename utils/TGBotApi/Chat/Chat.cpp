#pragma once

#include <utils/TGBotApi/Chat/Chat.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::JSONKeys::TYPE_KEY;
using std::map;
using std::string;

Chat::Chat(const json& json_chat): 
User(json_chat), 
_type(
    map<string, EnumChatType>{
        {"private", EnumChatType::PRIVATE}
    }
    [json_chat[TYPE_KEY]]
) {}

EnumChatType Chat::get_type() const noexcept {
    return _type;
}

}
