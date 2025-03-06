#pragma once

#include "abstract_checker.hpp"
#include "checker_components/submission_feedback.hpp"

namespace checker {

class PythonChecker : AbstractChecker {
public:
    std::vector<SubmissionFeedback> check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) final;

private:
    std::string get_code_file_name() final;
    std::string get_checker_name() final;
    std::string get_output_file_name() final;
    std::string get_error_file_name() final;

    void generate_files(const std::string &solution) final;
    SubmissionFeedback check_test(const Problem &problem) final;
};

}  // namespace checker
