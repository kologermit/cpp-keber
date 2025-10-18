#pragma once

#include <httplib/httplib.h>
#include <vector>
#include <string>

namespace Utils::HTTP {

    using std::string;
    using httplib::Request;
    using std::vector;

    enum CheckType : int {
        LONG_LONG,
        LONG_DOUBLE,
        STRING,
        BOOL,
        FILE,
    };

    struct CheckParam {
        string name;
        CheckType type = CheckType::STRING;
        bool required = true;
    };

    void validate_params(const Request& request, const vector<CheckParam>& params);
}