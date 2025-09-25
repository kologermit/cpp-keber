#pragma once

#include <utils/TGBotApi/User/User.hpp>
#include <utils/TGBotApi/Chat/Chat.hpp>
#include <utils/TGBotApi/Bot/Bot.hpp>
#include <utils/TGBotApi/Message/Message.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/InlineButton.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyKeyboard.hpp>
#include <utils/TGBotApi/Message/Keyboard/ReplyButton.hpp>
#include <utils/TGBotApi/CallbackQuery/CallbackQuery.hpp>

namespace Utils::TGBotApi::Types {

    using TGBotApi::User::User;
    using ptrUser = std::unique_ptr<User>;

    using TGBotApi::Chat::Chat;
    using ptrChat = std::unique_ptr<Chat>;

    using TGBotApi::Bot::Bot;
    using TGBotApi::Bot::InterfaceBot;
    using ptrBot = std::unique_ptr<InterfaceBot>;

    using TGBotApi::Message::Message;
    using ptrMessage = std::unique_ptr<Message>;

    using TGBotApi::Message::Keyboard::InlineKeyboard;
    using ptrInlineKeyboard = std::unique_ptr<InlineKeyboard>;

    using TGBotApi::Message::Keyboard::InlineButton;
    using InlineLane = std::vector<std::shared_ptr<InlineButton> >;
    using InlineButtons = std::vector<InlineLane>;
    using ptrInlineButtons = std::unique_ptr<InlineButtons>;

    using TGBotApi::Message::Keyboard::ReplyKeyboard;
    using ptrReplyKeyboard = std::unique_ptr<ReplyKeyboard>;

    using TGBotApi::Message::Keyboard::ReplyButton;
    using ReplyLane = std::vector<std::shared_ptr<ReplyButton> >;
    using ReplyButtons = std::vector<ReplyLane>;

    using TGBotApi::CallbackQuery::CallbackQuery;
    using ptrCallbackQuery = std::unique_ptr<CallbackQuery>;


}