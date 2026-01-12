#pragma once

#include <utils/Entity/Entity.hpp>
#include <utils/Entity/exec_templates.hpp>

namespace Utils::Entity {

    using pqxx::connection;
    using std::unique_ptr;

    template<typename EntityT>
    struct Repository {
        virtual ~Repository() = default;

        virtual unique_ptr<EntityT> get_by_id(long long id, bool check_deleted = true);
        virtual unique_ptr<EntityT> create(const EntityT& entity, bool add_id = false);
        virtual unique_ptr<EntityT> update(const EntityT& entity);
        virtual unique_ptr<EntityT> del(long long id, bool is_soft = true);

        explicit Repository(connection& _db);

    protected:

        connection& _db;
    };

    template<typename EntityT>
    Repository<EntityT>::Repository(connection& _db): _db(_db) {}

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::create(const EntityT& entity, bool add_id) {
        return exec_insert<EntityT>(
            _db,
            entity,
            add_id
        );
    }

    template<typename EntityT>
    unique_ptr<EntityT> Repository<EntityT>::get_by_id(long long id, bool check_deleted) {
        return exec_select_one<EntityT>(
            _db,
            EntityT::get_table_name(),
            {{ID_COLUMN, to_string(id)}},
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
    unique_ptr<EntityT> Repository<EntityT>::del(long long id, bool is_soft) {
        return Utils::Entity::exec_delete<EntityT>(_db, id, is_soft);
    }


}