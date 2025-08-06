#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestFactory.hpp>
#include <pqxx/pqxx>

namespace Utils::Entity::ApiRequest {

using pqxx::connection;

struct ApiRequestFactory : InterfaceApiRequestFactory {

    unique_ptr<ApiRequest> get_by_id(int id, bool check_deleted) const override;
    unique_ptr<ApiRequest> create(const ApiRequest& request) const override;
    void update(const ApiRequest& request, int column) const override;
    void update(const ApiRequest& request, vector<int> columns) const override;
    void del(int id) const override;

    ApiRequestFactory(string_view conn_url);

    private:

    mutable connection _db;

};

}