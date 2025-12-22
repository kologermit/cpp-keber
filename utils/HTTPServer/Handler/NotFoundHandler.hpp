#pragma once

#include <utils/HTTPServer/Handler/InterfaceHandler.hpp>

namespace Utils::HTTPServer::Handler {
    using std::unique_ptr;
    
    template <typename HandlerContext>
    struct NotFoundHandler : InterfaceHandler<HandlerContext> {
        json handle([[maybe_unused]] shared_ptr<HandlerContext> context) override {
            context->response.status = 404;
            return "not found";
        }
    };
    
    template <typename HandlerContext>
    struct NotFoundHandlerGET : NotFoundHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override {
            static const HandlerSignature signature{
                .name = "NotFoundHandlerGET",
                .pattern = ".*",
                .method = RequestHandlerMethod::GET,
                .is_auth = false,
            };
            return signature;
        }
    };

    template <typename HandlerContext>
    struct NotFoundHandlerPOST : NotFoundHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override {
            static const HandlerSignature signature{
                .name = "NotFoundHandlerPOST",
                .pattern = ".*",
                .method = RequestHandlerMethod::POST,
                .is_auth = false,
            };
            return signature;
        }
    };

    template <typename HandlerContext>
    struct NotFoundHandlerPATCH : NotFoundHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override {
            static const HandlerSignature signature{
                .name = "NotFoundHandlerPATCH",
                .pattern = ".*",
                .method = RequestHandlerMethod::PATCH,
                .is_auth = false,
            };
            return signature;
        }
    };

    template <typename HandlerContext>
    struct NotFoundHandlerDELETE : NotFoundHandler<HandlerContext> {
        const HandlerSignature& get_signature() const noexcept override {
            static const HandlerSignature signature{
                .name = "NotFoundHandlerDELETE",
                .pattern = ".*",
                .method = RequestHandlerMethod::DELETE,
                .is_auth = false,
            };
            return signature;
        }
    };
}