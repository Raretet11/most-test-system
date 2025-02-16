#pragma once

#include <string>

namespace checker {

struct Problem {
    std::string input, expected_output;
    Problem() = default;
    Problem(const std::string &input_, const std::string &expected_output_)
        : input(input_), expected_output(expected_output_){};
};

}  // namespace checker