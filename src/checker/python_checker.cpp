#include "python_checker.hpp"
#include <elf.h>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <userver/components/component.hpp>
#include <userver/engine/subprocess/environment_variables.hpp>
#include <userver/engine/subprocess/process_starter.hpp>
#include <vector>
#include "checker_components/execution_status.hpp"
#include "checker_components/submission_feedback.hpp"
#include "userver/engine/subprocess/child_process.hpp"
#include "userver/engine/subprocess/environment_variables.hpp"
#include "userver/engine/task/current_task.hpp"

namespace {

std::string read_from_file(const std::string &filename) {
    std::ifstream file(filename, std::ios::in);
    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    std::string result = ss.str();
    return result;
}

}  // namespace

namespace checker {

std::string PythonChecker::get_checker_name() const {
    std::string cmd = "./python.bash";
    return cmd;
};

std::string PythonChecker::get_code_file_name() const {
    return "script.py";
};

std::string PythonChecker::get_output_file_name() const {
    return "output.txt";
};

std::string PythonChecker::get_error_file_name() const {
    return "errors.txt";
};

void PythonChecker::generate_files(const std::string &code) const {
    std::vector<std::string> files_to_generate = {
        get_code_file_name(), get_error_file_name(), get_output_file_name()};
    for (const auto &file_name : files_to_generate) {
        std::ofstream file(file_name);
        if (file_name == get_code_file_name()) {
            file << code;
        }
        file.close();

        std::filesystem::permissions(
            file_name,
            std::filesystem::perms::owner_all |
                std::filesystem::perms::group_all |
                std::filesystem::perms::others_all,
            std::filesystem::perm_options::add
        );
    }
};

std::vector<SubmissionFeedback> PythonChecker::check_solution(
    const std::string &code,
    const std::vector<Problem> &problems
) const {
    generate_files(code);

    std::vector<SubmissionFeedback> testing_result;
    for (const auto &problem : problems) {
        testing_result.push_back(check_test(problem));
    }

    return testing_result;
};

SubmissionFeedback PythonChecker::check_test(const Problem &problem) const {
    std::vector<std::string> process_args = {
        get_code_file_name(),
        std::to_string(problem.time_limit_ms),
        std::to_string(problem.memory_limit_kb),
        get_error_file_name(),
        get_output_file_name(),
        problem.input};

    auto process = userver::engine::subprocess::ProcessStarter(
        userver::engine::current_task::GetTaskProcessor()
    );
    auto enviroment = userver::engine::subprocess::EnvironmentVariables({});

    auto info = process
                    .Exec(
                        get_checker_name(), process_args, enviroment,
                        get_output_file_name(), get_error_file_name()
                    )
                    .Get();

    long execution_time_ms = info.GetExecutionTime().count();
    int return_code = info.GetExitCode();

    std::string output = read_from_file(get_output_file_name());
    if (!output.empty() && output.back() == '\n') {
        output.pop_back();
    }

    std::string errors = read_from_file(get_error_file_name());

    SubmissionFeedback feedback;
    feedback.time_ms = execution_time_ms;
    feedback.memory_kb = 0;

    switch (return_code) {
        case static_cast<int>(ExecutionStatus::kOK):
            if (output == problem.expected_output) {
                feedback.execution_status = ExecutionStatus::kOK;
            } else {
                feedback.execution_status = ExecutionStatus::kWrongAnswer;
            }
            feedback.output = output;
            break;
        case static_cast<int>(ExecutionStatus::kCompilationError):
            feedback.execution_status = ExecutionStatus::kCompilationError;
            feedback.output = errors;
            break;
        case static_cast<int>(ExecutionStatus::kTimeLimit):
            feedback.execution_status = ExecutionStatus::kTimeLimit;
            break;
        case static_cast<int>(ExecutionStatus::kMemoryLimit):
            feedback.execution_status = ExecutionStatus::kMemoryLimit;
            break;
        default:
            feedback.execution_status = ExecutionStatus::kRuntimeError;
            feedback.output = errors;
            break;
    }

    return feedback;
}

};  // namespace checker
