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
    using Utils::Entity::DELETED_AT_COLUMN;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    unique_ptr<vector<Task>> TaskRepository::get_by_user_id(
            long long user_id,
            optional<datetime> start_at,
            optional<datetime> in_work_at,
            optional<datetime> completed_at,
            optional<datetime> deleted_at,
            optional<TaskState> state
        ) {
        nontransaction tx{_db};

        string optional_sql_where;

        if (start_at.has_value()) {
            optional_sql_where += fmt::format(
                " AND {} >= {}",
                tx.quote_name(START_AT_COLUMN),
                tx.quote(start_at->to_string(DATETIME_FORMAT))
            );
        }

        if (in_work_at.has_value()) {
            optional_sql_where += fmt::format(
                " AND {} >= {}",
                tx.quote_name(IN_WORK_AT_COLUMN),
                tx.quote(in_work_at->to_string(DATETIME_FORMAT))
            );
        }

        if (completed_at.has_value()) {
            optional_sql_where += fmt::format(
                " AND {} >= {}",
                tx.quote_name(COMPLETED_AT_COLUMN),
                tx.quote(completed_at->to_string(DATETIME_FORMAT))
            );
        }

        if (deleted_at.has_value()) {
            optional_sql_where += fmt::format(
                " AND {} >= {}",
                tx.quote_name(DELETED_AT_COLUMN),
                tx.quote(in_work_at->to_string(DATETIME_FORMAT))
            );
        }

        if (state.has_value()) {
            switch (state.value()) {
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
            "SELECT * FROM {} WHERE {} = {} {}",
            tx.quote_name(Task::get_table_name()),
            tx.quote_name(USER_ID_COLUMN),
            tx.quote(user_id),
            optional_sql_where
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