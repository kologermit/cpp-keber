#include <bot/Config/Config.hpp>
#include <bot/Server/Server.hpp>
#include <bot/Entity/User/UserRepository.hpp>
#include <bot/Entity/Chat/ChatRepository.hpp>
#include <bot/Entity/Message/MessageRepository.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>
#include <utils/Logger/JSONSTDOutLogger.hpp>
#include <utils/Random/Random.hpp>

using Utils::Logger::get_logger;
using Utils::Logger::JSONSTDOutLogger;
using TGBot = Utils::TGBotApi::Bot::Bot;
using Utils::Entity::ApiRequest::ApiRequestRepository;
using Utils::Random::rand_int;
using Utils::TGBotApi::Types::ptrMessage;
using Bot::Config::get_config;
using Bot::Config::Config;
using Bot::Entity::Repositories::Repositories;
using Bot::Entity::User::UserRepository;
using Bot::Entity::User::map_user_screen_to_string;
using Bot::Entity::User::EnumUserScreen;
using Bot::Entity::User::MENU;
using Bot::Entity::User::SCREEN;
using Bot::Entity::Chat::ChatRepository;
using Bot::Entity::Message::MessageRepository;
using Bot::Entity::Repositories::Repositories;
using Bot::Entity::Repositories::Repositories;
using Bot::Handler::InterfaceBotHandler;
using Bot::Handler::BotHandlerContext;
using Bot::Server::Server;
using std::shared_ptr;
using std::unique_ptr;
using std::make_unique;
using std::make_shared;

auto logger = get_logger(make_unique<JSONSTDOutLogger>());

struct ParrotHandler : InterfaceBotHandler {
    bool check(shared_ptr<BotHandlerContext> context) override {
        return context->message.text.starts_with("/random_screen");
    }

    ptrMessage handle(shared_ptr<BotHandlerContext> context) override {
        context->user.screen = static_cast<EnumUserScreen>(rand_int(1, 10)); 

        context->repositories->user_repository->update(context->user, {SCREEN});

        return context->bot->send_message({
            .chat_id = context->chat.telegram_id,
            .text = map_user_screen_to_string.at(context->user.screen),
            .reply_message_id = context->message.telegram_id,
        });
    }

};

int main(int argc, const char* argv[]) {
    auto config = get_config(make_unique<Config>(argc, argv));

    if (config->is_help()) {
        logger->info("HELP", "THIS IS BOT SERVICE", __FILE__, __LINE__);
        return 0;
    }

    auto bot = make_shared<TGBot>(config->get_bot_token());
    auto repositories = make_shared<Repositories>();

    repositories->api_request_repository = make_shared<ApiRequestRepository>(config->get_db_conn_url());
    repositories->user_repository = make_shared<UserRepository>(config->get_db_conn_url());
    repositories->chat_repository = make_shared<ChatRepository>(config->get_db_conn_url());
    repositories->message_repository = make_shared<MessageRepository>(config->get_db_conn_url());

    bot->set_webhook(config->get_webhook_url());

    Server server(repositories, bot);
    server.add_bot_handler(make_unique<ParrotHandler>());
    server.run();

    return 0;
}
