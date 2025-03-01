#pragma once

#include <string>
#include <vector>
#include "../problem.hpp"

namespace checker {

std::vector<Problem> decode_tests(const std::string &test);

};  // namespace checker