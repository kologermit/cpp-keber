#pragma once

#include <utils/HTTPServer/Server/InterfaceServer.hpp>
#include <utils/JSONKeys.hpp>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <stdexcept>

namespace Utils::HTTPServer::Server {

    using std::string;
    using std::string_view;
    using std::exception;
    using std::shared_ptr;
    using std::make_shared;
    using std::invalid_argument;
    using HTTPServer = httplib::Server;
    using HTTPHandler = httplib::Server::Handler;
    using httplib::Request;
    using httplib::Response;
    using nlohmann::json;
    using Utils::JSONKeys::STATUS_KEY;
    using Utils::JSONKeys::RESULT_KEY;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    template <typename GlobalContext, typename HandlerContext>
    struct Server : InterfaceServer<GlobalContext, HandlerContext> {
        using ptrIHandler = shared_ptr<InterfaceHandler<HandlerContext> >;

        Server(
            shared_ptr<GlobalContext> global_context,
            string_view ip,
            int port
        );
        void add_handler(ptrIHandler handler) noexcept override;
        void add_handlers(vector<ptrIHandler> handlers) noexcept override;
        void run() override;
        void stop() override;

        private:
        vector<ptrIHandler> _handlers;
        string _ip;
        int _port;

        public:
        HTTPServer _server;
        shared_ptr<GlobalContext> _global_context;
    };

    template <typename GlobalContext, typename HandlerContext>
    Server<GlobalContext, HandlerContext>::Server(
            shared_ptr<GlobalContext> global_context,
            string_view ip,
            int port
        ) : 
        _ip(ip),
        _port(port),
        _global_context(global_context)
    {}

    template <typename GlobalContext, typename HandlerContext>
    void Server<GlobalContext, HandlerContext>::add_handler(ptrIHandler handler) noexcept {
        _handlers.push_back(handler);
    }

    template <typename GlobalContext, typename HandlerContext>
    void Server<GlobalContext, HandlerContext>::add_handlers(vector<ptrIHandler> handlers) noexcept {
        for (const auto& handler : handlers) {
            _handlers.push_back(handler);
        }
    }

    template <typename GlobalContext, typename HandlerContext>
    void Server<GlobalContext, HandlerContext>::stop() {
        _server.stop();
    }

    enum HandleResult {
        ANAUTHORIZED
    };

    template <typename GlobalContext, typename HandlerContext>
    void Server<GlobalContext, HandlerContext>::run() {
        shared_ptr<GlobalContext> global_context = _global_context;
        for (ptrIHandler handler : _handlers) {
            HTTPHandler http_handler = [
                handler,
                global_context
            ](const Request& request, Response& response) {
                try {
                    response.status = 200;
                    shared_ptr<HandlerContext> context = make_shared<HandlerContext>(
                        global_context, 
                        request,
                        response
                    );
                    if (handler->get_signature().is_auth) {
                        const auto authorize_header = request.headers.find("Authorize");
                        if (authorize_header == request.headers.end() || authorize_header->second != global_context->auth_key) {
                            response.status = 401;
                                    response.set_content(
                                json{
                                    {STATUS_KEY, response.status}, 
                                    {RESULT_KEY, "anauthorized"}
                                }.dump(), 
                                "application/json"
                            );
                            throw HandleResult::ANAUTHORIZED;
                        }
                    }
                    json result = handler->handle(context);

                    global_context->logger->info("SERVER::REQUEST", fmt::format(
                        "({}/{}) {} {}",
                        handler->get_signature().name,
                        request.method,
                        response.status,
                        request.path
                    ), __FILE__, __LINE__);
                    response.set_content(
                        json{
                            {STATUS_KEY, response.status}, 
                            {RESULT_KEY, result}
                        }.dump(), 
                        "application/json"
                    );
                } catch (const HandleResult&) {
                    global_context->logger->info("SERVER::HANDLE::AUNUTHORIZED", "", __FILE__, __LINE__);
                } catch (const invalid_argument& exc) {
                    response.status = 400;
                    response.set_content(
                        json{
                            {STATUS_KEY, response.status}, 
                            {RESULT_KEY, fmt::format("invalid_argument: {}", exc.what())}
                        }.dump(), 
                        "application/json"
                    );
                } catch (const exception& exc) {
                    response.status = 500;
                    response.set_content(
                        json{
                            {STATUS_KEY, response.status}, 
                            {RESULT_KEY, fmt::format("enexcpected_exception: {}", exc.what())}
                        }.dump(), 
                        "application/json"
                    );
                    global_context->logger->error("SERVER::HANDLE", exc.what(), __FILE__, __LINE__);
                }
            };

            const auto& signature = handler->get_signature();
            if (signature.method == RequestHandlerMethod::GET) {
                _server.Get(signature.pattern, http_handler);
            } else if (signature.method == RequestHandlerMethod::POST) {
                _server.Post(signature.pattern, http_handler);
            } else if (signature.method == RequestHandlerMethod::PATCH) {
                _server.Patch(signature.pattern, http_handler);
            } else if (signature.method == RequestHandlerMethod::DELETE) {
                _server.Delete(signature.pattern, http_handler);
            }
            _server.Get(signature.pattern, http_handler);

            _server.listen(_ip, _port);
        }
    }
}