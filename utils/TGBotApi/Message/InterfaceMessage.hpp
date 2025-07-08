#pragma once

#include <utils/TGBotApi/Chat/InterfaceChat.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::InterfaceChat;
using Utils::TGBotApi::User::InterfaceUser;
using std::shared_ptr, std::string;

struct InterfaceMessage {
    using ptrUser = shared_ptr<InterfaceUser>;
    using ptrChat = shared_ptr<InterfaceChat>;

    virtual int get_id() const noexcept = 0;
    virtual string get_text() const noexcept = 0;
    virtual ptrUser from() const noexcept = 0;
    virtual ptrChat chat() const noexcept = 0; 

    ~InterfaceMessage() = default;

};

}
