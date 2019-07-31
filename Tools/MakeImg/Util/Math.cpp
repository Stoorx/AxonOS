//
// Created by cognitio on 13.07.2019.
//

#include "Math.hpp"

bool Math::isPowerOfTwo(uint32_t value) {
    return value && !(value & (value - 1));
}
