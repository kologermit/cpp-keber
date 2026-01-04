#include <utils/String/String.hpp>

namespace Utils::String {
    vector<string> split(const string& text, string_view key) noexcept {
        vector<string> result;
        size_t start = 0;
        size_t end = text.find(key);

        while (end != std::string::npos) {
            result.push_back(text.substr(start, end - start));
            start = end + key.length();
            end = text.find(key, start);
        }

        result.push_back(text.substr(start));
        return result;
    }
}
