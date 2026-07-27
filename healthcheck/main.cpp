#include <iostream>
#include <vector>
#include <httplib.h>
#include <utils/Config/ConfigParser.hpp>

int main(int argc, const char* argv[]) {
    using std::cout;
    using std::endl;
    using std::vector;
    using std::string;
    using std::shared_ptr;
    using std::invalid_argument;
    using std::pair;
    using httplib::Client;
    using httplib::Result;
    using httplib::Error;
    using Utils::Config::Argument;
    using Utils::Config::parse_config;
    using Utils::Config::get_help_by_vector_arguments;

    string url, psth;
    vector<Argument> arguments{
        Argument{
            .name = "URL",
            .is_required = true,
            .string_value = &url,
        },
    };
    const pair<shared_ptr<invalid_argument>, bool> parse_result = parse_config(argc, argv, arguments);
    const shared_ptr<invalid_argument> exc = parse_result.first;
    const bool is_help = parse_result.second;
    if (is_help) {
        cout << get_help_by_vector_arguments("cpp-keber-healthcheck", arguments) << endl;
        return 0;
    }
    if (exc != nullptr) {
        throw *exc;
    }

    Client client(url);
    client.set_read_timeout(5);
    client.set_write_timeout(5);
    client.set_connection_timeout(5);
    client.enable_server_certificate_verification(false);

    const Result response = client.Get("/health");
    if (response.error() != Error::Success) {
        cout << "Error: " << response.error() << endl;
        return 1;
    }

    if (response->status < 200 || response->status >= 300) {
        cout << "Unhealth status: " << response->status << endl;
        return 1;
    }

    cout << response->body << endl;
    return 0;
}