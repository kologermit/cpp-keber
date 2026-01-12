#pragma once

#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <utils/Entity/Repository.hpp>

namespace Utils::Entity::ApiRequest {

    using std::unique_ptr;
    using std::vector;

    struct InterfaceApiRequestRepository : Repository<ApiRequest> {
        explicit InterfaceApiRequestRepository(connection& db): Repository(db) {}
    };

}