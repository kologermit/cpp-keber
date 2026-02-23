#include <utils/TGBotApi/Message/Keyboard/Style.hpp>

namespace std {
    const string& to_string(Style style) {
        static const string red = "danger";
        static const string blue = "primary";
        static const string green = "success";
        static const string unknown = "unknown";
        switch (style) {
            case Style::RED: return red;
            case Style::BLUE: return blue;
            case Style::GREEN: return green;
        }
        return unknown;
    }
}