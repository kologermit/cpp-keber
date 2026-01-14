#include <utils/Api/Api.hpp>
#include <fmt/core.h>

#ifndef NDEBUG
#include <utils/Logger/InterfaceLogger.hpp>
#endif

namespace Utils::Api {
    using httplib::Error;
    using nlohmann::json;
    using std::to_string;
    using std::runtime_error;

    #ifndef NDEBUG
    using Utils::Logger::get_logger;
    #endif

    void throw_by_status_or_error(
        [[maybe_unused]]string_view base_url, 
        [[maybe_unused]]string_view path, 
        const Result& response
    ) {
        #ifndef NDEBUG
        get_logger()->debug("Api::BaseUrl", base_url, __FILE__, __LINE__);
        get_logger()->debug("Api::Path", path, __FILE__, __LINE__);
        get_logger()->debug("Api::Error", to_string(response.error()), __FILE__, __LINE__);
        #endif
        if (response.error() != Error::Success) {
            throw runtime_error(fmt::format("Api: error {}", to_string(response.error())));
        }
        #ifndef NDEBUG
        get_logger()->debug("Api::Status", to_string(response->status), __FILE__, __LINE__);
        get_logger()->debug("Api::Body", response->body, __FILE__, __LINE__);
        #endif
        if (response->status >= 300 || response->status < 200) {
            throw runtime_error(fmt::format("Api: status {}", response->status));
        }
        if (!json::accept(response->body)) {
            throw runtime_error("Api: body isn't json");
        }
    }
}