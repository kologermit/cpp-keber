#include <filesystem>
#include <stdexcept>
#include <utils/TGBotApi/File/File.hpp>
#include <utils/Types.hpp>
#include <fmt/core.h>

namespace Utils::TGBotApi::File {

using Utils::Types::const_string;
using fmt::format;
using std::to_string;
using std::runtime_error;
using std::filesystem::exists;
using std::filesystem::is_regular_file;
using std::filesystem::file_size;

const int MAX_FILE_SIZE = 40 * 1024 * 1024; // 40MB

ResultCheckFile is_correct_file(string_view filepath) {
    const_string _filepath(filepath);

    if (!exists(filepath)) {
        return ResultCheckFile::NOT_FOUND;
    }

    if (!is_regular_file(filepath)) {
        return ResultCheckFile::READ_DENIED;
    }

    if (file_size(filepath) > MAX_FILE_SIZE) {
        return ResultCheckFile::TOO_LARGE;
    }

    return ResultCheckFile::OK;
}

void throw_if_not_correct_file(string_view filepath) {
    ResultCheckFile check_result = is_correct_file(filepath);
    if (is_correct_file(filepath) != ResultCheckFile::OK) {
        throw runtime_error(format(
            "{}: {} is {}",
            "Utils::TGBotApi::File::throw_if_not_correct_file",
            filepath,
            to_string(check_result)
        ));
    }
}

}

namespace std {
    using Utils::TGBotApi::File::ResultCheckFile;
    std::string to_string(ResultCheckFile result_check) {
        switch (result_check) {
            case ResultCheckFile::NOT_FOUND:
                return "NOT_FOUND";
                break;
            case ResultCheckFile::READ_DENIED:
                return "READ_DENIED";
                break;
            case ResultCheckFile::TOO_LARGE:
                return "TOO_LARGE";
                break;
            case ResultCheckFile::OK:
                return "OK";
                break;
        }
        return "UNKNOWN";
    };
}