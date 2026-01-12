#include <utils/TGBotApi/Chat/Chat.hpp>
#include <utils/TGBotApi/JSONKeys.hpp>

namespace Utils::TGBotApi::Chat {

using Utils::TGBotApi::JSONKeys::TYPE_KEY;
using std::map, std::string;

Chat::Chat(
    long long id,
    string_view name, 
    string_view username,
    ChatType type
):
    User(id, name, username),
    type(type)
{}

Chat::Chat(const json& json_chat): 
    User(json_chat)
    {
        static const map<string, ChatType> chat_type_map = {
            {"private", ChatType::PRIVATE},
            {"group", ChatType::GROUP},
            {"supergroup", ChatType::SUPERGROUP},
            {"channel", ChatType::CHANNEL}
        };
        
        auto find_result = chat_type_map.find(json_chat[TYPE_KEY]);

        type = find_result == chat_type_map.end() ? ChatType::UNKNOWN : find_result->second;
    }

}
