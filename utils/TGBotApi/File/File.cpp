#include <filesystem>
#include <utils/TGBotApi/File/File.hpp>
#include <fmt/core.h>
#include <algorithm>

namespace Utils::TGBotApi::File {

using fmt::format;
using std::to_string;
using std::filesystem::exists;
using std::filesystem::is_regular_file;
using std::filesystem::file_size;

const int MAX_FILE_SIZE = 40 * 1024 * 1024; // 40MB

EnumResultCheckFile is_correct_file(string_view filepath) {
    if (!exists(filepath)) {
        return EnumResultCheckFile::NOT_FOUND;
    }

    if (!is_regular_file(filepath)) {
        return EnumResultCheckFile::READ_DENIED;
    }

    if (file_size(filepath) > MAX_FILE_SIZE) {
        return EnumResultCheckFile::TOO_LARGE;
    }

    return EnumResultCheckFile::OK;
}

void throw_if_not_correct_file(string_view filepath) {
    EnumResultCheckFile check_result = is_correct_file(filepath);
    if (is_correct_file(filepath) != EnumResultCheckFile::OK) {
        throw NotCorrentFileException(format(
            "{}: {} is {}",
            "Utils::TGBotApi::File::throw_if_not_correct_file",
            filepath,
            to_string(check_result)
        ));
    }
}

}

namespace std {
    using Utils::TGBotApi::File::convert_map_result_check_file;
    using Utils::TGBotApi::File::EnumResultCheckFile;
    using Utils::TGBotApi::File::convert_map_content_type;
    using Utils::TGBotApi::File::EnumContentType;

    
    string to_string(EnumResultCheckFile result_check) {
        if (convert_map_result_check_file.find(result_check) == convert_map_result_check_file.end()) {
            return "UNKNOWN";
        }

        return convert_map_result_check_file.at(result_check);
    };

    string to_string(Utils::TGBotApi::File::EnumContentType content_type) {

        if (convert_map_content_type.find(content_type) == convert_map_content_type.end()) {
            return "UNKNOWN";
        }

        return convert_map_content_type.at(content_type);
    }
}