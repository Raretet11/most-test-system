#pragma once

#include <cstdint>
#include <string>
#include "execution_status.hpp"

namespace checker {

struct SubmissionFeedback {
    ExecutionStatus execution_status = ExecutionStatus::kOK;
    std::uint32_t time_ms = 0;
    std::uint32_t memory_kb = 0;
    std::string output;

    SubmissionFeedback() = default;
    SubmissionFeedback(
        const ExecutionStatus &result_,
        std::uint32_t time_ms_ = 0,
        std::uint32_t memory_kb_ = 0,
        const std::string &output_ = ""
    )
        : execution_status(result_),
          time_ms(time_ms_),
          memory_kb(memory_kb_),
          output(output_) {};
};

}  // namespace checker
