#include <utils/HTTP/HTTP.hpp>
#include <utils/Config/InterfaceConfig.hpp>
#include <stdexcept>
#include <fmt/core.h>

namespace Utils::HTTP {

    using Utils::Config::get_config;
    using std::ifstream;
    using std::invalid_argument;
    using std::exception;
    using std::stoll;
    using std::stold;

    void validate_params(const Request& request, const vector<CheckParam>& params) {
        for (const auto& param : params) {
            const string param_str = request.get_param_value(param.name);
            if (param.required && param_str.empty()) {
                throw invalid_argument(fmt::format("required param {} not found", param.name));
            }
            if (!param.required && param_str.empty()) {
                continue;
            }
            switch (param.type) {
                case CheckType::STRING: continue; break;
                case CheckType::LONG_LONG:
                    try {
                        stoll(param_str);
                    } catch (const exception&) {
                        throw invalid_argument(fmt::format("param {} must be type int", param.name));
                    }
                    break;
                case CheckType::LONG_DOUBLE:
                    try {
                        stold(param_str);
                    } catch (const exception&) {
                        throw invalid_argument(fmt::format("param {} must be type float", param.name));
                    }
                    break;
                case CheckType::BOOL:
                    if (param_str != "true" && param_str != "false") {
                        throw invalid_argument(fmt::format("param {} must be type bool", param.name));
                    }
                    break;
                case CheckType::FILE:
                    ifstream file(fmt::format("{}/{}", get_config()->get_file_buffer_path(), param_str));
                    if (!file.good()) {
                        throw invalid_argument(fmt::format("cant read file {}", param_str));
                    }
                    break;
            }
        }
    }
}