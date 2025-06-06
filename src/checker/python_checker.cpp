#include "python_checker.hpp"
#include <elf.h>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <userver/components/component.hpp>
#include <userver/engine/subprocess/environment_variables.hpp>
#include <userver/engine/subprocess/process_starter.hpp>
#include <vector>
#include "checker_components/execution_status.hpp"
#include "checker_components/submission_feedback.hpp"
#include "userver/engine/subprocess/child_process.hpp"
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

std::vector<std::string> split(std::string s, const std::string &delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        tokens.push_back(token);
        s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(s);

    return tokens;
};

std::unordered_map<std::string, std::string> parse_meta_info(
    const std::string &meta_filename
) {
    std::string data = read_from_file(meta_filename);
    std::unordered_map<std::string, std::string> result;
    auto splited_data = split(data, "\n");
    for (std::size_t i = 0; i < splited_data.size(); i++) {
        auto cur = split(splited_data[i], ":");
        if (cur.size() < 2) {
            continue;
        }
        result[cur[0]] = cur[1];
    }
    return result;
}

}  // namespace

namespace checker {

std::string PythonChecker::get_checker_name() const {
    std::string cmd = "./python.bash";
    return cmd;
};

std::string PythonChecker::get_code_file_name() const {
    return std::to_string(_checker_index) + "-script.py";
};

std::string PythonChecker::get_test_file_name() const {
    return std::to_string(_checker_index) + "-test.txt";
};

std::string PythonChecker::get_output_file_name() const {
    return std::to_string(_checker_index) + "-output.txt";
};

std::string PythonChecker::get_meta_file_name() const {
    return std::to_string(_checker_index) + "-meta.txt";
};

std::string PythonChecker::get_error_file_name() const {
    return std::to_string(_checker_index) + "-errors.txt";
};

void PythonChecker::generate_files(const std::string &code) const {
    std::vector<std::string> files_to_generate = {
        get_code_file_name(), get_error_file_name(), get_output_file_name(),
        get_meta_file_name()
    };
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

void PythonChecker::generate_test(const Problem &problem) const {
    std::ofstream file(get_test_file_name());
    file << problem.input;
    file.close();

    std::filesystem::permissions(
        get_test_file_name(),
        std::filesystem::perms::owner_all | std::filesystem::perms::group_all |
            std::filesystem::perms::others_all,
        std::filesystem::perm_options::add
    );
}

void PythonChecker::delete_test() const {
    try {
        std::filesystem::remove(get_test_file_name());
    } catch (...) {
    }
}

void PythonChecker::delete_files() const {
    std::vector<std::string> files_to_delete = {
        get_code_file_name(), get_error_file_name(), get_output_file_name(),
        get_meta_file_name()
    };

    for (const auto &filename : files_to_delete) {
        try {
            std::filesystem::remove(filename);
        } catch (...) {
        }
    }
}

PythonChecker::PythonChecker() {
    _checker_index = PythonChecker::_global_checker_index;
    PythonChecker::_global_checker_index++;
}

std::vector<SubmissionFeedback> PythonChecker::check_solution(
    const std::string &code,
    const std::vector<Problem> &problems
) const {
    generate_files(code);

    std::vector<SubmissionFeedback> testing_result;
    for (const auto &problem : problems) {
        generate_test(problem);
        testing_result.push_back(check_test(problem));
        delete_test();
    }

    delete_files();
    return testing_result;
};

SubmissionFeedback PythonChecker::check_test(const Problem &problem) const {
    std::vector<std::string> process_args = {
        get_code_file_name(),
        std::to_string(problem.time_limit_ms),
        std::to_string(problem.memory_limit_kb),
        get_error_file_name(),
        get_output_file_name(),
        get_test_file_name(),
        get_meta_file_name()
    };

    auto process = userver::engine::subprocess::ProcessStarter(
        userver::engine::current_task::GetTaskProcessor()
    );
    auto options = userver::engine::subprocess::ExecOptions();

    [[maybe_unused]] auto result =
        process.Exec(get_checker_name(), process_args, std::move(options))
            .Get();

    auto parsed_data = parse_meta_info(get_meta_file_name());

    int return_code = 1;
    if (parsed_data.count("exitcode") != 0) {
        return_code = std::stoi(parsed_data["exitcode"]);
    }
    std::string errors = read_from_file(get_error_file_name());
    std::string output = read_from_file(get_output_file_name());
    if (output.size() > 0 && output.back() == '\n') {
        output.pop_back();
    }

    std::string status = "OK";

    if (return_code != 0) {
        status = parsed_data["status"];
    }

    SubmissionFeedback feedback;
    feedback.time_ms =
        static_cast<std::uint32_t>(std::stod(parsed_data["time"]) * 1000);
    feedback.memory_kb =
        static_cast<std::uint32_t>(std::stoi(parsed_data["max-rss"]));
    switch (return_code) {
        case 0:
            if (output == problem.expected_output) {
                feedback.execution_status = ExecutionStatus::kOK;
            } else {
                feedback.execution_status = ExecutionStatus::kWrongAnswer;
            }
            feedback.output = output;
            break;
        default:
            if (status == "RE") {
                if (errors.find("SyntaxError") != std::string::npos) {
                    feedback.execution_status =
                        ExecutionStatus::kCompilationError;
                } else {
                    feedback.execution_status = ExecutionStatus::kRuntimeError;
                }
                feedback.output = errors;
            } else if (status == "TO") {
                feedback.execution_status = ExecutionStatus::kTimeLimit;
                feedback.output = "Time Limit exceeded";
            } else if (status == "SG") {
                feedback.execution_status = ExecutionStatus::kCompilationError;
                feedback.output = errors;
            } else {
                feedback.execution_status = ExecutionStatus::kServerError;
                feedback.output = errors;
            }
            break;
    }
    return feedback;
}

};  // namespace checker
