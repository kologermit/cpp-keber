#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace Utils::String {
    using std::vector;
    using std::string;
    using std::string_view;

    vector<string> split(const string& text, string_view key) noexcept;
}