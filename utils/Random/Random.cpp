#pragma once

#include <utils/Type.cpp>
#include <random>
#include <ctime>

namespace Utils {
namespace Random {

using Utils::Type::const_int;

void init() {
    std::srand(std::time(0));
}

int rand_int(const_int start, const_int end) {
    return (std::rand() % (end - start)) + start;
}

}
}