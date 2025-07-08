#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::Message {

using Utils::Types::const_string;
using nlohmann::json;

struct Message : InterfaceMessage {
    int get_id() const noexcept override;
    string get_text() const noexcept override;
    ptrUser from() const noexcept override;
    ptrChat chat() const noexcept override;

    Message(const json&);

    protected:

    int _id;
    const_string _text;
    ptrUser _from;
    ptrChat _chat;
};

}
