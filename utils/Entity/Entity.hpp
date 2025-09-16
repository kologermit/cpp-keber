#pragma once

#include <utils/Datetime.hpp>
#include <optional>
#include <string>
#include <map>
#include <pqxx/pqxx>

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

    struct Column {
        const char* name;
        explicit Column(const char* name) : name(name) {}
    };

    struct Entity {

        static const char* get_table_name() noexcept;
        [[nodiscard]] map<const char*, optional<string> > to_map(bool is_full = false) const noexcept;

        int id = 0;
        datetime created_at{};
        datetime updated_at{};
        optional<datetime> deleted_at = nullopt;

        explicit Entity(const row& entity_row);
        Entity(int id, datetime created_at, datetime updated_at, optional<datetime> deleted_at = nullopt);
        Entity() = default;

    };

    inline const char* Entity::get_table_name() noexcept {
        static const char* table_name = "\"entities\"";
        return table_name;
    }

    inline auto ID = make_shared<Column>("\"id\"");
    inline auto CREATED_AT = make_shared<Column>("\"created_at\"");
    inline auto UPDATED_AT = make_shared<Column>("\"updated_at\"");
    inline auto DELETED_AT = make_shared<Column>("\"deleted_at\"");
    inline auto NAME = make_shared<Column>("\"name\"");

    inline Entity::Entity(int id, datetime created_at, datetime updated_at, optional<datetime> deleted_at):
        id(id), created_at(std::move(created_at)), updated_at(std::move(updated_at)), deleted_at(std::move(deleted_at)) {}
    inline Entity::Entity(const row& entity_row):
    id(entity_row[ID->name].get<int>().value()),
    created_at(datetime::parse(DATETIME_FORMAT, entity_row[CREATED_AT->name].get<string>().value())),
    updated_at(datetime::parse(DATETIME_FORMAT, entity_row[UPDATED_AT->name].get<string>().value())) {
        if (const auto deleted_at_value = entity_row[DELETED_AT->name].get<string>();
            deleted_at_value.has_value()) {
            deleted_at = datetime::parse(DATETIME_FORMAT, deleted_at_value.value());
        }
    }

    inline map<const char*, optional<string> > Entity::to_map(bool is_full) const noexcept {
        if (!is_full) {
            return {};
        }
        return {
            {ID->name, to_string(id)},
            {CREATED_AT->name, created_at.to_string(DATETIME_FORMAT)},
            {UPDATED_AT->name, updated_at.to_string(DATETIME_FORMAT)},
            {DELETED_AT->name, (deleted_at.has_value() ? optional<string>(deleted_at.value().to_string(DATETIME_FORMAT)) : nullopt)},
        };
    }

    void create_rows_in_enum_table_if_empty(connection& conn, const char* table, const map<int, string>& map_int_to_string);
}