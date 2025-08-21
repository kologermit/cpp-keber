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

using Utils::TGBotApi::User::User;
using ptrUser = std::unique_ptr<User>;

using Utils::TGBotApi::Chat::Chat;
using ptrChat = std::unique_ptr<Chat>;

using Utils::TGBotApi::Bot::Bot;
using Utils::TGBotApi::Bot::InterfaceBot;
using ptrBot = std::unique_ptr<InterfaceBot>;

using Utils::TGBotApi::Message::Message;
using ptrMessage = std::unique_ptr<Message>;

using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
using ptrInlineKeyboard = std::unique_ptr<InlineKeyboard>;

using Utils::TGBotApi::Message::Keyboard::InlineButton;
using ptrInlineButton = std::shared_ptr<InlineButton>;
using InlineLane = std::vector<ptrInlineButton>;
using InlineButtons = std::vector<InlineLane>;
using ptrInlineButtons = std::unique_ptr<InlineButtons>;

using Utils::TGBotApi::Message::Keyboard::ReplyKeyboard;
using ptrReplyKeyboard = std::unique_ptr<ReplyKeyboard>;

using Utils::TGBotApi::Message::Keyboard::ReplyButton;
using ReplyButtons = std::vector<std::vector<std::shared_ptr<ReplyButton> > >;

using Utils::TGBotApi::CallbackQuery::CallbackQuery;
using ptrCallbackQuery = std::unique_ptr<CallbackQuery>;



}