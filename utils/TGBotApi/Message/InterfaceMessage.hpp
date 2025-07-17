#pragma once

#include <utils/TGBotApi/Chat/InterfaceChat.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>
#include <utils/TGBotApi/File/File.hpp>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::InterfaceChat;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::File::EnumContentType;
using Utils::Types::optional_const_string;
using Utils::Types::optional_int;
using std::shared_ptr, std::string;

struct InterfaceMessage {
    using ptrUser = shared_ptr<InterfaceUser>;
    using ptrChat = shared_ptr<InterfaceChat>;
    using ptrMessage = shared_ptr<InterfaceMessage>;

    virtual int                     get_id()            const noexcept = 0;
    virtual optional_const_string   get_text()          const noexcept = 0;
    virtual optional_const_string   get_file_download_id() const noexcept = 0;
    virtual EnumContentType         get_file_content_type() const noexcept = 0;
    virtual optional_const_string   get_file_name()     const noexcept = 0;
    virtual optional_int            get_file_size()     const noexcept = 0;
    virtual ptrUser                 get_from()          const noexcept = 0;
    virtual ptrChat                 get_chat()          const noexcept = 0;
    virtual ptrMessage              get_reply_message() const noexcept = 0;

    ~InterfaceMessage() = default;

};

}
