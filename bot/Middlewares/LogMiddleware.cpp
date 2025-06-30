#pragma once

#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <Middlewares/AbstractMiddleware.cpp>
#include <utils/Random.cpp>
#include <utils/Types.cpp>

using Utils::Types::ConstRequest;
using Utils::Types::ConstResponse;
using Utils::Random::randint;
using std::cout;
using fmt::format;
using Middlewares::AbstractMiddleware;

namespace Middlewares {

struct LogMiddleware : public AbstractMiddleware {
    
    void generate_id(context& ctx) {
        ctx.id = randint(1, 1000000000);
    }

    void log_request(ConstRequest& req, context& ctx) {
        cout << format(
            "event={}; request_id={}; method={}; uri={}\n",
            "new-request",
            ctx.id,
            method_name(req.method),
            req.url
        );
    }

    void before(ConstRequest& req, ConstResponse& res, context& ctx) override {
        this->generate_id(ctx);
        this->log_request(req, ctx);
    };
};

}