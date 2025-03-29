#pragma once

#include <string>
#include <vector>
#include "components/problem.hpp"
#include "components/submission_feedback.hpp"

namespace checker {

class AbstractChecker {
public:
    virtual std::vector<SubmissionFeedback> check_solution(
        const std::string &solution,
        const std::vector<Problem> &problems
    ) const = 0;

private:
    virtual SubmissionFeedback check_test(const Problem &problem) const = 0;
};

}  // namespace checker
