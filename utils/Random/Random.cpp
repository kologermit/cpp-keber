#include <utils/Random/Random.hpp>
#include <random>
#include <ctime>

namespace Utils::Random {

using std::srand, std::rand, std::time;

void init() {
    std::srand((unsigned int)std::time(0));
}

int rand_int(int start, int end) {
    return (std::rand() % (end - start)) + start;
}

}
