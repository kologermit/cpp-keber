#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>
#include <utils/Entity/Exceptions/FailedInsertException.hpp>
#include <utils/Entity/Exceptions/FailedUpdateException.hpp>
#include <utils/Entity/Exceptions/FailedDeleteException.hpp>
#include <fmt/core.h>
#include <utils/Entity/exec_templates.hpp>
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


ApiRequestRepository::ApiRequestRepository(string_view conn_str):
_db(conn_str.data()) 
{
    create_rows_in_enum_table_if_empty(_db, API_REQUEST_SERVICES_TABLE, map_enum_to_service_name);
}

unique_ptr<ApiRequest> ApiRequestRepository::get_by_id(int id, bool check_deleted) {
    return exec_select<ApiRequest>(_db, API_REQUESTS_TABLE, {
        {ID_COLUMN, to_string(id)}
    }, check_deleted);
}

unique_ptr<ApiRequest> ApiRequestRepository::create(const ApiRequest& request) {
    return exec_insert<ApiRequest>(_db, API_REQUESTS_TABLE, {
        {FROM_COLUMN, to_string(request.from)},
        {TO_COLUMN, to_string(request.to)},
        {REQUEST_COLUMN, request.request.dump()},
        {RESPONSE_COLUMN, request.response.dump()}
    });
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

unique_ptr<ApiRequest> ApiRequestRepository::update(const ApiRequest& request, const vector<int>& columns) {
    map<string, string> data;
    for (int column : columns) {
        data[throw_if_invalid_column_to_update(column)] = request.get_data_by_column(column);
    }
    
    return exec_update_by_id<ApiRequest>(_db, API_REQUESTS_TABLE, data, request.id);
}

unique_ptr<ApiRequest> ApiRequestRepository::del(int id) {

    return exec_update_by_id<ApiRequest>(_db, API_REQUESTS_TABLE, {{DELETED_AT_COLUMN, datetime().to_string(DATETIME_FORMAT)}}, id);

}

}