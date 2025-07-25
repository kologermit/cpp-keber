#pragma once

#include <utils/TGBotApi/CallbackQuery/InterfaceCallbackQuery.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using std::shared_ptr;
using nlohmann::json;

struct CallbackQuery : virtual InterfaceCallbackQuery {

    const_string                 get_id()      const noexcept override;
    shared_ptr<InterfaceUser>    get_from()    const noexcept override;
    shared_ptr<InterfaceMessage> get_message() const noexcept override;
    const_string                 get_data()    const noexcept override;

    CallbackQuery(const json& json_callback_query);

    private:

    const_string                 _id;
    const_string                 _data;
    shared_ptr<InterfaceUser>    _from;
    shared_ptr<InterfaceMessage> _message;

};

}