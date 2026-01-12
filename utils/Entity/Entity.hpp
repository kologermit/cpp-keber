#pragma once

#include <utils/Datetime.hpp>
#include <optional>
#include <string>
#include <map>
#include <pqxx/pqxx>
#include <nlohmann/json.hpp>

namespace Utils::Entity {
    using std::nullopt;
    using std::optional;
    using std::string;
    using std::shared_ptr;
    using std::to_string;
    using std::make_shared;
    using std::map;
    using Datetime::datetime;
    using Datetime::DATETIME_FORMAT;
    using pqxx::row;
    using pqxx::connection;
    using nlohmann::json;

    constexpr const char* ID_COLUMN = "id";
    constexpr const char* CREATED_AT_COLUMN = "created_at";
    constexpr const char* UPDATED_AT_COLUMN = "updated_at";
    constexpr const char* DELETED_AT_COLUMN = "deleted_at";
    constexpr const char* NAME_COLUMN = "name";

    struct Entity {
        
        long long id = 0;
        datetime created_at{};
        datetime updated_at{};
        optional<datetime> deleted_at = nullopt;

        explicit Entity(
            long long id, 
            datetime created_at = {}, 
            datetime updated_at = {}, 
            optional<datetime> deleted_at = nullopt
        ):
            id(id), 
            created_at(std::move(created_at)), 
            updated_at(std::move(updated_at)), 
            deleted_at(std::move(deleted_at)) 
        {}

        explicit Entity(const row& entity_row):
            id(entity_row[ID_COLUMN].get<long long>().value_or(0)),
            created_at(datetime::parse(DATETIME_FORMAT, entity_row[CREATED_AT_COLUMN].get<string>().value())),
            updated_at(datetime::parse(DATETIME_FORMAT, entity_row[UPDATED_AT_COLUMN].get<string>().value())),
            deleted_at(
                !entity_row[DELETED_AT_COLUMN].is_null()
                ? optional<datetime>(datetime::parse(DATETIME_FORMAT, entity_row[DELETED_AT_COLUMN].get<string>().value()))
                : nullopt
            )
        {}

        static const char* get_table_name() noexcept {
            static const char* table_name = "entities";
            return table_name;
        }

        map<const char*, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            map<const char*, optional<string> > result;
            if (add_id) {
                result[ID_COLUMN] = to_string(id);
            }
            if (is_full) {
                result[ID_COLUMN] = to_string(id);
                result[CREATED_AT_COLUMN] = created_at.to_string(DATETIME_FORMAT);
                result[UPDATED_AT_COLUMN] = updated_at.to_string(DATETIME_FORMAT);
                result[DELETED_AT_COLUMN] = (deleted_at.has_value() ? optional<string>(deleted_at.value().to_string(DATETIME_FORMAT)) : nullopt);
            }
            return result;
        }

        json to_json() const noexcept {
            json result{
                {ID_COLUMN, id},
                {CREATED_AT_COLUMN, created_at.to_string(DATETIME_FORMAT)},
                {UPDATED_AT_COLUMN, updated_at.to_string(DATETIME_FORMAT)}
            };
            if (deleted_at.has_value()) {
                result[DELETED_AT_COLUMN] = deleted_at->to_string(DATETIME_FORMAT);
            } else {
                result[DELETED_AT_COLUMN] = nullptr;
            }
            return result;
        }
    };

    void create_rows_in_enum_table_if_empty(connection& conn, const char* table, const map<int, string>& map_int_to_string);
}