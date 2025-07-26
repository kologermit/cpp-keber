#include <utils/TGBotApi/Chat/Chat.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::JSONKeys::TYPE_KEY;
using std::map, std::string;

Chat::Chat(const json& json_chat): 
    User(json_chat)
    {
        static const map<string, EnumChatType> chat_type_map = {
            {"private", EnumChatType::PRIVATE},
            {"group", EnumChatType::GROUP},
            {"supergroup", EnumChatType::SUPERGROUP},
            {"channel", EnumChatType::CHANNEL}
        };
        
        auto find_result = chat_type_map.find(json_chat[TYPE_KEY]);

        _type = find_result == chat_type_map.end() ? EnumChatType::UNKNOWN : find_result->second;
    }

EnumChatType Chat::get_type() const noexcept {
    return _type;
}

}
