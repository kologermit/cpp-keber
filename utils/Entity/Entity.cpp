#include <utils/Entity/Entity.hpp>
#include <fmt/core.h>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Entity {

using std::pair;
using std::make_unique;
using fmt::format;
using pqxx::result;
using pqxx::row;
using pqxx::nontransaction;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif

void create_rows_in_enum_table_if_empty(connection& conn, const char* table, const map<int, string>& map_int_to_string) {
    const string select_sql_query = format(
        "SELECT * FROM {}",
        table
    );

    nontransaction tx{conn};

    #ifndef NDEBUG
    get_logger()->debug("create_rows_in_enum_table_if_empty::select", select_sql_query, __FILE__, __LINE__);
    #endif

    auto result = tx.exec(select_sql_query);

    if (!result.empty()) {
        return;
    }

    string insert_sql_query = format(
        "INSERT INTO {} ({}, {}) VALUES",
        table,
        ID_COLUMN,
        NAME_COLUMN
    );

    for (const auto& service : map_int_to_string) {
        insert_sql_query += format(
            " ({}, {}),",
            tx.quote(service.first),
            tx.quote(service.second)
        );
    }

    if (insert_sql_query.ends_with(",")) {
        insert_sql_query.erase(insert_sql_query.end()-1);
    }

    #ifndef NDEBUG
    get_logger()->debug("create_rows_in_enum_table_if_empty::insert", insert_sql_query, __FILE__, __LINE__);
    #endif

    tx.exec(insert_sql_query);

}

}