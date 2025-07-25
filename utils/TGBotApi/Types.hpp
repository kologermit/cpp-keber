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

using Utils::TGBotApi::User::InterfaceUser;
using Utils::TGBotApi::User::User;
using ptrUser = std::shared_ptr<InterfaceUser>;

using Utils::TGBotApi::Chat::InterfaceChat;
using Utils::TGBotApi::Chat::Chat;
using ptrChat = std::shared_ptr<InterfaceChat>;

using Utils::TGBotApi::Bot::InterfaceBot;
using Utils::TGBotApi::Bot::Bot;
using ptrBot = std::shared_ptr<InterfaceBot>;

using Utils::TGBotApi::Message::InterfaceMessage;
using Utils::TGBotApi::Message::Message;
using ptrMessage = std::shared_ptr<InterfaceMessage>;

using Utils::TGBotApi::Message::Keyboard::InterfaceInlineKeyboard;
using Utils::TGBotApi::Message::Keyboard::InlineKeyboard;
using ptrInlineKeyboard = std::shared_ptr<InterfaceInlineKeyboard>;

using Utils::TGBotApi::Message::Keyboard::InterfaceInlineButton;
using Utils::TGBotApi::Message::Keyboard::InlineButton;
using ptrInlineButton = std::shared_ptr<InterfaceInlineButton>;
using ptrInlineButtons = std::shared_ptr<std::vector<std::vector<InterfaceInlineButton> > >;

using Utils::TGBotApi::Message::Keyboard::InterfaceReplyKeyboard;
using Utils::TGBotApi::Message::Keyboard::ReplyKeyboard;
using ptrReplyKeyboard = std::shared_ptr<InterfaceReplyKeyboard>;

using Utils::TGBotApi::Message::Keyboard::InterfaceReplyButton;
using Utils::TGBotApi::Message::Keyboard::ReplyButton;
using ptrReplyButton = std::shared_ptr<InterfaceReplyButton>;
using ptrReplyButtons = std::shared_ptr<std::vector<std::vector<InterfaceReplyButton> > >;

using Utils::TGBotApi::CallbackQuery::InterfaceCallbackQuery;
using Utils::TGBotApi::CallbackQuery::CallbackQuery;
using ptrCallbackQuery = std::shared_ptr<InterfaceCallbackQuery>;



}