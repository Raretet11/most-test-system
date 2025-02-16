#pragma once

#include <vector>
#include <string>
#include "../problem.hpp"

namespace checker {

std::vector<Problem> decode_tests(const std::string &test);

}; // namespace checker