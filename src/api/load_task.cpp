#include "load_task.hpp"
#include <fmt/format.h>
#include <cstdint>
#include <string>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "userver/formats/json/value.hpp"

namespace most {

namespace {

class ApiHandler final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "handler-api-load-task";

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
        const auto &task_name = json["name"].As<std::string>();
        const auto &tests = json["tests"].As<std::string>();
        const auto &time_limit = json["time_limit"].As<int>();
        const auto &memory_limit = json["memory_limit"].As<int>();
        const auto &legend = json["legend"].As<std::string>();

        // TODO: validate
        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.tasks(name, tests, legend, time_limit, "
            "memory_limit) "
            "VALUES($1, $2, $3, $4, $5)",
            task_name, tests, legend, time_limit, memory_limit
        );

        std::string out = "{ \"result\": \"" + task_name + " " + tests + " " +
                          std::to_string(time_limit) + " " +
                          std::to_string(memory_limit) + " " + legend + "\"}";
        return userver::formats::json::FromString(out);
    }

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_task_api_handler_load_tasks_component(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
