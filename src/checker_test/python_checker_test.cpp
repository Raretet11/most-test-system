#include "checker/python_checker.hpp"
#include <gtest/gtest.h>
#include <userver/utest/utest.hpp>
#include <vector>
#include "checker_components/execution_status.hpp"

UTEST(PythonSolutionChecker, BasicOkSolution) {
    auto checker = checker::PythonChecker();

    const std::string solution =
        "a, b = map(int, input().split())\nprint(a + b)";
    const std::vector<checker::Problem> problems = {
        checker::Problem("1 2", "3"),
        checker::Problem("100 100", "200"),
        checker::Problem("-1 1", "0"),
        checker::Problem("1000 -100", "900"),
        checker::Problem("-124 12152", "12028"),
        checker::Problem("0 12", "12"),
        checker::Problem("-5142124 -6125125", "-11267249"),
    };

    for (const auto &feedback : checker.check_solution(solution, problems)) {
        EXPECT_EQ(feedback.execution_status, checker::ExecutionStatus::kOK);
    }
}

UTEST(PythonSolutionChecker, BasicWrongAnswerSolution) {
    auto checker = checker::PythonChecker();

    const std::string solution =
        "a, b = map(int, input().split())\nprint(a + b + 1)";
    const std::vector<checker::Problem> problems = {
        checker::Problem("1 2", "3"),
        checker::Problem("100 100", "200"),
        checker::Problem("-1 1", "0"),
        checker::Problem("1000 -100", "900"),
        checker::Problem("-124 12152", "12028"),
        checker::Problem("0 12", "12"),
        checker::Problem("-5142124 -6125125", "-11267249"),
    };

    for (const auto &feedback : checker.check_solution(solution, problems)) {
        EXPECT_EQ(
            feedback.execution_status, checker::ExecutionStatus::kWrongAnswer
        );
    }
}
