#pragma once

#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <Middlewares/InterfaceMiddleware.cpp>
#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Random/Random.cpp>
#include <utils/Type.cpp>

namespace Middlewares {

using namespace Utils::Type;
using namespace Utils::Logger;
using namespace Utils::Random;
using namespace fmt;

struct LogMiddleware : public InterfaceMiddleware {
    
    void generate_id(context& ctx) {
        ctx.id = rand_int(1, 1000000000);
    }

    void log_request(const_request& req, context& ctx) {
        GLOBAL_LOGGER->info("NEW REQUEST", format(
            "event={}; request_id={}; method={}; uri={}\n",
            "new-request",
            ctx.id,
            method_name(req.method),
            req.url
        ));
    }

    void before(const_request& req, const_response& res, context& ctx) override {
        this->generate_id(ctx);
        this->log_request(req, ctx);
    };
};

}