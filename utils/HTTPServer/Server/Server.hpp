#pragma once

#include <utils/HTTPServer/Server/InterfaceServer.hpp>
#include <utils/String/String.hpp>
#include <utils/JSONKeys.hpp>
#include <utils/Random/Random.hpp>
#include <nlohmann/json.hpp>
#include <fmt/core.h>
#include <string>
#include <string_view>
#include <stdexcept>

namespace Utils::HTTPServer::Server {

    using std::stoll;
    using std::stod;
    using std::string;
    using std::string_view;
    using std::to_string;
    using std::exception;
    using std::shared_ptr;
    using std::make_shared;
    using std::invalid_argument;
    using std::out_of_range;
    using std::runtime_error;
    using HTTPServer = httplib::Server;
    using HTTPHandler = httplib::Server::Handler;
    using httplib::Request;
    using httplib::Response;
    using nlohmann::json;
    using Utils::Random::rand_int;
    using Utils::JSONKeys::STATUS_KEY;
    using Utils::JSONKeys::RESULT_KEY;
    using Utils::JSONKeys::HANDLE_ID_KEY;
    using Utils::String::split;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::HTTPServer::Handler::HandlerSignature;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    template <typename GlobalContext, typename HandlerContext>
    struct Server final : InterfaceServer<GlobalContext, HandlerContext> {
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

        void run() override {
            shared_ptr<GlobalContext> global_context = _global_context;
            for (ptrIHandler handler : _handlers) {
                const HTTPHandler http_handler =
                [handler, global_context](const Request& request, Response& response) {
                    int handle_id = rand_int(1, 1000000);
                    #ifndef NDEBUG
                    global_context->logger->debug("SERVER::REQUEST", request.body, __FILE__, __LINE__);
                    #endif
                    json handle_result;
                    shared_ptr<HandlerContext> handler_context = make_shared<HandlerContext>(
                        global_context,
                        request,
                        response
                    );
                    try {
                        response.status = 200;

                        const HandlerSignature& signature = handler->get_signature();
                        if (signature.is_auth) {
                            if (const auto authorize_header = request.headers.find("Authorization");
                                authorize_header == request.headers.end() || authorize_header->second != global_context->auth_key) {
                                throw string("UNAUTHORIZED");
                            }
                        }

                        if (!signature.path_params.empty()) {
                            for (const auto&[name, type, is_required] : signature.path_params) {
                                if (!is_required && !request.path_params.contains(name)) {
                                    continue;
                                }
                                const string& value = request.path_params.at(name);
                                if (type == ParamType::STRING) {
                                        handler_context->string_params[name] = value;
                                } else if (type == ParamType::INT) {
                                    try {
                                        handler_context->ll_params[name] = stoll(value);
                                    } catch (const exception& exc) {
                                        throw invalid_argument(fmt::format("invalid path. param {} must be integer", name));
                                    }
                                } else if (type == ParamType::FLOAT) {
                                    try {
                                        handler_context->double_params[name] = stod(value);
                                    } catch (const exception& exc) {
                                        throw invalid_argument(fmt::format("invalid path. param {} must be float", name));
                                    }
                                } else if (type == ParamType::BOOL) {
                                    if (value != "true" && value != "false") {
                                        throw invalid_argument(fmt::format("invalid path. param {} must be boolean", name));
                                    }
                                    handler_context->bool_params[name] = value == "true";
                                }
                            }
                        }

                        if (signature.is_json_body && !json::accept(request.body)) {
                            throw invalid_argument("body must be json");
                        }

                        if (signature.is_json_body && !signature.body_params.empty()) {
                            json body_json = json::parse(request.body);
                            if (!body_json.is_object()) {
                                throw invalid_argument("body must be object");                            
                            }
                            for (const auto&[name, type, is_required] : signature.body_params) {
                                if (!body_json.contains(name) && is_required) {
                                    throw invalid_argument(fmt::format("body must have key {}", name));
                                }
                                if (!body_json.contains(name)) {
                                    continue;
                                }
                                if (json body_json_param = body_json[name];
                                    (type == ParamType::INT && !body_json_param.is_number_integer())
                                    || (type == ParamType::FLOAT && !body_json_param.is_number_float())
                                    || (type == ParamType::BOOL && !body_json_param.is_boolean())
                                    || (type == ParamType::STRING && !body_json_param.is_string())
                                    || (type == ParamType::OBJECT && !body_json_param.is_object())
                                    || (type == ParamType::ARRAY && !body_json_param.is_array())
                                ) {
                                    throw invalid_argument(fmt::format("param {} has invalid type", name));
                                }
                            }
                        }

                        handle_result = handler->handle(handler_context);

                        global_context->logger->info("SERVER::REQUEST", fmt::format(
                            "({}/{}) ({}) {} {}",
                            handler->get_signature().name,
                            request.method,
                            response.status,
                            handle_id,
                            request.path
                        ), __FILE__, __LINE__);
                    } catch (const string& exc) {
                        response.status = 401;
                        handle_result = "unauthorized";
                    } catch (const invalid_argument& exc) {
                        response.status = 400;
                        handle_result = fmt::format("invalid_argument: {}", exc.what());
                    } catch (const runtime_error& exc) {
                        response.status = 409;
                        handle_result = fmt::format("runtime_error: {}", exc.what());
                    } catch (const exception& exc) {
                        response.status = 500;
                        handle_result = fmt::format("unexpected_exception: {}", exc.what());
                        global_context->logger->error("SERVER::HANDLE", exc.what(), __FILE__, __LINE__);
                    } catch (...) {
                        response.status = 500;
                        handle_result = "unexpected_exception";
                        global_context->logger->error("SERVER::HANDLE", "", __FILE__, __LINE__);
                    }
                    response.set_content(
                        json{
                            {HANDLE_ID_KEY, handle_id},
                            {STATUS_KEY, response.status}, 
                            {RESULT_KEY, handle_result},
                        }.dump(), 
                        "application/json"
                    );

                    #ifndef NDEBUG
                    global_context->logger->debug("SERVER::RESPONSE", response.body, __FILE__, __LINE__);
                    #endif
                };

                const HandlerSignature& signature = handler->get_signature();
                if (signature.method == RequestHandlerMethod::GET) {
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
                        "handler={} method={} pattern={}",
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