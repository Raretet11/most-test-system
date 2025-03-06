#pragma once

#include <string>
#include <vector>
#include "checker_components/problem.hpp"
#include "checker_components/submission_feedback.hpp"

namespace checker {

class AbstractChecker {
public:
    virtual std::vector<SubmissionFeedback> check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) = 0;

private:
    virtual std::string get_code_file_name() = 0;
    virtual std::string get_checker_name() = 0;
    virtual std::string get_output_file_name() = 0;
    virtual std::string get_error_file_name() = 0;

    virtual void generate_files(const std::string &solution) = 0;
    virtual SubmissionFeedback check_test(const Problem &problem) = 0;
};

}  // namespace checker
