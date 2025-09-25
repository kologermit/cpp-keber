#pragma once

#include "Entity.hpp"
#include "exec_templates.hpp"

namespace pqxx {
    class connection;
}

namespace Utils::Entity {

    using pqxx::connection;
    using std::unique_ptr;

    template<typename EntityT>
    struct Repository {
        virtual ~Repository() = default;

        virtual unique_ptr<EntityT> get_by_id(int id, bool check_deleted);
        virtual unique_ptr<EntityT> create(const EntityT& entity);
        virtual unique_ptr<EntityT> update(const EntityT& entity);
        virtual unique_ptr<EntityT> del(int id, bool is_soft = true);

        explicit Repository(connection& _db);

    protected:

        connection& _db;
    };

    template<typename EntityT>
    Repository<EntityT>::Repository(connection& _db): _db(_db) {}

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::create(const EntityT& entity) {
        return exec_insert<EntityT>(
            _db,
            entity
        );
    }

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::get_by_id(int id, bool check_deleted) {
        return exec_select_one<EntityT>(
            _db,
            EntityT::get_table_name(),
            {{ID->name, to_string(id)}},
            check_deleted
        );
    }

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::update(const EntityT &entity) {
        return exec_update_by_id<EntityT>(
            _db,
            entity
        );
    }

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::del(int id, bool is_soft) {
        return Utils::Entity::exec_delete<EntityT>(_db, id, is_soft);
    }


}