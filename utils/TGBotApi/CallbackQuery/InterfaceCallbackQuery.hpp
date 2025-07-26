#pragma once

#include <utils/TGBotApi/Message/InterfaceMessage.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::User::InterfaceUser;
using std::unique_ptr, std::string;

struct InterfaceCallbackQuery {

    virtual string                  get_id()      const noexcept = 0;
    virtual string                  get_data()    const noexcept = 0;
    virtual const InterfaceUser*    get_from()    const noexcept = 0;
    virtual const InterfaceMessage* get_message() const noexcept = 0;

};

}