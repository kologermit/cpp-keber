#pragma once

#include <utils/Logger/InterfaceLogger.hpp>
#include <fmt/core.h>
#include <pqxx/pqxx>
#include <vector>
#include <stdexcept>

namespace Utils::Entity {

    using Logger::get_logger;
    using std::pair;
    using std::make_unique;
    using std::unique_ptr;
    using std::vector;
    using std::runtime_error;
    using fmt::format;
    using pqxx::result;
    using pqxx::row;
    using pqxx::nontransaction;

    template<typename EntityT>
    unique_ptr<EntityT> exec_insert(connection& conn, const EntityT& entity, bool add_id = false) {
        nontransaction tx{conn};

        string sql_columns;
        string sql_values;

        if (add_id) {
            sql_columns = format(" {},", tx.quote_name(ID->name));
            sql_values = format(" {},", tx.quote(entity.id));
        }

        for (const auto& data : entity.to_map(false)) {
            const auto& [column, value] = data;
            sql_columns += format(
                " {},",
                tx.quote_name(column)
            );

            sql_values += format(
                " {},",
                tx.quote(value)
            );
        }

        if (sql_columns.ends_with(",")) {
            sql_columns.erase(sql_columns.end()-1);
        }

        if (sql_values.ends_with(",")) {
            sql_values.erase(sql_values.end()-1);
        }

        const string sql_query = format(
            "INSERT INTO {} ({}) VALUES ({}) RETURNING *;",
            tx.quote_name(EntityT::get_table_name()),
            sql_columns,
            sql_values
        );

        #ifndef NDEBUG
        get_logger()->debug("exec_insert::sql_query", sql_query, __FILE__, __LINE__);
        #endif

        auto res = tx.exec(sql_query);

        if (res.empty()) {
            throw runtime_error(format("exec_insert::insert_error -- row doesnt insert in table {}", EntityT::get_table_name()));
        }

        return make_unique<EntityT>(res.one_row());

    }

    template<typename EntityT>
    unique_ptr<EntityT> exec_update_by_id(connection& conn, const EntityT& entity) {
        nontransaction tx{conn};

        string sql_query = format(
            "UPDATE {} SET",
            tx.quote_name(EntityT::get_table_name())
        );

        for (const auto& iter : entity.to_map(false)) {
            const auto& [column, value] = iter;
            sql_query += format(
                " {} = {},",
                tx.quote_name(column),
                tx.quote(value)
            );
        }

        if (sql_query.ends_with(",")) {
            sql_query.erase(sql_query.end()-1);
        }

        sql_query += format(
            " WHERE {} = {} RETURNING *;",
            tx.quote_name(ID->name),
            tx.quote(entity.id)
        );

        #ifndef NDEBUG
        get_logger()->debug("exec_update_by_id::sql", sql_query, __FILE__, __LINE__);
        #endif

        auto res = tx.exec(sql_query);

        if (res.empty()) {
            throw runtime_error(format("exec_update::update_error -- row doesnt updated in table {}", EntityT::get_table_name()));
        }

        return make_unique<EntityT>(res.one_row());
    }

    inline result exec_select(connection& conn, const char* table, const map<string, string>& where, bool check_deleted = true) {
        nontransaction tx{conn};

        string sql_where;
        for (const auto&[column, value] : where) {
            sql_where += fmt::format(
                " {} = {} AND",
                tx.quote_name(column),
                tx.quote(value)
            );
        }

        if (sql_where.ends_with("AND")) {
            sql_where.erase(sql_where.end()-3, sql_where.end());
        }

        string sql_query = fmt::format(
            "SELECT {}.* FROM {} WHERE {}",
            tx.quote_name(table),
            tx.quote_name(table),
            sql_where
        );

        if (check_deleted) {
            sql_query += format(
                " AND {} IS NULL",
                tx.quote_name(DELETED_AT->name)
            );
        }

        sql_query += ";";

        #ifndef NDEBUG
        get_logger()->debug("exec_select::sql", sql_query, __FILE__, __LINE__);
        #endif

        return tx.exec(sql_query);
    }

    template<typename EntityT>
    unique_ptr<EntityT> exec_select_one(connection& conn, const char* table, const map<string, string>& where, bool check_deleted = true) {
        auto res = exec_select(conn, table, where, check_deleted);

        if (res.empty()) {
            return nullptr;
        }

        return make_unique<EntityT>(res.one_row());
    }

    template<typename EntityT>
    vector<unique_ptr<EntityT> > exec_select_many(connection& conn, const char* table, const map<string, string>& where, bool check_deleted = true) {
        auto res = exec_select(conn, table, where, check_deleted);

        vector<unique_ptr<EntityT> > vector_result;

        for (const auto& r : res) {
            vector_result.emplace_back(make_unique<EntityT>(r));
        }

        return vector_result;
    }

    template<typename EntityT>
    unique_ptr<EntityT> exec_delete(connection& conn, long long id, bool is_soft = true) {
        nontransaction tx{conn};
        string sql_query;
        if (is_soft) {
            sql_query = fmt::format(
                "UPDATE {} SET {} = {} WHERE {} = {} RETURNING *",
                tx.quote_name(EntityT::get_table_name()),
                tx.quote_name(DELETED_AT->name),
                tx.quote(datetime().to_string(DATETIME_FORMAT)),
                tx.quote_name(ID->name),
                tx.quote(id)
            );
        } else {
            sql_query = fmt::format(
                "DELETE FROM {} WHERE {} = {} RETURNING *",
                tx.quote_name(EntityT::get_table_name()),
                tx.quote_name(ID->name),
                tx.quote(id)
            );
        }

        #ifndef NDEBUG
        get_logger()->debug("exec_delete::sql", sql_query, __FILE__, __LINE__);
        #endif

        const auto res = tx.exec(sql_query);

        if (res.empty()) {
            throw runtime_error(format("exec_update::update_error -- row doesnt deleted in table {}", EntityT::get_table_name()));
        }

        
        return make_unique<EntityT>(res.one_row());
    }
}