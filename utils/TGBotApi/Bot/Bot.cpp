#pragma once


#include <utils/Logger/InterfaceLogger.cpp>
#include <utils/Type.cpp>
#include <utils/UUID4/UUID4.cpp>
#include <utils/TGBotApi/Query.cpp>
#include <utils/TGBotApi/User/User.cpp>
#include <utils/TGBotApi/Bot/InterfaceBot.cpp>

namespace Utils {
namespace TGBotApi {
namespace Bot {

using namespace fmt;
using namespace Utils::TGBotApi::User;
using namespace Utils::Type;
using namespace Utils::Logger;

class Bot : public virtual InterfaceBot, public User {

    protected:
    const_string _token;
    const_string _secret;

    public:

    const User& get_me() const override { return Query<User>(_token).Get("getMe").result; }
    
    Bot(const_string& token): _token(token), _secret(UUID4::generate_str()) {
        static_cast<User&>(*this) = get_me();
    }

    void delete_webhook() const override { Query<User>(_token).Get("deleteWebhook"); }
    void set_webhook(const_string& webhook_url) const override { 
        Query<User>(_token).Get("setWebhook", {
            {"url", webhook_url},
            {"secret", _secret}
        }); 
    }
};

}
}
}