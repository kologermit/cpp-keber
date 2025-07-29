// #define CPPHTTPLIB_OPENSSL_SUPPORT

#include <bot/src/Config/Config.hpp>
#include <utils/Logger/StdOutLogger.hpp>
#include <memory>

using Bot::Config::InterfaceConfig;
using Bot::Config::Config;
using std::shared_ptr;
using std::make_shared;
using std::make_unique;

auto logger = Utils::Logger::get_logger(make_unique<Utils::Logger::StdOutLogger>());

int main(int argc, const char* argv[]) {
    shared_ptr<InterfaceConfig> config = make_shared<Config>(argc, argv);
    return 0;
}
