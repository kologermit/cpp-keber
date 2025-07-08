#pragma once

#include <utils/Type.cpp>
#include <random>
#include <ctime>

namespace Utils::Random {

void init() {
    std::srand(std::time(0));
}

int rand_int(int start, int end) {
    return (std::rand() % (end - start)) + start;
}

}