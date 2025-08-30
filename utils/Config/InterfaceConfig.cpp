#include <utils/Config/InterfaceConfig.hpp>
#include <stdexcept>

namespace Utils::Config {

    using std::move;
    using std::runtime_error;

    const InterfaceConfig* get_config(unique_ptr<InterfaceConfig> config) {
        static unique_ptr<InterfaceConfig> _config = nullptr;

        if (config != nullptr && _config == nullptr) {
            _config = move(config);
        }

        if (_config == nullptr) {
            throw runtime_error("Bot::Config::get_config not initialized!");
        }

        return _config.get();
    }

}