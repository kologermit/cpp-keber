#pragma once

#include <utils/Entity/Entity.hpp>
#include <nlohmann/json.hpp>
#include <string>
#include <string_view>
#include <memory>

namespace Utils::Entity::ApiRequest {

using std::string;
using std::string_view;
using std::move;
using std::to_string;
using nlohmann::json;

enum EnumRequestService {
    UNKNOWN,
    NGINX,
    BOT,
    TASK,
    YT,
    MERGER,
    CRON,
    PROMETHEUS,
};

enum ApiRequestColumns {
    FROM,
    TO,
    REQUEST,
    RESPONSE,
};

constexpr const char* FROM_COLUMN = "\"from\"";
constexpr const char* TO_COLUMN = "\"to\"";
constexpr const char* REQUEST_COLUMN = "\"request\"";
constexpr const char* RESPONSE_COLUMN = "\"response\"";

struct ApiRequest : Entity {

    static const char* TABLE_NAME() {
        static const char* table_name = "\"api_requests\"";
        return table_name;
    }

    EnumRequestService from;
    EnumRequestService to;
    json request;
    json response;

    ApiRequest(
        int id = 0,
        optional<datetime> created_at = nullopt,
        optional<datetime> updated_at = nullopt,
        optional<datetime> deleted_at = nullopt,
        EnumRequestService from = EnumRequestService::UNKNOWN,
        EnumRequestService to = EnumRequestService::UNKNOWN,
        json request = json(),
        json response = json()
    ):
        Entity(id, created_at, updated_at, deleted_at),
        from(from),
        to(to),
        request(request),
        response(response)
    {}

    ApiRequest(const row& api_request_row):
        Entity(api_request_row),
        from(static_cast<EnumRequestService>(api_request_row[FROM_COLUMN].get<int>().value())),
        to(static_cast<EnumRequestService>(api_request_row[TO_COLUMN].get<int>().value())),
        request(json::parse(api_request_row[REQUEST_COLUMN].get<string>().value())),
        response(json::parse(api_request_row[RESPONSE_COLUMN].get<string>().value()))
    {}

    string get_data_by_column(int column) const {
        switch (column)
        {
        case ApiRequestColumns::FROM:
            return to_string(from);
            break;

        case ApiRequestColumns::TO:
            return to_string(to);
            break; 

        case ApiRequestColumns::REQUEST:
            return request.dump();
            break;

        case ApiRequestColumns::RESPONSE:
            return response.dump();
            break;
        
        default:
            return "";
            break;
        }
    }

    
};

}