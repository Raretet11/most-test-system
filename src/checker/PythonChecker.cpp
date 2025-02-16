#include "PythonChecker.hpp"

#include <fstream>
#include <memory>
#include <iostream>
#include <array>
#include <string>

namespace checker {

CheckerResult PythonChecker::check_solution(const std::string &code, const std::vector<Problem> &problems) {
    save_code_to_file(code);

    bool ok = true;
    for (const auto &data : problems) {
        if (check_code_output(data) != CheckerResult::kOK) {
            ok = false;
        }
    }

    if (ok) return CheckerResult::kOK;
    return CheckerResult::kWrongAnswer;
};

std::string PythonChecker::get_checker_command() {
    std::string cmd = "python "; cmd += get_filename();
    return cmd;
};

std::string PythonChecker::get_filename() {
    return "script.py";
};

void PythonChecker::save_code_to_file(const std::string &code) {
    std::ofstream codeFile(get_filename());
    codeFile << code;
    codeFile.close();
};

std::string PythonChecker::execute_file(const std::string &inputData) {
    std::string command = get_checker_command() + " \"" + inputData + "\"";

    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    std::array<char, 128> buffer;
    std::string result;
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    result.pop_back();
    return result;
};

CheckerResult PythonChecker::check_code_output(const Problem &problem) {
    std::string code_result = execute_file(problem.input);
    if (code_result == problem.expected_output) {
        return CheckerResult::kOK;
    } return CheckerResult::kWrongAnswer;
};

}; // namespace checker
