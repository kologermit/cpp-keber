#pragma once

#include <utils/Entity/ApiRequest/ApiRequest.hpp>
#include <memory>
#include <vector>
#include <exception>

namespace Utils::Entity::ApiRequest {

using std::unique_ptr;
using std::vector;

struct InterfaceApiRequestFactory {

    virtual unique_ptr<ApiRequest> get_by_id(int id, bool check_deleted = true) const = 0;
    virtual unique_ptr<ApiRequest> create(const ApiRequest& request) const = 0;
    virtual void update(const ApiRequest& request, int column) const = 0;
    virtual void update(const ApiRequest& request, vector<int> columns) const = 0;
    virtual void del(int id) const = 0;

};

}