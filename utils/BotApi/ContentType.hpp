#pragma once

namespace Utils::BotApi {
    enum class ContentType : int {
        UNKNOWN,
        PHOTO,
        VIDEO,
        AUDIO,
        DOCUMENT,
        TEXT,
    };
}