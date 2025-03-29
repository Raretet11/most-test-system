#include "submit_solution.hpp"
#include <fmt/format.h>
#include <cstdint>
#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include "checker/python_checker.hpp"
#include "components/execution_status.hpp"
#include "components/test_decoder.hpp"

namespace most {

namespace {

std::string execution_status_to_string(const checker::ExecutionStatus &status) {
    std::string result = "";
    switch (status) {
        case checker::ExecutionStatus::kOK:
            result = "Accepted!";
            break;
        case checker::ExecutionStatus::kWrongAnswer:
            result = "Wrong answer!";
            break;
        case checker::ExecutionStatus::kCompilationError:
            result = "Compilation Error";
            break;
        case checker::ExecutionStatus::kTimeLimit:
            result = "Time Limit exceeded";
            break;
        case checker::ExecutionStatus::kMemoryLimit:
            result = "Memory Limit exceeded";
            break;
        default:
            result = "Runtime Error";
            break;
    }
    return result;
}

class ApiHandler final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-api-submit-solution";

    ApiHandler(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &component_context
    )
        : HttpHandlerJsonBase(config, component_context),
          pg_cluster_(
              component_context
                  .FindComponent<userver::components::Postgres>("postgres-db-1")
                  .GetCluster()
          ) {
    }

    userver::formats::json::Value
    HandleRequestJsonThrow(const userver::server::http::HttpRequest &, const userver::formats::json::Value &json, userver::server::request::RequestContext &)
        const override {
        const auto &task_id = json["task_id"].As<int>();
        const auto &language = json["language"].As<std::string>();
        const auto &solution = json["solution"].As<std::string>();

        auto python_checker = checker::PythonChecker();

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT tests, time_limit, memory_limit FROM most_db.tasks WHERE "
            "id = ($1);",
            task_id
        );

        const auto task = result.AsSingleRow<std::tuple<std::string, int, int>>(
            userver::storages::postgres::kRowTag
        );

        auto tests = std::get<0>(task);
        std::uint32_t time_limit =
            static_cast<std::uint32_t>(std::get<1>(task));
        std::uint32_t memory_limit =
            static_cast<std::uint32_t>(std::get<2>(task));
        auto problems =
            checker::build_problem_set(tests, time_limit, memory_limit);
        auto check_res = python_checker.check_solution(solution, problems);

        std::string res;
        checker::ExecutionStatus status = checker::ExecutionStatus::kOK;
        std::uint32_t max_time_needed = 0;
        std::uint32_t last_test = 0;
        for (const auto &feedback : check_res) {
            max_time_needed = std::max(max_time_needed, feedback.time_ms);
            if (feedback.execution_status != checker::ExecutionStatus::kOK) {
                status = feedback.execution_status;
                break;
            }
            last_test++;
        }

        result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.solutions(task_id, language, code, "
            "time_limit, verdict, last_test) "
            "VALUES($1, $2, $3, $4, $5, $6)",
            task_id, language, solution, static_cast<int>(max_time_needed),
            execution_status_to_string(status), static_cast<int>(last_test)
        );

        std::string json_out =
            "{ \"verdict\": \"" + execution_status_to_string(status) + "\",\n" +
            "\"last_test\": " + std::to_string(last_test) +
            ",\n \"max_time_needed\":" + std::to_string(max_time_needed) + "}";
        return userver::formats::json::FromString(json_out);
    }

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_solution_api_handler_submit_solution_component(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
