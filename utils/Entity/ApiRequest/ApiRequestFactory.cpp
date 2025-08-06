#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <utils/Entity/ApiRequest/ApiRequestFactory.hpp>
#include <utils/Entity/Exceptions/FailedInsertException.hpp>
#include <utils/Entity/Exceptions/FailedUpdateException.hpp>
#include <utils/Entity/Exceptions/FailedDeleteException.hpp>
#include <fmt/core.h>
#include <map>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Entity::ApiRequest {

using pqxx::nontransaction;
using fmt::format;
using std::make_unique;
using Utils::Entity::Exceptions::FailedInsertException;
using Utils::Entity::Exceptions::FailedUpdateException;
using Utils::Entity::Exceptions::FailedDeleteException;
using std::map;
using std::move;
using std::to_string;

#ifndef NDEBUG
using Utils::Logger::get_logger;
#endif


ApiRequestFactory::ApiRequestFactory(string_view conn_str):
_db(conn_str.data()) 
{}

unique_ptr<ApiRequest> ApiRequestFactory::get_by_id(int id, bool check_deleted) const {
    nontransaction tx{_db};

    const string sql_query = format(
        "SELECT * FROM {} WHERE {} = {}",
        ApiRequest::TABLE_NAME(),
        ID_COLUMN,
        tx.quote(id)
    );

    #ifndef NDEBUG
    get_logger()->debug("get_by_id::sql_query", sql_query, __FILE__, __LINE__);
    #endif

    for (const auto& row : tx.exec(sql_query)) {
        if (check_deleted && !row[DELETED_AT_COLUMN].get<string>().has_value()) {
            return nullptr;
        }
        return make_unique<ApiRequest>(row);
    }

    return nullptr;
}

unique_ptr<ApiRequest> ApiRequestFactory::create(const ApiRequest& request) const {
    nontransaction tx{_db};

    const string sql_query = format(
        "INSERT INTO {} ({}, {}, {}, {}) VALUES ({}, {}, {}, {}) RETURNING *",
        ApiRequest::TABLE_NAME(),
        FROM_COLUMN,
        TO_COLUMN,
        REQUEST_COLUMN,
        RESPONSE_COLUMN,
        tx.quote(static_cast<int>(request.from)),
        tx.quote(static_cast<int>(request.to)),
        tx.quote(request.request.dump()),
        tx.quote(request.response.dump())
    );

    #ifndef NDEBUG
    get_logger()->debug("create::sql_query", sql_query, __FILE__, __LINE__);
    #endif

    for (const auto& row : tx.exec(sql_query)) {
        return make_unique<ApiRequest>(row);
    }

    throw FailedInsertException("api request wasnt returned");
}

const string& throw_if_invalid_column_to_update(int column) {
    static const map<int, const string> map_column_to_name{
        {ApiRequestColumns::FROM, FROM_COLUMN},
        {ApiRequestColumns::TO, TO_COLUMN},
        {ApiRequestColumns::REQUEST, REQUEST_COLUMN},
        {ApiRequestColumns::RESPONSE, RESPONSE_COLUMN}
    };

    auto find_iterator = map_column_to_name.find(column);

    if (find_iterator == map_column_to_name.end()) {
        throw FailedUpdateException("column cant be updated");
    }

    return find_iterator->second;
}


void ApiRequestFactory::update(const ApiRequest& request, int column) const {
    nontransaction tx{_db};

    string sql_query = format(
        "UPDATE {} SET {} = {} WHERE {} = {}",
        ApiRequest::TABLE_NAME(),
        throw_if_invalid_column_to_update(column),
        tx.quote(request.get_data_by_column(column)),
        ID_COLUMN,
        tx.quote(request.id)
    );

    #ifndef NDEBUG
    get_logger()->debug("update::sql_query", sql_query, __FILE__, __LINE__);
    #endif

    if (tx.exec(sql_query).affected_rows() <= 0) {
        throw FailedUpdateException("rows doesnt updated");
    }
}

void ApiRequestFactory::update(const ApiRequest& request, vector<int> columns) const {
    if (columns.empty()) {
        throw FailedUpdateException("columns is empty");
    }

    nontransaction tx{_db};

    string sql_query = string("UPDATE ") + ApiRequest::TABLE_NAME() + "SET ";

    for (int column : columns) {
        sql_query += throw_if_invalid_column_to_update(column) + " = " + tx.quote(request.get_data_by_column(column)) + ",";
    }

    if (sql_query.ends_with(",")) {
        sql_query.erase(sql_query.end() - 1);
    }

    sql_query += string(" WHERE ") + ID_COLUMN + " = " + tx.quote(request.id);

    #ifndef NDEBUG
    get_logger()->debug("update::sql_query", sql_query, __FILE__, __LINE__);
    #endif

    if (tx.exec(sql_query).affected_rows() <= 0) {
        throw FailedUpdateException("rows doesnt updated");
    }
}

void ApiRequestFactory::del(int id) const {
    nontransaction tx{_db};

    const string sql_query = format(
        "DELETE FROM {} WHERE {} = {}",
        ApiRequest::TABLE_NAME(),
        ID_COLUMN,
        tx.quote(id)
    );

    #ifndef NDEBUG
    get_logger()->debug("del", sql_query, __FILE__, __LINE__);
    #endif

    if (tx.exec(sql_query).affected_rows() <= 0) {
        throw FailedDeleteException("rows doesnt deleted");
    }
}

}