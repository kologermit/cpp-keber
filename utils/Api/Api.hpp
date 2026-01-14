#pragma once

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include <httplib/httplib.h>
#include <nlohmann/json.hpp>

namespace Utils::Api {

    using std::string_view;
    using std::string;
    using httplib::Client;
    using httplib::Result;
    using httplib::Headers;

    struct Api {
        Api(string_view base_url, string_view auth_key):
            _base_url(base_url),
            _auth_key(auth_key),
            _client(_base_url)
        {
            _client.set_default_headers(Headers{
                {"Authorization", _auth_key},\
            });
        }

        protected:

        const string _base_url;
        const string _auth_key;
        Client _client;
    };

    void throw_by_status_or_error(
        string_view base_url, 
        string_view path, 
        const Result& response
    );
}