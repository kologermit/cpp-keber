#include <task_tracker/Entity/Task/TaskRepository.hpp>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace TaskTracker::Entity::Task {

    using std::to_string;
    using std::make_unique;
    using pqxx::nontransaction;
    using pqxx::result;
    using pqxx::const_result_iterator;
    using Utils::Entity::ID_COLUMN;
    using Utils::Entity::CREATED_AT_COLUMN;
    using Utils::Entity::DELETED_AT_COLUMN;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    struct ParamAndColumnAndSymbol {
        const char* column;
        bool is_gte;
        const optional<datetime>& param;
    };

    unique_ptr<vector<Task>> TaskRepository::get_by_user_id(const GetByUserParams& params) {
        nontransaction tx{_db};

        string optional_sql_where;
        
        for (const auto& [column, is_gte, param] : vector<ParamAndColumnAndSymbol>{
            {CREATED_AT_COLUMN, true, params.created_at_gte},
            {CREATED_AT_COLUMN, false, params.created_at_lte},
            {START_AT_COLUMN, true, params.start_at_gte},
            {START_AT_COLUMN, false, params.start_at_lte},
            {IN_WORK_AT_COLUMN, true, params.in_work_at_gte},
            {IN_WORK_AT_COLUMN, false, params.in_work_at_lte},
            {COMPLETED_AT_COLUMN, true, params.completed_at_gte},
            {COMPLETED_AT_COLUMN, false, params.completed_at_lte},
            {DELETED_AT_COLUMN, true, params.deleted_at_gte},
            {DELETED_AT_COLUMN, false, params.deleted_at_lte},
        }) {
            if (param.has_value()) {
                optional_sql_where += fmt::format(
                    "AND {} {} {} ",
                    tx.quote_name(column),
                    (is_gte ? ">=" : "<="),
                    tx.quote(param->to_string(DATETIME_FORMAT))
                );
            }
        }

        if (params.state.has_value()) {
            switch (params.state.value()) {
                case TaskState::NEW:
                    optional_sql_where += fmt::format(
                        " AND {} IS NULL AND {} IS NULL AND {} IS NULL",
                        tx.quote_name(IN_WORK_AT_COLUMN),
                        tx.quote_name(COMPLETED_AT_COLUMN),
                        tx.quote_name(DELETED_AT_COLUMN)
                    );
                    break;
                case TaskState::IN_WORK:
                    optional_sql_where += fmt::format(
                        " AND {} IS NOT NULL AND {} IS NULL AND {} IS NULL",
                        tx.quote_name(IN_WORK_AT_COLUMN),
                        tx.quote_name(COMPLETED_AT_COLUMN),
                        tx.quote_name(DELETED_AT_COLUMN)
                    );
                    break;
                case TaskState::COMPLETED:
                    optional_sql_where += fmt::format(
                        " AND {} IS NOT NULL AND {} IS NULL",
                        tx.quote_name(COMPLETED_AT_COLUMN),
                        tx.quote_name(DELETED_AT_COLUMN)
                    );
                    break;
                case TaskState::DELETED:
                    optional_sql_where += fmt::format(
                        " AND {} IS NOT NULL",
                        tx.quote_name(DELETED_AT_COLUMN)
                    );
                    break;
                default: break;
            }
        }

        const string sql_query = fmt::format(
            "SELECT * FROM {} WHERE {} = {} {} ORDER BY {} ASC",
            tx.quote_name(Task::get_table_name()),
            tx.quote_name(USER_ID_COLUMN),
            tx.quote(params.user_id),
            optional_sql_where,
            tx.quote_name(ID_COLUMN)
        );

        #ifndef NDEBUG
        get_logger()->debug("SQL_QUERY", sql_query, __FILE__, __LINE__);
        #endif

        const result res = tx.exec(sql_query);
        unique_ptr<vector<Task>> return_result = make_unique<vector<Task>>();

        for (const const_result_iterator& row : res) {
            return_result->emplace_back(row);
        }

        return return_result;
    }
}