#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>

namespace Utils::HTTPServer::Handler {
    using std::unique_ptr;
    
    template <typename HandlerContext>
    struct NotFoundHandler : InterfaceHandler<HandlerContext> {
        json handle(shared_ptr<HandlerContext> context) override {
            context->response.status = 404;
            return "not found";
        }
        
        const HandlerSignature& get_signature() const noexcept override {
            static const HandlerSignature signature{
                .name = "NotFoundHandler",
                .pattern = "*",
                .method = RequestHandlerMethod::NONE,
                .is_auth = false,
            };
            return signature;
        }
    };
}