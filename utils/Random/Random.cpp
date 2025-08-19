#include <utils/Random/Random.hpp>
#include <random>
#include <ctime>
#include <cmath>

namespace Utils::Random {

using std::srand, std::rand, std::time;

void init() {
    std::srand((unsigned int)std::time(0));
}

int rand_int(int start, int end) {
    return (std::rand() % (abs(end - start) + 1)) + start;
}

}
