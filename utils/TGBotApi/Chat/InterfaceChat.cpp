#pragma once

#include <utils/Type.cpp>

namespace Utils {
namespace TGBotApi {
namespace Chat {

using namespace Type;

struct InterfaceChat {

virtual int get_id() const = 0;
virtual const_string& get_name() const = 0;

};

}
}
}