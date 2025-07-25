#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::Types::const_string;
using std::shared_ptr;

struct InterfaceCallbackQuery {

    virtual const_string                 get_id()      const noexcept = 0;
    virtual shared_ptr<InterfaceUser>    get_from()    const noexcept = 0;
    virtual shared_ptr<InterfaceMessage> get_message() const noexcept = 0;
    virtual const_string                 get_data()    const noexcept = 0;

};

using ptrCallbackQuery = std::shared_ptr<InterfaceCallbackQuery>;

}