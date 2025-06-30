#pragma once

#include <utils/Types.cpp>
#include <utils/Functions.cpp>

namespace Utils {
namespace Random {

using Utils::Types::ConstInt;
using Utils::Functions::SRand;
using Utils::Functions::Rand;
using Utils::Functions::Time;

void init() {
    SRand(Time(0));
}

int randint(ConstInt start, ConstInt end) {
    return (Rand() % (end - start)) + start;
}

}
}