#pragma once

#include <utils/TGBotApi/Chat/InterfaceChat.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>
#include <utils/TGBotApi/File/File.hpp>
#include <memory>

namespace Utils::TGBotApi::Message {

using Utils::TGBotApi::Chat::InterfaceChat;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::File::EnumContentType;
using std::string, std::unique_ptr;

struct InterfaceMessage {

    virtual long long         get_id()                const noexcept = 0;
    virtual string            get_text()              const noexcept = 0;
    virtual string            get_file_download_id()  const noexcept = 0;
    virtual EnumContentType   get_file_content_type() const noexcept = 0;
    virtual string            get_file_name()         const noexcept = 0;
    virtual long long         get_file_size()         const noexcept = 0;
    virtual const InterfaceUser*   get_from()             const noexcept = 0;
    virtual const InterfaceChat*    get_chat()            const noexcept = 0;
    virtual const InterfaceMessage* get_reply_message()   const noexcept = 0;

    ~InterfaceMessage() = default;

};

}
