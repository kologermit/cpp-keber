#pragma once

#include <string>
#include <string_view>
#include <memory>
#include <utils/Types.hpp>
#include <utils/TGBotApi/User/InterfaceUser.hpp>
#include <utils/TGBotApi/Message/InterfaceMessage.hpp>

namespace Utils::TGBotApi::Bot {

using std::string_view, std::shared_ptr, std::nullopt, std::optional;
using Utils::Types::const_string;
using Utils::Types::optional_string_view;
using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::Message::InterfaceMessage;

struct InterfaceBot : virtual InterfaceUser {
    virtual bool can_join_groups() const noexcept = 0;
    virtual bool can_read_all_group_messages() const noexcept = 0;
    virtual bool supports_inline_queries() const noexcept = 0;
    virtual bool can_connect_to_business() const noexcept = 0;
    virtual bool has_main_web_app() const noexcept = 0;

    using ptrMessage = shared_ptr<InterfaceMessage>;
    
    virtual ptrMessage send_text(int chat_id, string_view text, optional_string_view reply_message_id = nullopt) const = 0;
    virtual ptrMessage send_photo(
        int chat_id, string_view filepath, 
        optional_string_view text = nullopt, 
        optional_string_view reply_message_id = nullopt
    ) const = 0;
    // virtual ptrMessage send_document(
    //     int chat_id, 
    //     string_view filepath, 
    //     optional_string_view filename = nullopt, 
    //     optional_string_view text = nullopt
    // ) const = 0;
    virtual void delete_webhook() const = 0;
    virtual void set_webhook(string_view) const = 0;

    ~InterfaceBot() = default;
};

}
