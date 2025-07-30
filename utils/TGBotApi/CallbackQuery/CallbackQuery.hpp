#pragma once

#include <utils/TGBotApi/CallbackQuery/InterfaceCallbackQuery.hpp>
#include <nlohmann/json.hpp>

namespace Utils::TGBotApi::CallbackQuery {

using std::unique_ptr;
using nlohmann::json;

struct CallbackQuery : InterfaceCallbackQuery {

    string                  get_id()      const noexcept override;
    string                  get_data()    const noexcept override;
    const InterfaceUser*    get_from()    const noexcept override;
    const InterfaceMessage* get_message() const noexcept override;

    CallbackQuery(const json& json_callback_query);

    private:

    string                 _id;
    string                 _data;
    unique_ptr<InterfaceUser>    _from;
    unique_ptr<InterfaceMessage> _message;

};

}