#pragma once

#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <utils/Random.cpp>
#include <utils/Types.cpp>

using Utils::Types::ConstRequest;
using Utils::Types::ConstResponse;

namespace Middlewares {


struct AbstractMiddleware {
    struct context {
        int id;
    };

    virtual void before(ConstRequest& req, ConstResponse& res, context& ctx) {};
    virtual void after(ConstRequest& req, ConstResponse& res, context& ctx) {};

    template<typename AllContext> 
    void before_handle(ConstRequest& req, ConstResponse& res, context& ctx, AllContext& all_context) {
        this->before(req, res, ctx);
    };

    template<typename AllContext>
    void after_handle(ConstRequest& req, ConstResponse& res, context& ctx, AllContext& all_context) {
        this->after(req, res, ctx);
    };
};

}