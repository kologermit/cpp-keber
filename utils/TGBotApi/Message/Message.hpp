#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message {

using nlohmann::json;

struct Message : InterfaceMessage {
    long long         get_id()                const noexcept override;
    string            get_text()              const noexcept override;
    string            get_file_download_id()  const noexcept override;
    EnumContentType   get_file_content_type() const noexcept override;
    string            get_file_name()         const noexcept override;
    long long         get_file_size()         const noexcept override;
    const InterfaceUser*    get_from()             const noexcept override;
    const InterfaceChat*    get_chat()             const noexcept override;
    const InterfaceMessage* get_reply_message()    const noexcept override;

    explicit Message(const json&);

    private:

    long long         _id;
    string            _text;
    string            _file_download_id;
    EnumContentType   _file_content_type;
    string            _file_name;
    long long         _file_size;
    unique_ptr<InterfaceUser>    _from;
    unique_ptr<InterfaceChat>    _chat;
    unique_ptr<InterfaceMessage> _reply_message;
};

}
