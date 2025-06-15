#pragma once

#include "abstract_checker.hpp"
#include "checker_components/problem.hpp"
#include "checker_components/submission_feedback.hpp"

namespace checker {

class PythonChecker : public AbstractChecker {
public:
    PythonChecker();

    [[nodiscard]] std::vector<SubmissionFeedback> check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) const override;

private:
    inline static std::size_t _global_checker_index = 0;
    std::size_t _checker_index;

    std::string get_code_file_name() const;
    std::string get_checker_name() const;
    std::string get_output_file_name() const;
    std::string get_error_file_name() const;
    std::string get_meta_file_name() const;
    std::string get_test_file_name() const;

    void generate_test(const Problem &problem) const;
    void generate_files(const std::string &solution) const;
    void delete_files() const;
    void delete_test() const;
    SubmissionFeedback check_test(const Problem &problem) const override;
};

}  // namespace checker
