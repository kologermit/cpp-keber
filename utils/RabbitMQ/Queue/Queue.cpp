#include <utils/RabbitMQ/Queue/Queue.hpp>
#include <fmt/core.h>
#include <stdexcept>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::RabbitMQ {

    using std::runtime_error;
    using std::to_string;
    using std::nullopt;
    using std::make_unique;
    using httplib::Result;
    using httplib::Error;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    constexpr const char* APPLICATION_JSON = "application/json";

    string encode_vhost(string_view vhost) {
        if (vhost == "/") {
            return "%2F";
        }
        return httplib::detail::encode_url(vhost.data());
    }

    Queue::Queue(
        string_view name,
        string_view vhost,
        string_view login,
        string_view password,
        string_view host,
        int port,
        time_t timeout,
        bool declare,
        bool auto_delete, 
        bool durable, 
        const json& arguments
    ):
        _host(host),
        _port(port),
        _name(name),
        _vhost(encode_vhost(vhost)),
        _login(login),
        _password(password),
        _timeout(timeout),
        _client(_host, _port)
    {
        #ifndef NDEBUG
        get_logger()->debug("Queue::name", name, __FILE__, __LINE__);
        get_logger()->debug("Queue::vhost", vhost, __FILE__, __LINE__);
        get_logger()->debug("Queue::login", login, __FILE__, __LINE__);
        get_logger()->debug("Queue::password", password, __FILE__, __LINE__);
        get_logger()->debug("Queue::host", string(host) + ":" + to_string(port), __FILE__, __LINE__);
        get_logger()->debug("Queue::timeout", to_string(timeout), __FILE__, __LINE__);
        #endif   

        _client.set_basic_auth(_login, _password);
        _client.set_write_timeout(_timeout);
        _client.set_read_timeout(_timeout);
        _client.set_connection_timeout(_timeout);
        _client.enable_server_certificate_verification(false);

        if (!declare) {
            return;
        }

        test_connection();

        if (!exists()) {
            this->declare(auto_delete, durable, arguments);
        }
    }

    void throw_if_invalid_result(const Result& result) {
        if (!result || result.error() != Error::Success) {
            throw runtime_error(fmt::format("RabbitMQ::Queue: invalid query result ({})", to_string(result.error())));
        }
        #ifndef NDEBUG
        get_logger()->debug("body", result->body, __FILE__, __LINE__);
        get_logger()->debug("status", to_string(result->status), __FILE__, __LINE__);
        #endif
    }

    bool Queue::test_connection() const {
        static const char* path = "/api/overview";
        #ifndef NDEBUG
        get_logger()->debug("Queue::test_connection::path", path, __FILE__, __LINE__);
        #endif
        auto result = _client.Get(path);
        return result && result->status == 200;
    }

    bool Queue::exists() const {
        const string path = fmt::format("/api/queues/{}/{}", _vhost, _name);
        #ifndef NDEBUG
        get_logger()->debug("Queue::exists::path", path, __FILE__, __LINE__);
        #endif
        auto result = _client.Get(path);
        throw_if_invalid_result(result);
        return result->status == 200;
    }

    bool Queue::declare(bool auto_delete, bool durable, const json& arguments) const {
        const string path = fmt::format("/api/queues/{}/{}", _vhost, _name);
        json body{
            {"auto_delete", auto_delete},
            {"durable", durable},
            {"arguments", arguments},
        };
        #ifndef NDEBUG
        get_logger()->debug("Queue::declare::path", path, __FILE__, __LINE__);
        get_logger()->debug("Queue::declare::body", body.dump(), __FILE__, __LINE__);
        #endif
        auto result = _client.Put(path, body.dump(), APPLICATION_JSON);
        throw_if_invalid_result(result);
        return result->status != 201 && result->status != 204;
    }

    bool Queue::publish_message(const json& data) const {
        const string path = fmt::format("/api/exchanges/{}/amq.default/publish", _vhost);
        json body = {
            {"properties", {}},
            {"routing_key", _name},
            {"payload", data.dump()},
            {"payload_encoding", "string"}
        };
        #ifndef NDEBUG
        get_logger()->debug("Queue::publish_message::path", path, __FILE__, __LINE__);
        get_logger()->debug("Queue::publish_message::body", body.dump(), __FILE__, __LINE__);
        #endif
        auto result = _client.Post(path, body.dump(), APPLICATION_JSON);
        throw_if_invalid_result(result);
        if (result->status != 200) {
            return false;
        }
        auto parsed_result = json::parse(result->body);
        return bool(parsed_result.value("routed", false));
    }

    unique_ptr<Message> Queue::get_message(bool acknowledge) const {
        const string path = fmt::format("/api/queues/{}/{}/get", _vhost, _name);
        json body{
            {"count", 1},
            {"ackmode", acknowledge ? "ack_requeue_false" : "ack_requeue_true"},
            {"encoding", "auto"},
            {"truncate", 50000}
        };
        #ifndef NDEBUG
        get_logger()->debug("Queue::get_message::path", path, __FILE__, __LINE__);
        get_logger()->debug("Queue::get_message::body", body.dump(), __FILE__, __LINE__);
        #endif
        auto result = _client.Post(path.c_str(), body.dump(), APPLICATION_JSON);
        throw_if_invalid_result(result);
        if (result->status != 200) {
            return nullptr;
        }
        const auto parsed_body = json::parse(result->body);
        if (parsed_body.empty()) {
            return nullptr;
        }
        return make_unique<Message>(parsed_body[0]);
    }
}