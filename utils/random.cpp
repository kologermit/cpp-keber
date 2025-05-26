#pragma once

#include <ctime>
#include <cstdlib>
#include <random>

namespace utils {

void init_random() {
    std::srand(std::time(0));
}

const int randint(const int start, const int end) {
    return (std::rand() % (end - start)) + start;
}

}