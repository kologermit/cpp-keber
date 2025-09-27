#include <bot/Entity/Repositories.hpp>
#include <bot/Entity/Message/MessageRepository.hpp>
#include <bot/Entity/User/UserRepository.hpp>
#include <bot/Entity/Chat/ChatRepository.hpp>
#include <bot/Entity/Access/AccessRepository.hpp>
#include <bot/Entity/Callback/CallbackRepository.hpp>
#include <utils/Entity/ApiRequest/ApiRequestRepository.hpp>

namespace Bot::Entity::Repositories {
    using std::make_shared;
    using std::runtime_error;
    using Utils::Entity::ApiRequest::ApiRequestRepository;
    using Bot::Entity::Message::MessageRepository;
    using Bot::Entity::User::UserRepository;
    using Bot::Entity::Chat::ChatRepository;
    using Bot::Entity::Access::AccessRepository;
    using Bot::Entity::Callback::CallbackRepository;

    Repositories::Repositories(string_view conn_str):
        _db(conn_str.data()),
        api_request_repository(make_shared<ApiRequestRepository>(_db)),
        message_repository(make_shared<MessageRepository>(_db)),
        callback_repository(make_shared<CallbackRepository>(_db)),
        user_repository(make_shared<UserRepository>(_db)),
        chat_repository(make_shared<ChatRepository>(_db)),
        access_repository(make_shared<AccessRepository>(_db))
    {}

    shared_ptr<Repositories> get_repositories(unique_ptr<Repositories> repositories) {
        static shared_ptr<Repositories> static_repositories = nullptr;

        if (static_repositories == nullptr && repositories == nullptr) {
            throw runtime_error("get_repositories -- not initialized!");
        }

        if (static_repositories == nullptr) {
            static_repositories = std::move(repositories);
        }

        return static_repositories;
    }
    
}