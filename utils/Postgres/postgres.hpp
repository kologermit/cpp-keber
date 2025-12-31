#pragma once

#include <pqxx/pqxx>
#include <string_view>

namespace Utils::Postgres {
    using std::string_view;
    using pqxx::connection;

    connection create_connection(string_view conn_url);
}