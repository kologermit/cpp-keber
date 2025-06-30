#pragma once

#include <cstdlib>
#include <random>
#include <ctime>

#include <fmt/core.h>


#define USING_FUNC(USING_NAME, FUNC_NAME)\
    constexpr auto USING_NAME = [](auto&&... args) { \
        return FUNC_NAME(std::forward<decltype(args)>(args)...); \
    }

#define USING_FUNC_T(USING_NAME, FUNC_NAME) \
    template<typename ...T> \
    auto USING_NAME = [](auto&&... args) { \
        return FUNC_NAME<T...>(std::forward<decltype(args)>(args)...); \
    };


namespace Utils {
namespace Functions {

// c
USING_FUNC(GetEnv, getenv);

// std
USING_FUNC(SRand, std::srand);
USING_FUNC(Rand, std::rand);
USING_FUNC(Time, std::time);
USING_FUNC_T(MakeShared, std::make_shared);

// fmt
USING_FUNC(Format, fmt::format);

}
}