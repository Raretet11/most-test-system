#pragma once

#include "abstract_checker.hpp"
#include "checker_components/submission_feedback.hpp"

namespace checker {

class PythonChecker : public AbstractChecker {
public:
    [[nodiscard]] std::vector<SubmissionFeedback> check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) const override;

private:
    std::string get_code_file_name() const;
    std::string get_checker_name() const;
    std::string get_output_file_name() const;
    std::string get_error_file_name() const;

    void generate_files(const std::string &solution) const;
    SubmissionFeedback check_test(const Problem &problem) const override;
};

}  // namespace checker
