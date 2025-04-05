#include "get_tasks.hpp"
#include <fmt/format.h>
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
    static constexpr std::string_view kName = "handler-api-get-tasks";

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
    HandleRequestJsonThrow(const userver::server::http::HttpRequest &, const userver::formats::json::Value &, userver::server::request::RequestContext &)
        const override {
        auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT * FROM most_db.tasks;"
        );

        userver::formats::json::ValueBuilder json_builder;
        for (const auto &row : result) {
            userver::formats::json::ValueBuilder task_json;
            task_json["id"] = row["id"].As<int>();
            task_json["name"] = row["name"].As<std::string>();
            task_json["tests"] = row["tests"].As<std::string>();
            task_json["legend"] = row["legend"].As<std::string>();
            task_json["time_limit"] = row["time_limit"].As<int>();
            task_json["memory_limit"] = row["memory_limit"].As<int>();
            json_builder.PushBack(task_json.ExtractValue());
        }

        return json_builder.ExtractValue();
    }

    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_task_api_handler_get_tasks_component(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
