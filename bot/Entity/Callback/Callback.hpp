#pragma once

#include <utils/Entity/Entity.hpp>
#include <pqxx/pqxx>

namespace Bot::Entity::Callback {

    using Utils::Datetime::datetime;
    using Utils::Entity::ID;
    using Utils::Entity::Column;
    using Utils::Entity::Entity;
    using pqxx::row;
    using std::map;
    using std::make_shared;
    using std::string;
    using std::string_view;
    using std::to_string;
    using std::optional;
    using std::nullopt;

    const auto DATA = make_shared<Column>("data");
    const auto MESSAGE_ID = make_shared<Column>("message_id");
    const auto USER_ID = make_shared<Column>("user_id");
    const auto CHAT_ID = make_shared<Column>("chat_id");

    struct Callback : Entity {
        string id;
        string data;
        long long message_id;
        long long user_id;
        long long chat_id;

        Callback(
            string_view data = "",
            long long message_id = 0,
            long long user_id = 0,
            long long chat_id = 0,
            string_view id = "",
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
        Entity(0, created_at, updated_at, deleted_at),
        id(id),
        data(data),
        message_id(message_id),
        user_id(user_id),
        chat_id(chat_id)
        {}


        Callback(const row& callback_row):
            Entity(callback_row),
            id(callback_row[ID->name].get<string>().value()),
            data(callback_row[DATA->name].get<string>().value()),
            message_id(callback_row[MESSAGE_ID->name].get<long long>().value()),
            user_id(callback_row[USER_ID->name].get<long long>().value()),
            chat_id(callback_row[CHAT_ID->name].get<long long>().value())
        {}

        static const char* get_table_name() noexcept {
            static const char* table = "callbacks";
            return table;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result = Entity::to_map(is_full, add_id);

            if (is_full || add_id) {
                result[ID->name] = id;
            }
            result[DATA->name] = data;
            result[MESSAGE_ID->name] = to_string(message_id);
            result[USER_ID->name] = to_string(user_id);
            result[CHAT_ID->name] = to_string(chat_id);

            return result;
        }
    };

}