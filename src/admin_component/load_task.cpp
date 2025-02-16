#include "load_task.hpp"
#include "../checker/PythonChecker.hpp"
#include <fmt/format.h>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include <string>

namespace most {

namespace {

class ApiHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-admin-load-task";

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
        const auto &task_name = request.GetArg("name");
        const auto &tests = request.GetArg("tests");

        // TODO: validate
        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.tasks(name, tests) VALUES($1, $2)",
            task_name, tests
        );

        return "OK";
    }

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_task_loader_component(userver::components::ComponentList &component_list) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
