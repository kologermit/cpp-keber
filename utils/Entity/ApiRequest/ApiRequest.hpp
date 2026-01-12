#pragma once

#include <utility>
#include <utils/Entity/Entity.hpp>
#include <nlohmann/json.hpp>

namespace Utils::Entity::ApiRequest {

    using std::string;
    using std::string_view;
    using std::to_string;
    using std::map;
    using nlohmann::json;

    enum RequestService {
        UNKNOWN,
        NGINX,
        BOT,
        TASK,
        YOUTUBE,
        MERGER,
        CRON,
        PROMETHEUS,
    };

    constexpr const char* API_REQUEST_SERVICES_TABLE = "api_request_services";
    const map<int, string> map_enum_to_service_name{
        {UNKNOWN, "UNKNOWN"},
        {NGINX, "NGINX"},
        {BOT, "BOT"},
        {TASK, "TASK"},
        {YOUTUBE, "YOUTUBE"},
        {MERGER, "MERGER"},
        {CRON, "CRON"},
        {PROMETHEUS, "PROMETHEUS"},
    };

    constexpr const char* FROM_HEADER = "FROM_SERVICE";

    constexpr const char* FROM_COLUMN = "from";
    constexpr const char* TO_COLUMN = "to";
    constexpr const char* REQUEST_COLUMN = "request";
    constexpr const char* RESPONSE_COLUMN = "response";

    struct ApiRequest : Entity {

        RequestService from;
        RequestService to;
        json request;
        json response;

        explicit ApiRequest(
            RequestService from = UNKNOWN,
            RequestService to = UNKNOWN,
            json request = {},
            json response = {},
            long long id = 0,
            datetime created_at = {},
            datetime updated_at = {},
            optional<datetime> deleted_at = nullopt
        ):
            Entity(id, std::move(created_at), std::move(updated_at), std::move(deleted_at)),
            from(from),
            to(to),
            request(std::move(request)),
            response(std::move(response))
        {}

        explicit ApiRequest(const row& api_request_row):
            Entity(api_request_row),
            from(static_cast<RequestService>(api_request_row[FROM_COLUMN].get<int>().value())),
            to(static_cast<RequestService>(api_request_row[TO_COLUMN].get<int>().value())),
            request(json::parse(api_request_row[REQUEST_COLUMN].get<string>().value())),
            response(json::parse(api_request_row[RESPONSE_COLUMN].get<string>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table_name = "api_requests";
            return table_name;
        }

        map<const char *, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result_map = Entity::to_map(is_full, add_id);

            result_map[FROM_COLUMN] = to_string(from);
            result_map[TO_COLUMN] = to_string(to);
            result_map[REQUEST_COLUMN] = request.dump();
            result_map[RESPONSE_COLUMN] = response.dump();

            return result_map;
        }
    };
}