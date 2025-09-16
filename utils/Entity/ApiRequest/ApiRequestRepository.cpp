#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Entity::ApiRequest {

    #ifndef NDEBUG
    using Logger::get_logger;
    #endif


    ApiRequestRepository::ApiRequestRepository(connection& db) : _repository(db) {
        create_rows_in_enum_table_if_empty(db, API_REQUEST_SERVICES_TABLE, map_enum_to_service_name);
    }

    unique_ptr<ApiRequest> ApiRequestRepository::get_by_id(int id, bool check_deleted) {
        return _repository.get_by_id(id, check_deleted);
    }

    unique_ptr<ApiRequest> ApiRequestRepository::create(const ApiRequest& request) {
        return _repository.create(request);
    }

    unique_ptr<ApiRequest> ApiRequestRepository::update(const ApiRequest& request) {
        return _repository.update(request);
    }

    unique_ptr<ApiRequest> ApiRequestRepository::del(int id) {
        return _repository.del(id);
    }

}