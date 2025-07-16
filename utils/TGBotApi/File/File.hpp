#include <string_view>

namespace Utils::TGBotApi::File {

using std::string_view;

enum ResultCheckFile {
    NOT_FOUND,
    READ_DENIED,
    TOO_LARGE,
    OK
};



ResultCheckFile is_correct_file(string_view filepath);
void throw_if_not_correct_file(string_view filepath);

}
namespace std {
    std::string to_string(Utils::TGBotApi::File::ResultCheckFile result_check);
}