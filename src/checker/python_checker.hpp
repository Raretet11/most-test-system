#pragma once

#include "abstract_checker.hpp"

namespace checker {

class PythonChecker : AbstractChecker {
public:
    CheckerResult check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) final;

private:
    std::string get_filename() final;
    std::string get_checker_command() final;

    void save_code_to_file(const std::string &code) final;
    std::string execute_file(const std::string &input) final;

    CheckerResult check_code_output(const Problem &problem) final;
};

}  // namespace checker
