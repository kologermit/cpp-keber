#include <utils/Postgres/postgres.hpp>
#include <utils/Logger/InterfaceLogger.hpp>
#include <fmt/core.h>
#include <unistd.h>

namespace Utils::Postgres {
    using pqxx::broken_connection;
    using Utils::Logger::get_logger;
    
    connection create_connection(string_view conn_url) {
        while (true) {
            try {
                return connection(conn_url.data());
            } catch (const broken_connection& exc) {
                get_logger()->warning("POSTGRES::CONNECTION", fmt::format(
                    "Failed to connect: {}",
                    exc.what()
                ), __FILE__, __LINE__);
                sleep(3);
            }
        }
    }
}