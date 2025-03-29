#include "test_decoder.hpp"
#include <cstdint>

namespace checker {

namespace {

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

}  // namespace

std::vector<Problem> build_problem_set(
    const std::string &test,
    std::uint32_t time_limit,
    std::uint32_t memory_limit
) {
    std::vector<Problem> res;

    auto splited_tests = split(test, "|");

    for (size_t i = 0; i < splited_tests.size(); i++) {
        auto cur = split(splited_tests[i], "#");
        res.emplace_back(cur[0], cur[1], time_limit, memory_limit);
    }
    return res;
}

};  // namespace checker
