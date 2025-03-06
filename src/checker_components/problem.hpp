#pragma once

#include <string>

namespace checker {

struct Problem {
    std::string input, expected_output;
    long time_limit_ms{}, memory_limit_kb{};

    Problem() = default;
    Problem(
        const std::string &input_,
        const std::string &expected_output_,
        long time_limit_ms_ = 1000,
        long memory_limit_kb_ = 1024
    )
        : input(input_),
          expected_output(expected_output_),
          time_limit_ms(time_limit_ms_),
          memory_limit_kb(memory_limit_kb_){};
};

}  // namespace checker
