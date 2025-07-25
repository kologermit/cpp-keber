#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message {

using Utils::Types::const_string;
using nlohmann::json;

struct Message : InterfaceMessage {
    long long                    get_id()                const noexcept override;
    optional_const_string        get_text()              const noexcept override;
    optional_const_string        get_file_download_id()  const noexcept override;
    EnumContentType              get_file_content_type() const noexcept override;
    optional_const_string        get_file_name()         const noexcept override;
    optional_long_long           get_file_size()         const noexcept override;
    shared_ptr<InterfaceUser>    get_from()              const noexcept override;
    shared_ptr<InterfaceChat>    get_chat()              const noexcept override;
    shared_ptr<InterfaceMessage> get_reply_message()     const noexcept override;

    explicit Message(const json&);

    private:

    long long                    _id;
    optional_const_string        _text;
    optional_const_string        _file_download_id;
    EnumContentType              _file_content_type;
    optional_const_string        _file_name;
    optional_long_long           _file_size;
    shared_ptr<InterfaceUser>    _from;
    shared_ptr<InterfaceChat>    _chat;
    shared_ptr<InterfaceMessage> _reply_message;
};

}
