#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/Entity/Exceptions/FailedUpdateException.hpp>
#include <utils/Entity/Exceptions/FailedInsertException.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <fmt/core.h>
#include <vector>

namespace Utils::Entity {

    using Exceptions::FailedUpdateException;
    using Exceptions::FailedInsertException;
    using Logger::get_logger;
    using std::pair;
    using std::make_unique;
    using std::vector;
    using fmt::format;
    using pqxx::result;
    using pqxx::row;
    using pqxx::nontransaction;

    template<typename EntityT>
    unique_ptr<EntityT> exec_insert(connection& conn, const char* table, const map<string, string>& data) {
        nontransaction tx{conn};

        string sql_columns;
        string sql_values;

        for (const auto&[fst, snd] : data) {
            sql_columns += format(
                " {},",
                fst
            );

            sql_values += format(
                " {},",
                tx.quote(snd)
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
            table,
            sql_columns,
            sql_values
        );

        #ifndef NDEBUG
        get_logger()->debug("exec_insert::sql_query", sql_query, __FILE__, __LINE__);
        #endif

        auto res = tx.exec(sql_query);

        if (res.empty()) {
            throw FailedInsertException("row wasnt inserted");
        }

        return make_unique<EntityT>(res.one_row());

    }

    template<typename EntityT>
    unique_ptr<EntityT> exec_update_by_id(connection& conn, const char* table, const map<string, string>& data, int id) {
        nontransaction tx{conn};

        string sql_query = format(
            "UPDATE {} SET",
            table
        );

        for (const auto& iter : data) {
            sql_query += format(
                " {} = {},",
                iter.first,
                tx.quote(iter.second)
            );
        }

        if (sql_query.ends_with(",")) {
            sql_query.erase(sql_query.end()-1);
        }

        sql_query += format(
            " WHERE {} = {} RETURNING *;",
            ID_COLUMN,
            tx.quote(id)
        );

        #ifndef NDEBUG
        get_logger()->debug("exec_update_by_id::sql", sql_query, __FILE__, __LINE__);
        #endif

        auto res = tx.exec(sql_query);

        if (res.empty()) {
            throw FailedUpdateException("row wasnt updated");
        }

        return make_unique<EntityT>(res.one_row());
    }

    inline result exec_select(connection& conn, const char* table, const map<string, string>& where, bool check_deleted = true) {
        nontransaction tx{conn};

        string sql_where;
        for (const auto& cond : where) {
            sql_where += format(
                " {} = {} AND",
                cond.first,
                tx.quote(cond.second)
            );
        }

        if (sql_where.ends_with("AND")) {
            sql_where.erase(sql_where.end()-3, sql_where.end());
        }

        string sql_query = format(
            "SELECT {}.* FROM {} WHERE {}",
            table,
            table,
            sql_where
        );

        if (check_deleted) {
            sql_query += format(
                " AND {} IS NULL",
                DELETED_AT_COLUMN
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

}