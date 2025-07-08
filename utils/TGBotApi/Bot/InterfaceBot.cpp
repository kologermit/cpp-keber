#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utils/Type.cpp>
#include <utils/TGBotApi/User/InterfaceUser.cpp>
#include <utils/TGBotApi/Message/InterfaceMessage.cpp>

namespace Utils::TGBotApi::Bot {

using std::string_view, std::shared_ptr;
using Utils::Type::const_string;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::Message::InterfaceMessage;

struct InterfaceBot : virtual InterfaceUser {
    virtual bool can_join_groups() const noexcept = 0;
    virtual bool can_read_all_group_messages() const noexcept = 0;
    virtual bool supports_inline_queries() const noexcept = 0;
    virtual bool can_connect_to_business() const noexcept = 0;
    virtual bool has_main_web_app() const noexcept = 0;

    using ptrMessage = shared_ptr<InterfaceMessage>;
    
    virtual ptrMessage send_text(int, string_view) const = 0;
    virtual void delete_webhook() const = 0;
    virtual void set_webhook(string_view) const = 0;

    ~InterfaceBot() = default;
};

}