#pragma once

#include <cstdint>
#include <string>

namespace checker {

struct Problem {
    std::string input, expected_output;
    std::uint32_t time_limit_ms = 0;
    std::uint32_t memory_limit_kb = 0;

    Problem() = default;
    Problem(
        const std::string &input_,
        const std::string &expected_output_,
        std::uint32_t time_limit_ms_ = 1000,
        std::uint32_t memory_limit_kb_ = 1024
    )
        : input(input_),
          expected_output(expected_output_),
          time_limit_ms(time_limit_ms_),
          memory_limit_kb(memory_limit_kb_){};
};

}  // namespace checker
