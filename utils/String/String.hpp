#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace Utils::String {
    using std::vector;
    using std::string;
    using std::string_view;

    vector<string> split(const string& text, string_view key) noexcept;

    template<typename Iterator>
    string join(Iterator begin_iter, Iterator end_iter, string_view key = " ") {
        string result = (begin_iter != end_iter ? *(begin_iter++) : string(""));
        for (; begin_iter != end_iter; ++begin_iter) {
            result += key;
            result += *begin_iter;
        }
        return result;
    }
}