#pragma once

#include <bot/Entity/Access/Access.hpp>

namespace Bot::Entity::Access {

using std::unique_ptr;
using std::vector;

struct InterfaceAccessRepository {
    virtual ~InterfaceAccessRepository() = default;

    virtual vector<unique_ptr<Access> > get_raw_by_user_id(int user_id) = 0;
    virtual UserAccess get_by_user_id(int user_id) = 0;
    virtual unique_ptr<Access> create(const Access& access) = 0;
    virtual unique_ptr<Access> update(const Access& access, vector<int> columns) = 0;
    virtual unique_ptr<Access> del(int id) = 0;

};

}