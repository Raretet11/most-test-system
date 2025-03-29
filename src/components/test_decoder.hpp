#pragma once

#include <string>
#include <vector>
#include "components/problem.hpp"

namespace checker {

std::vector<Problem> build_problem_set(
    const std::string &test,
    std::uint32_t time_limit,
    std::uint32_t memory_limit = 1
);

};  // namespace checker