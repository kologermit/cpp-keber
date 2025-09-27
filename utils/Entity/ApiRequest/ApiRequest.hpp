#pragma once

#include <utility>
#include <utils/Entity/Entity.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <memory>
#include <map>

namespace Utils::Entity::ApiRequest {

    using std::string;
    using std::string_view;
    using std::to_string;
    using std::map;
    using nlohmann::json;

    enum EnumRequestService : int {
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

    const auto FROM = make_shared<Column>("from");
    const auto TO = make_shared<Column>("to");
    const auto REQUEST = make_shared<Column>("request");
    const auto RESPONSE = make_shared<Column>("response");

    struct ApiRequest : Entity {

        EnumRequestService from;
        EnumRequestService to;
        json request;
        json response;

        explicit ApiRequest(
            EnumRequestService from = UNKNOWN,
            EnumRequestService to = UNKNOWN,
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
            from(static_cast<EnumRequestService>(api_request_row[FROM->name].get<int>().value())),
            to(static_cast<EnumRequestService>(api_request_row[TO->name].get<int>().value())),
            request(json::parse(api_request_row[REQUEST->name].get<string>().value())),
            response(json::parse(api_request_row[RESPONSE->name].get<string>().value()))
        {}

        static const char* get_table_name() noexcept {
            static const char* table_name = "api_requests";
            return table_name;
        }

        map<const char *, optional<string> > to_map(bool is_full = false, bool add_id = false) const noexcept {
            auto result_map = Entity::to_map(is_full, add_id);

            result_map[FROM->name] = to_string(from);
            result_map[TO->name] = to_string(to);
            result_map[REQUEST->name] = request.dump();
            result_map[RESPONSE->name] = response.dump();

            return result_map;
        }
    };
}