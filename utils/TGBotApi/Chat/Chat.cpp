#include <utils/TGBotApi/Chat/Chat.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::JSONKeys::TYPE_KEY;
using std::map, std::string;

Chat::Chat(
    long long id,
    string_view name, 
    string_view username,
    EnumChatType type
):
    User(id, name, username),
    type(type)
{}

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

        type = find_result == chat_type_map.end() ? EnumChatType::UNKNOWN : find_result->second;
    }

}
