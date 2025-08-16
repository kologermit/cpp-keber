#pragma once

#include <utils/Datetime.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <memory>
#include <map>
#include <pqxx/pqxx>

#define GET_PAIR_OF_OBJECT_AND_STRINGOBJECT(obj) {obj, #obj}

namespace Utils::Entity {

using std::nullopt;
using std::optional;
using std::string;
using std::string_view;
using std::map;
using std::unique_ptr;
using jed_utils::datetime;
using Utils::Datetime::DATETIME_FORMAT;
using pqxx::row;
using pqxx::connection;

enum EntityColumns {
    ID,
    CREATED_AT,
    UPDATED_AT,
    DELETED_AT,
};

constexpr const char* ID_COLUMN = "\"id\"";
constexpr const char* NAME_COLUMN = "\"name\"";
constexpr const char* CREATED_AT_COLUMN = "\"created_at\"";
constexpr const char* UPDATED_AT_COLUMN = "\"updated_at\"";
constexpr const char* DELETED_AT_COLUMN = "\"deleted_at\"";

void create_rows_in_enum_table_if_empty(connection& conn, const char* table, const map<int, string>& map_int_to_string);

struct Entity {

    int                id;
    datetime           created_at;
    datetime           updated_at;
    optional<datetime> deleted_at;

    Entity(
        int id = 0, 
        optional<datetime> created_at = nullopt, 
        optional<datetime> updated_at = nullopt, 
        optional<datetime> deleted_at = nullopt
    ):
        id(id),
        created_at(created_at.value_or(datetime())),
        updated_at(
            updated_at.has_value()
            ? *updated_at
            : deleted_at.has_value()
            ? *deleted_at
            : created_at.has_value()
            ? *created_at
            : datetime()
        ),
        deleted_at(deleted_at)
    {}

    Entity(const row& entity_row):
    id(entity_row[ID_COLUMN].get<int>().value()),
    created_at(datetime::parse(DATETIME_FORMAT, entity_row[CREATED_AT_COLUMN].get<string>().value())),
    updated_at(datetime::parse(DATETIME_FORMAT, entity_row[UPDATED_AT_COLUMN].get<string>().value())),
    deleted_at(
        entity_row[DELETED_AT_COLUMN].is_null()
        ? nullopt
        : optional<datetime>(datetime::parse(DATETIME_FORMAT, entity_row[DELETED_AT_COLUMN].get<string>().value()))
    ){}

};
}