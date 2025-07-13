#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message {

using Utils::Types::const_string;
using nlohmann::json;

struct Message : InterfaceMessage {
    int get_id() const noexcept override;
    optional_const_string get_text() const noexcept override;
    optional_const_string get_photo_id() const noexcept override;
    optional_const_string get_document_id() const noexcept override;
    optional_const_string get_document_name() const noexcept override;
    ptrUser from() const noexcept override;
    ptrChat chat() const noexcept override;

    explicit Message(const json&);

    protected:

    int _id;
    optional_const_string _text;
    optional_const_string _photo_id;
    optional_const_string _document_id;
    optional_const_string _document_name;
    ptrUser _from;
    ptrChat _chat;
};

}
