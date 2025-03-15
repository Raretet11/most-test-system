#include "submit_solution.hpp"
#include <fmt/format.h>
#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include "checker/python_checker.hpp"
#include "checker_components/execution_status.hpp"
#include "checker_components/test_decoder.hpp"

namespace most {

namespace {

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

        auto p = checker::PythonChecker();

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT tests FROM most_db.tasks WHERE id = ($1);", task_id
        );

        auto tests = result.AsSingleRow<std::string>();
        auto inputData = checker::decode_tests(tests);
        auto check_res = p.check_solution(solution, inputData);

        std::string res;
        checker::ExecutionStatus status = checker::ExecutionStatus::kOK;
        int max_time_needed = 0;
        for (const auto &feedback : check_res) {
            max_time_needed =
                std::max(max_time_needed, static_cast<int>(feedback.time_ms));
            if (feedback.execution_status != checker::ExecutionStatus::kOK) {
                status = feedback.execution_status;
                break;
            }
        }

        result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.solutions(task_id, language, code, "
            "time_limit, verdict) "
            "VALUES($1, $2, $3, $4, $5)",
            task_id, language, solution, max_time_needed,
            std::to_string(static_cast<int>(status))
        );

        std::string json_out =
            "{ \"verdict\":" + std::to_string(static_cast<int>(status)) +
            ",\n \"max_time_needed\":" + std::to_string(max_time_needed) + "}";
        return userver::formats::json::FromString(json_out);
    }

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_task_submiter_component(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
