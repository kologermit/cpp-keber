#pragma once

#include <string_view>

namespace Utils::Env {

using std::string;
using std::string_view;

string Get(string_view variable, string_view _default="");

}
