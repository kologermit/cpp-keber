#pragma once

#include <utils/Entity/Entity.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Callback {

    using Utils::Datetime::datetime;
    using Utils::Entity::Entity;
    using pqxx::row;
    using std::string;
    using std::string_view;
    using std::to_string;
    using std::optional;
    using std::nullopt;

    enum CallbackColumn {
        TELEGRAM_ID,
        MESSAGE_ID,
        DATA,
        USER_ID,
        CHAT_ID,
    };

    constexpr const char* CALLBACKS_TABLE = "\"callbacks\"";
    constexpr const char* TELEGRAM_ID_COLUMN = "\"telegram_id\"";
    constexpr const char* MESSAGE_ID_COLUMN = "\"message_id\"";
    constexpr const char* DATA_COLUMN = "\"data\"";
    constexpr const char* USER_ID_COLUMN = "\"user_id\"";
    constexpr const char* CHAT_ID_COLUMN = "\"chat_id\"";

    struct Callback : Entity {
        string telegram_id;
        string data;
        int message_id;
        int user_id;
        int chat_id;

        Callback(
            int id,
            optional<datetime> created_at = nullopt,
            optional<datetime> updated_at = nullopt,
            optional<datetime> deleted_at = nullopt,
            string_view telegram_id = "",
            string_view data = "",
            int message_id = 0,
            int user_id = 0,
            int chat_id = 0
        ):
        Entity(id, created_at, updated_at, deleted_at),
        telegram_id(telegram_id),
        data(data),
        message_id(message_id),
        user_id(user_id),
        chat_id(chat_id)
        {}

        Callback(
            string_view telegram_id,
            string_view data,
            int message_id,
            int user_id,
            int chat_id
        ):
        telegram_id(telegram_id),
        data(data),
        message_id(message_id),
        user_id(user_id),
        chat_id(chat_id)
        {}

        Callback(const row& callback_row):
        telegram_id(callback_row[TELEGRAM_ID].get<string>().value()),
        data(callback_row[DATA_COLUMN].get<string>().value()),
        message_id(callback_row[MESSAGE_ID_COLUMN].get<int>().value()),
        user_id(callback_row[USER_ID_COLUMN].get<int>().value()),
        chat_id(callback_row[CHAT_ID_COLUMN].get<int>().value())
        {}

        string get_data_by_column(int column) const {
            switch (column)
            {
                case TELEGRAM_ID: return telegram_id;
                case MESSAGE_ID: return to_string(message_id);
                case DATA: return data;
                case USER_ID: return to_string(user_id);
                case CHAT_ID: return to_string(chat_id);
                default: return "";
            }
        }
    };

}