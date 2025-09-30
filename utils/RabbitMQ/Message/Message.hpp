#pragma once

#include <string>
#include <map>
#include <nlohmann/json.hpp>

namespace Utils::RabbitMQ {
    using std::string;
    using std::map;
    using nlohmann::json;
    
    struct Message {
        long long payload_bytes;
        bool redelivered;
        string exchange;
        string routing_key;
        long long message_count;
        long long delivery_mode;
        json payload;
        map<string, string> headers;
        string payload_encoding;

        Message(const json& json_message) :
        payload_bytes(json_message.value("payload_bytes", 0)),
        redelivered(json_message.value("redelivered", false)),
        exchange(json_message.value("exchange", "")),
        routing_key(json_message.value("routing_key", "")),
        message_count(json_message.value("message_count", 0)),
        delivery_mode(get_delivery_mode(json_message)),
        payload_encoding(json_message.value("payload_encoding", ""))
        {
            if (json_message.contains("payload") && json_message["payload"].is_string()) {
                string payload_str = json_message["payload"].get<string>();
                if (json::accept(payload_str)) {
                    payload = json::parse(payload_str);
                } else {
                    payload = payload_str;
                }
            } else if (json_message.contains("payload")) {
                payload = json_message["payload"];
            }
            if (json_message.contains("properties") && 
                json_message["properties"].is_object() &&
                json_message["properties"].contains("headers") &&
                json_message["properties"]["headers"].is_object()) {
                
                for (const auto& [key, value] : json_message["properties"]["headers"].items()) {
                    if (value.is_string()) {
                        headers[key] = value.get<string>();
                    } else {
                        headers[key] = value.dump();
                    }
                }
            }
        }

        private:
        static long long get_delivery_mode(const json& json_message) {
            if (json_message.contains("properties") && 
                json_message["properties"].is_object() &&
                json_message["properties"].contains("delivery_mode")) {
                return json_message["properties"]["delivery_mode"].get<long long>();
            }
            return 2;
        }
    };
}