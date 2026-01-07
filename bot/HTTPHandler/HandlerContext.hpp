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
        shared_ptr<GlobalContext> global_ctx;
        shared_ptr<DBContext> db;
        shared_ptr<InterfaceLogger> logger;
        shared_ptr<InterfaceConfig> config;
        optional<json> json_body;
        map<string, long long> ll_params;
        map<string, double> double_params;
        map<string, string> string_params;
        map<string, bool> bool_params;

        HandlerContext(
            shared_ptr<GlobalContext> global_ctx,
            const Request& request,
            Response& response
        ):
            request(request),
            response(response),
            global_ctx(global_ctx),
            db(global_ctx->db),
            logger(global_ctx->logger),
            config(global_ctx->config),
            json_body(
                json::accept(request.body)
                ? optional<json>(json::parse(request.body))
                : nullopt
            )
        {}
    };
}