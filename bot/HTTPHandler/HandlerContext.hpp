#pragma once

#include <bot/GlobalContext.hpp>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <optional>
#include <map>

namespace Bot::HTTPHandler {

    using std::map;
    using std::optional;
    using std::nullopt;
    using nlohmann::json;
    using httplib::Request;
    using httplib::Response;

    struct HandlerContext {
        const Request& request;
        Response& response;
        shared_ptr<GlobalContext> global_context;
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        optional<json> json_body;
        map<string, long long> ll_params;
        map<string, double> double_params;
        map<string, string> string_params;
        map<string, bool> bool_params;

        HandlerContext(
            shared_ptr<GlobalContext> global_context,
            const Request& request,
            Response& response
        ):
            request(request),
            response(response),
            global_context(global_context),
            logger(global_context->logger),
            config(global_context->config),
            json_body(
                json::accept(request.body)
                ? optional<json>(json::parse(request.body))
                : nullopt
            )
        {}
    };
}