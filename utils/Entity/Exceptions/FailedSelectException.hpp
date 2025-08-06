#pragma once

#include <string>
#include <string_view>
#include <exception>

namespace Utils::Entity::Exceptions {

using std::string;
using std::string_view;
using std::exception;

struct FailedSelectException : exception {

    FailedSelectException(string_view message): _message(message) {};

    const char* what() const noexcept override {
        return _message.data();
    }

    private:

    string _message;

};

}