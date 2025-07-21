#pragma once

#include <cstdint>
#include <type_traits>


template<std::size_t used_bits>
using min_unsigned_type = std::conditional_t<
    used_bits <=  8, uint8_t,
    std::conditional_t<
        used_bits <= 16, uint16_t,
        std::conditional_t<
            used_bits <= 32, uint32_t,
            std::conditional_t<
                used_bits <= 64, uint64_t,
                void // greater type is not supported, generates compilation error
            >
        >
    >
>;
