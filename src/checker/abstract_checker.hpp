#pragma once

#include <string>
#include <vector>
#include "../problem.hpp"

namespace checker {

enum class CheckerResult { kOK, kWrongAnswer, kTimeLimit, kPresentationError };

class AbstractChecker {
public:
    virtual CheckerResult check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) = 0;

private:
    virtual std::string get_filename() = 0;
    virtual std::string get_checker_command() = 0;

    virtual void save_code_to_file(const std::string &solution) = 0;
    virtual std::string execute_file(const std::string &input) = 0;

    virtual CheckerResult check_code_output(const Problem &problem) = 0;
};

}  // namespace checker
