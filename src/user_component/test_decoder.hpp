#pragma once

#include <string>
#include <vector>
#include "checker_components/problem.hpp"

namespace checker {

std::vector<Problem> decode_tests(const std::string &test);

};  // namespace checker