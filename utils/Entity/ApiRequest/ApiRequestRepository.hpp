#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <pqxx/pqxx>

namespace Utils::Entity::ApiRequest {

using pqxx::connection;

struct ApiRequestRepository : InterfaceApiRequestRepository {

    unique_ptr<ApiRequest> get_by_id(int id, bool check_deleted) override;
    unique_ptr<ApiRequest> create(const ApiRequest& request) override;
    unique_ptr<ApiRequest> update(const ApiRequest& request, const vector<int>& columns) override;
    unique_ptr<ApiRequest> del(int id) override;

    ApiRequestRepository(connection& _db);

    private:

    connection& _db;

};

}