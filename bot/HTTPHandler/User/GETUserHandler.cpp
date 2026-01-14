#include <bot/HTTPHandler/User/GETUserHandler.hpp>

namespace Bot::HTTPHandler::User {
    using std::vector;
    using std::unique_ptr;
    using Bot::Entity::User::User;
    using Bot::Entity::User::map_user_screen_to_string;
    using Utils::HTTPServer::Handler::Param;
    using Utils::HTTPServer::Handler::ParamType;
    using Utils::HTTPServer::Handler::RequestHandlerMethod;

    const HandlerSignature& GETUserHandler::get_signature() const noexcept {
        static const HandlerSignature signature{
            .name = "GETUserHandler",
            .pattern = "/user/:id",
            .method = RequestHandlerMethod::GET,
            .is_auth = true,
            .path_params{
                Param{"id", ParamType::INT}
            }
        };
        return signature;
    }

    json GETUserHandler::handle(ptrContext ctx) {
        const long long id = ctx->ll_params["id"];
        const unique_ptr<User> user = ctx->db->user->get_by_id(id, true);

        if (user == nullptr) {
            return nullptr;
        }

        json result{
            {"id", user->id},
            {"name", user->name},
            {"username", user->username},
            {"screen", user->screen},
            {"created_at", user->created_at.to_string()},
            {"updated_at", user->updated_at.to_string()},
        };
        if (user->deleted_at != nullopt) {
            result["deleted_at"] = user->deleted_at.value().to_string();
        } else {
            result["deleted_at"] = nullptr;
        }
        return result;
    }
}
