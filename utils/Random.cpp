#pragma once

#include <ctime>
#include <cstdlib>
#include <random>
#include <utils/Types.cpp>

using Utils::Types::ConstInt;

namespace Utils {
namespace Random {

void init() {
    std::srand(std::time(0));
}

int randint(ConstInt start, ConstInt end) {
    return (std::rand() % (end - start)) + start;
}

}
}