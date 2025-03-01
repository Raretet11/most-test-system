#include "submit_solution.hpp"
#include <fmt/format.h>
#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include "../checker/python_checker.hpp"
#include "test_decoder.hpp"

namespace most {

namespace {

class ApiHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-user-submit-solution";

    ApiHandler(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &component_context
    )
        : HttpHandlerBase(config, component_context),
          pg_cluster_(
              component_context
                  .FindComponent<userver::components::Postgres>("postgres-db-1")
                  .GetCluster()
          ) {
    }

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        const auto &task_id = std::stoi(request.GetArg("task-id"));
        const auto &solution = request.GetArg("code");

        auto p = checker::PythonChecker();

        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT tests FROM most_db.tasks WHERE id = ($1);", task_id
        );

        auto tests = result.AsSingleRow<std::string>();
        auto inputData = checker::decode_tests(tests);
        auto check_res = p.check_solution(solution, inputData);

        std::string res = "OK";
        if (check_res != checker::CheckerResult::kOK) {
            res = "WRONG_ANSWER";
        }

        result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.solutions(task_id, language, code, verdict) "
            "VALUES($1, $2, $3, $4)",
            task_id, "python", solution, res
        );

        return res;
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
