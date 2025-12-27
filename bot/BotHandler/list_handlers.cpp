#include <bot/BotHandler/list_handlers.hpp>
#include <bot/BotHandler/Access/AccessHandler.hpp>
#include <bot/BotHandler/Access/AccessCallbackHandler.hpp>
#include <bot/BotHandler/CallbackDeleter/CallbackDeleterHandler.hpp>
#include <bot/BotHandler/Dice/DiceHandler.hpp>
#include <bot/BotHandler/Menu/MenuHandler.hpp>
#include <bot/BotHandler/NotFound/NotFoundHandler.hpp>
#include <bot/BotHandler/Start/StartHandler.hpp>
#include <bot/BotHandler/YouTube/YouTubeHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaHandler.hpp>
#include <bot/BotHandler/YouTube/Media/MediaCallbackHandler.hpp>
#include <bot/BotHandler/YouTube/Playlist/PlaylistHandler.hpp>


namespace Bot::BotHandler {

    using std::make_shared;
    using Bot::BotHandler::Access::AccessHandler;
    using Bot::BotHandler::Access::AccessCallbackHandler;
    using Bot::BotHandler::CallbackDeleter::CallbackDeleterHandler;
    using Bot::BotHandler::Dice::DiceHandler;
    using Bot::BotHandler::Menu::MenuHandler;
    using Bot::BotHandler::NotFound::NotFoundHandler;
    using Bot::BotHandler::Start::StartHandler;
    using Bot::BotHandler::YouTube::YouTubeHandler;
    using Bot::BotHandler::YouTube::Media::MediaHandler;
    using Bot::BotHandler::YouTube::Media::MediaCallbackHandler;
    using Bot::BotHandler::YouTube::Playlist::PlaylistHandler;

    const vector<shared_ptr<InterfaceBotHandler> >& get_list_bot_handlers() {
        static const vector<shared_ptr<InterfaceBotHandler> > list_bot_handlers{
            make_shared<DiceHandler>(),
            make_shared<StartHandler>(),
            make_shared<MenuHandler>(),
            make_shared<AccessCallbackHandler>(),
            make_shared<AccessHandler>(),
            make_shared<YouTubeHandler>(),
            make_shared<MediaHandler>(),
            make_shared<MediaCallbackHandler>(),
            make_shared<PlaylistHandler>(),
            make_shared<CallbackDeleterHandler>(),
            make_shared<NotFoundHandler>(),
        };

        return list_bot_handlers;
    }
}