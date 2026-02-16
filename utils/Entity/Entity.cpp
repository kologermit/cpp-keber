#include <utils/Entity/Entity.hpp>
#include <fmt/core.h>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Entity {

    using std::pair;
    using std::make_unique;
    using std::runtime_error;
    using fmt::format;
    using pqxx::result;
    using pqxx::row;
    using pqxx::nontransaction;

    #ifndef NDEBUG
    using Logger::get_logger;
    #endif

    void create_rows_in_enum_table_if_empty(connection& conn, const char* table, const map<int, string>& map_int_to_string) {
        nontransaction tx{conn};

        const string select_sql_query = format(
            "SELECT * FROM {} ORDER BY {} ASC",
            tx.quote_name(table),
            tx.quote_name(ID_COLUMN)
        );

        #ifndef NDEBUG
        get_logger()->debug("create_rows_in_enum_table_if_empty::select", select_sql_query, __FILE__, __LINE__);
        #endif

        auto result = tx.exec(select_sql_query);

        if (static_cast<size_t>(result.size()) == static_cast<size_t>(map_int_to_string.size())) {
            for (const auto& [id, name] : map_int_to_string) {
                const auto row = result[id];
                const long long row_id = *row[ID_COLUMN].get<long long>();
                const string row_name = *row[NAME_COLUMN].get<string>();
                if (row_id != id || row_name != name) {
                    throw runtime_error(fmt::format("Not Found row {}:{} in table {}",
                        id, name, table
                    ));
                }
            }
            return;
        }

        if (result.size() > 0) {
            throw runtime_error(fmt::format("Count of rows != {} in table {}", 
                map_int_to_string.size(),
                table
            ));
        }

        string insert_sql_query = format(
            "INSERT INTO {} ({}, {}) VALUES",
            tx.quote_name(table),
            tx.quote_name(ID_COLUMN),
            tx.quote_name(NAME_COLUMN)
        );

        for (const auto&[id, name] : map_int_to_string) {
            insert_sql_query += fmt::format(
                " ({}, {}),",
                tx.quote(id),
                tx.quote(name)
            );
        }

        if (insert_sql_query.ends_with(",")) {
            insert_sql_query.erase(insert_sql_query.end()-1);
        }

        insert_sql_query += ";";

        #ifndef NDEBUG
        get_logger()->debug("create_rows_in_enum_table_if_empty::insert", insert_sql_query, __FILE__, __LINE__);
        #endif

        tx.exec(insert_sql_query);

    }
}
