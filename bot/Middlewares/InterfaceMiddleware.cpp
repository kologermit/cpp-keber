#pragma once

#include <crow.h>
#include <fmt/core.h>
#include <iostream>
#include <utils/Types.hpp>

namespace Middlewares {

using namespace Utils::Types;

struct InterfaceMiddleware {
    struct context { int id; };

    virtual void before(const_request& req, const_response& res, context& ctx) {};
    virtual void after(const_request& req, const_response& res, context& ctx) {};

    template<typename AllContext> 
    void before_handle(const_request& req, const_response& res, context& ctx, AllContext& all_context) {
        ->before(req, res, ctx);
    };

    template<typename AllContext>
    void after_handle(const_request& req, const_response& res, context& ctx, AllContext& all_context) {
        ->after(req, res, ctx);
    };
};

}
