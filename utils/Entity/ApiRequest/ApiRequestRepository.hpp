#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <utils/Entity/Repository.hpp>
#include <pqxx/pqxx>

namespace Utils::Entity::ApiRequest {

    using pqxx::connection;

    struct ApiRequestRepository final : InterfaceApiRequestRepository {
        unique_ptr<ApiRequest> get_by_id(int id, bool check_deleted) override;
        unique_ptr<ApiRequest> create(const ApiRequest& request) override;
        unique_ptr<ApiRequest> update(const ApiRequest& request) override;
        unique_ptr<ApiRequest> del(int id, bool is_soft) override;
        unique_ptr<ApiRequest> del(int id) override;

        explicit ApiRequestRepository(connection& db);

        private:

        Repository<ApiRequest> _repository;
    };

}