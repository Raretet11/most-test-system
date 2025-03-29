#pragma once

namespace checker {

enum class ExecutionStatus {
    kOK = 0,
    kWrongAnswer = -1,
    kTimeLimit = 124,
    kMemoryLimit = 127,
    kCompilationError = 1,
    kRuntimeError = 2,
    kServerError = 400
};

}  // namespace checker
