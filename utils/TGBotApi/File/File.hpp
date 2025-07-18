#pragma once

#include <string_view>
#include <map>
#include <utils/Types.hpp>

namespace Utils::TGBotApi::File {

using std::string_view;
using std::map;
using Utils::Types::const_string;

enum EnumResultCheckFile {
    NOT_FOUND,
    READ_DENIED,
    TOO_LARGE,
    UNSUPPORTED_CONTENT_TYPE,
    OK
};

enum EnumContentType {
    PHOTO,
    VIDEO,
    AUDIO,
    DOCUMENT,
    TEXT,
    UNKNOWN
};

const map<EnumContentType, const_string> convert_map_content_type = {
    {EnumContentType::PHOTO, "PHOTO"},
    {EnumContentType::VIDEO, "VIDEO"},
    {EnumContentType::AUDIO, "AUDIO"},
    {EnumContentType::DOCUMENT, "DOCUMENT"},
    {EnumContentType::TEXT, "TEXT"},
};

const map<EnumResultCheckFile, const_string> convert_map_result_check_file = {
    {EnumResultCheckFile::NOT_FOUND, "NOT_FOUND"},
    {EnumResultCheckFile::READ_DENIED, "READ_DENIED"},
    {EnumResultCheckFile::TOO_LARGE, "TOO_LARGE"},
    {EnumResultCheckFile::UNSUPPORTED_CONTENT_TYPE, "UNSUPPORTED_CONTENT_TYPE"},
    {EnumResultCheckFile::OK, "OK"}
};

EnumResultCheckFile is_correct_file(string_view filepath);
void throw_if_not_correct_file(string_view filepath);

}


namespace std {
    std::string to_string(Utils::TGBotApi::File::EnumResultCheckFile result_check);
    std::string to_string(Utils::TGBotApi::File::EnumContentType conntent_type);
}