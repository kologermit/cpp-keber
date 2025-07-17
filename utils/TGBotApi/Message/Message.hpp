#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message {

using Utils::Types::const_string;
using nlohmann::json;

struct Message : InterfaceMessage {
    int                     get_id()            const noexcept override;
    optional_const_string   get_text()          const noexcept override;
    optional_const_string   get_file_download_id() const noexcept override;
    EnumContentType         get_file_content_type() const noexcept override;
    optional_const_string   get_file_name()     const noexcept override;
    optional_int            get_file_size()     const noexcept override;
    ptrUser                 get_from()          const noexcept override;
    ptrChat                 get_chat()          const noexcept override;
    ptrMessage              get_reply_message() const noexcept override;

    explicit Message(const json&);

    private:

    int                     _id;
    optional_const_string   _text;
    optional_const_string   _file_download_id;
    EnumContentType         _file_content_type;
    optional_const_string   _file_name;
    optional_int            _file_size;
    ptrUser                 _from;
    ptrChat                 _chat;
    ptrMessage              _reply_message;
};

}
