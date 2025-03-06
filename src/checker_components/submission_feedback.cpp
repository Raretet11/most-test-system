#include "checker_components/submission_feedback.hpp"
#include <string>

namespace checker {

std::string SubmissionFeedback::to_string() const {
    return "Result code: " + std::to_string(static_cast<int>(execution_status)) + '\n' +
           "Done in: " + std::to_string(time_ms) +
           " ms\nMemory used: " + std::to_string(memory_kb) +
           "\nWith output: " + output + '\n';
}

}  // namespace checker
