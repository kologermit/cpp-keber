#pragma once

#include <utils/Entity/ApiRequest/InterfaceApiRequestRepository.hpp>
#include <utils/Entity/Repository.hpp>
#include <pqxx/pqxx>

namespace Utils::Entity::ApiRequest {

    using pqxx::connection;

    struct ApiRequestRepository final : InterfaceApiRequestRepository {

        explicit ApiRequestRepository(connection& db): InterfaceApiRequestRepository(db) {
            create_rows_in_enum_table_if_empty(db, API_REQUEST_SERVICES_TABLE, map_enum_to_service_name);
        };

        private:
    };

}