#pragma once

#include <string>
#include "execution_status.hpp"

namespace checker {

struct SubmissionFeedback {
    ExecutionStatus execution_status;
    long time_ms{};
    long memory_kb{};
    std::string output;

    SubmissionFeedback() = default;
    SubmissionFeedback(
        const ExecutionStatus &result_,
        long time_ms_ = -1,
        long memory_kb_ = -1,
        std::string output_ = ""
    )
        : execution_status(result_),
          time_ms(time_ms_),
          memory_kb(memory_kb_),
          output(output_){};

    std::string to_string() const;
};

}  // namespace checker
