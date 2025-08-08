#pragma once

#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <memory>
#include <vector>
#include <exception>

namespace Utils::Entity::ApiRequest {

using std::unique_ptr;
using std::vector;

struct InterfaceApiRequestRepository {

    virtual unique_ptr<ApiRequest> get_by_id(int id, bool check_deleted = true) = 0;
    virtual unique_ptr<ApiRequest> create(const ApiRequest& request) = 0;
    virtual unique_ptr<ApiRequest> update(const ApiRequest& request, const vector<int>& columns) = 0;
    virtual unique_ptr<ApiRequest> del(int id) = 0;

};

}