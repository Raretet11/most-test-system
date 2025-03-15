#include "get_task_by_id.hpp"
#include <fmt/format.h>
#include <string>
#include <tuple>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "userver/formats/json/value.hpp"
#include "userver/storages/postgres/io/row_types.hpp"
#include "userver/storages/postgres/result_set.hpp"

namespace most {

namespace {

class ApiHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-api-get-task-by-id";

    ApiHandler(
        const userver::components::ComponentConfig &config,
        const userver::components::ComponentContext &context
    )
        : HttpHandlerBase(config, context),
          pg_cluster_(
              context
                  .FindComponent<userver::components::Postgres>("postgres-db-1")
                  .GetCluster()
          ) {
    }

    std::string
    HandleRequestThrow(const userver::server::http::HttpRequest &request, userver::server::request::RequestContext &)
        const override {
        const auto task_id = request.GetPathArg("id");

        const auto result = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT id, name, tests, legend, time_limit, memory_limit FROM "
            "most_db.tasks "
            "WHERE id = $1",
            std::stoi(task_id)
        );

        const auto task = result.AsSingleRow<
            std::tuple<int, std::string, std::string, std::string, int, int>>(
            userver::storages::postgres::kRowTag
        );

        userver::formats::json::ValueBuilder json;
        json["id"] = std::get<0>(task);
        json["name"] = std::get<1>(task);
        json["tests"] = std::get<2>(task);
        json["legend"] = std::get<3>(task);
        json["time_limit"] = std::get<4>(task);
        json["memory_limit"] = std::get<5>(task);

        return userver::formats::json::ToString(json.ExtractValue());
    }

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_task_api_handler_get_task_by_id_component(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
}

}  // namespace most
