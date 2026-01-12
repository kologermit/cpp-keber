#pragma once

#include <string_view>
#include <map>
#include <exception>

namespace Utils::TGBotApi::File {

using std::string_view, std::string, std::map;
using std::exception;

enum ResultCheckFile {
    NOT_FOUND,
    READ_DENIED,
    TOO_LARGE,
    UNSUPPORTED_CONTENT_TYPE,
    OK
};

enum ContentType {
    UNKNOWN,
    PHOTO,
    VIDEO,
    AUDIO,
    DOCUMENT,
    TEXT,
};


const map<ContentType, string> convert_map_content_type = {
    {ContentType::PHOTO, "PHOTO"},
    {ContentType::VIDEO, "VIDEO"},
    {ContentType::AUDIO, "AUDIO"},
    {ContentType::DOCUMENT, "DOCUMENT"},
    {ContentType::TEXT, "TEXT"},
};

const map<ResultCheckFile, string> convert_map_result_check_file = {
    {ResultCheckFile::NOT_FOUND, "NOT_FOUND"},
    {ResultCheckFile::READ_DENIED, "READ_DENIED"},
    {ResultCheckFile::TOO_LARGE, "TOO_LARGE"},
    {ResultCheckFile::UNSUPPORTED_CONTENT_TYPE, "UNSUPPORTED_CONTENT_TYPE"},
    {ResultCheckFile::OK, "OK"}
};

ResultCheckFile is_correct_file(string_view filepath);
void throw_if_not_correct_file(string_view filepath);

}


namespace std {
    string to_string(Utils::TGBotApi::File::ResultCheckFile result_check);
    string to_string(Utils::TGBotApi::File::ContentType content_type);
}