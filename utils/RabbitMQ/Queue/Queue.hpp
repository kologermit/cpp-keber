#pragma once
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <string>
#include <string_view>
#include <memory>
#include <optional>
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>
#include <utils/RabbitMQ/Message/Message.hpp>

namespace Utils::RabbitMQ {

    using std::string;
    using std::string_view;
    using std::unique_ptr;
    using httplib::Client;
    using nlohmann::json;

    struct Queue {

        explicit Queue(
            string_view name,
            string_view vhost,
            string_view login,
            string_view password,
            string_view host = "localhost",
            int port = 15672,
            time_t timeout = 30,
            bool declare = true,
            bool auto_delete = false, 
            bool durable = true, 
            const json& arguments = json::object()
        );

        bool test_connection() const;
        bool exists() const;
        bool declare(bool auto_delete = false, bool durable = false, const json& arguments = json::object()) const;
        bool publish_message(const json& data) const;
        unique_ptr<Message> get_message(bool acknowledge = true) const;

        private:

        const string _host;
        const int _port;
        const string _name;
        const string _vhost;
        const string _login;
        const string _password;
        const time_t _timeout;

        mutable Client _client;
    };
}