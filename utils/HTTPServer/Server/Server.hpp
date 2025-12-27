#pragma once

#include <utils/HTTPServer/Server/InterfaceServer.hpp>
#include <utils/JSONKeys.hpp>
#include <utils/Random/Random.hpp>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <stdexcept>

namespace Utils::HTTPServer::Server {

    using std::string;
    using std::string_view;
    using std::to_string;
    using std::exception;
    using std::shared_ptr;
    using std::make_shared;
    using std::invalid_argument;
    using HTTPServer = httplib::Server;
    using HTTPHandler = httplib::Server::Handler;
    using httplib::Request;
    using httplib::Response;
    using nlohmann::json;
    using Utils::Random::rand_int;
    using Utils::JSONKeys::STATUS_KEY;
    using Utils::JSONKeys::RESULT_KEY;
    using Utils::JSONKeys::HANDLE_ID_KEY;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::HandlerSignature;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    template <typename GlobalContext, typename HandlerContext>
    struct Server : public InterfaceServer<GlobalContext, HandlerContext> {
        using ptrIHandler = shared_ptr<InterfaceHandler<HandlerContext> >;

        private:
        vector<ptrIHandler> _handlers;
        string _ip;
        int _port;
        HTTPServer _server;
        shared_ptr<GlobalContext> _global_context;

        public:
        Server(
                shared_ptr<GlobalContext> global_context,
                string_view ip,
                int port
            ) : 
            _ip(ip),
            _port(port),
            _global_context(global_context)
        {};
        
        void add_handler(ptrIHandler handler) noexcept override {
            _handlers.push_back(handler);
        };

        void add_handlers(const vector<ptrIHandler>& handlers) noexcept override {
            for (ptrIHandler handler : handlers) {
                _handlers.push_back(handler);
            }
        };

        void stop() override {
            _server.stop();
        };

        enum HandleResult {
            UNAUTHORIZED,
            INVALID_BODY,
        };

        void run() override {
            shared_ptr<GlobalContext> global_context = _global_context;
            for (ptrIHandler handler : _handlers) {
                HTTPHandler http_handler = [
                    handler,
                    global_context
                ](const Request& request, Response& response) {
                    int handle_id = rand_int(1, 1000000);
                    try {
                        response.status = 200;

                        const HandlerSignature& signature = handler->get_signature();
                        if (signature.is_auth) {
                            const auto authorize_header = request.headers.find("Authorization");
                            if (authorize_header == request.headers.end() || authorize_header->second != global_context->auth_key) {
                                response.status = 401;
                                response.set_content(
                                    json{
                                        {HANDLE_ID_KEY, handle_id},
                                        {STATUS_KEY, response.status}, 
                                        {RESULT_KEY, "unauthorized"}
                                    }.dump(), 
                                    "application/json"
                                );
                                throw HandleResult::UNAUTHORIZED;
                            }
                        }

                        if (signature.is_json_body && !json::accept(request.body)) {
                            response.status = 400;
                            response.set_content(
                                json{
                                    {HANDLE_ID_KEY, handle_id},
                                    {STATUS_KEY, response.status},
                                    {RESULT_KEY, "invalid_body"}
                                }.dump(),
                                "application/json"
                            );
                            throw HandleResult::INVALID_BODY;
                        }

                        json result = handler->handle(make_shared<HandlerContext>(
                            global_context, 
                            request,
                            response
                        ));

                        global_context->logger->info("SERVER::REQUEST", fmt::format(
                            "({}/{}) ({}) {} {}",
                            handler->get_signature().name,
                            request.method,
                            response.status,
                            handle_id,
                            request.path
                        ), __FILE__, __LINE__);
                        response.set_content(
                            json{
                                {HANDLE_ID_KEY, handle_id},
                                {STATUS_KEY, response.status}, 
                                {RESULT_KEY, result}
                            }.dump(), 
                            "application/json"
                        );
                    } catch (const HandleResult& handle_result) {
                        global_context->logger->info("SERVER::HANDLE", fmt::format("{} - {}",
                            handle_id,
                            (
                                handle_result == HandleResult::UNAUTHORIZED ? "UNAUTHORIZED"
                                : handle_result == HandleResult::INVALID_BODY ? "INVALID_BODY"
                                : "UNKNOWN"
                            )
                        ), __FILE__, __LINE__);
                    } catch (const invalid_argument& exc) {
                        response.status = 400;
                        response.set_content(
                            json{
                                {HANDLE_ID_KEY, handle_id},
                                {STATUS_KEY, response.status}, 
                                {RESULT_KEY, fmt::format("invalid_argument: {}", exc.what())}
                            }.dump(), 
                            "application/json"
                        );
                    } catch (const exception& exc) {
                        response.status = 500;
                        response.set_content(
                            json{
                                {HANDLE_ID_KEY, handle_id},
                                {STATUS_KEY, response.status}, 
                                {RESULT_KEY, fmt::format("unexcpected_exception: {}", exc.what())}
                            }.dump(), 
                            "application/json"
                        );
                        global_context->logger->error("SERVER::HANDLE", exc.what(), __FILE__, __LINE__);
                    }
                };

                const HandlerSignature& signature = handler->get_signature();
                if (signature.pattern == ".*" || signature.pattern == "*") {
                    _server.set_error_handler(http_handler);
                } else if (signature.method == RequestHandlerMethod::GET) {
                    _server.Get(signature.pattern, http_handler);
                } else if (signature.method == RequestHandlerMethod::POST) {
                    _server.Post(signature.pattern, http_handler);
                } else if (signature.method == RequestHandlerMethod::PATCH) {
                    _server.Patch(signature.pattern, http_handler);
                } else if (signature.method == RequestHandlerMethod::DELETE) {
                    _server.Delete(signature.pattern, http_handler);
                }
                global_context->logger->info(
                    "SERVER::SET_HANDLER", fmt::format(
                        "handler={} method={} parrent={} is_auth={}",
                        signature.name,
                        to_string(signature.method),
                        signature.pattern,
                        signature.is_auth
                    ), 
                    __FILE__, 
                    __LINE__
                );
            }
            global_context->logger->info(
                "SERVER::LISTEN", 
                fmt::format("{}:{}", _ip, _port), 
                __FILE__, 
                __LINE__
            );
            _server.listen(_ip, _port);
        }
    };
}