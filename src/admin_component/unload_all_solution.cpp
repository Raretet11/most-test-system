#include "unload_all_solution.hpp"

#include <fmt/format.h>
#include <userver/clients/dns/component.hpp>
#include <userver/components/component.hpp>
#include <userver/formats/json.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace most {

namespace {

using userver::formats::json::ValueBuilder;
using userver::server::handlers::HttpHandlerBase;
using userver::storages::postgres::ClusterHostType;

class GetAllSolutionsHandler final : public HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-all-solutions";

  GetAllSolutionsHandler(const userver::components::ComponentConfig& config,
                        const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(
            context.FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    
    // Получаем все решения с полной информацией
    auto result = pg_cluster_->Execute(
        ClusterHostType::kMaster,
        "SELECT s.id, s.task_id, t.name as task_name, s.language, "
        "s.code, s.verdict "
        "FROM most_db.solutions s "
        "JOIN most_db.tasks t ON s.task_id = t.id");

    // Строим JSON-ответ
    ValueBuilder json;
    for (const auto& row : result) {
      ValueBuilder solution;
      solution["id"] = row["id"].As<int>();
      solution["task_id"] = row["task_id"].As<int>();
      solution["task_name"] = row["task_name"].As<std::string>();
      solution["language"] = row["language"].As<std::string>();
      solution["code"] = row["code"].As<std::string>();
      solution["verdict"] = row["verdict"].As<std::string>();

      json.PushBack(std::move(solution));
    }

    return ToString(json.ExtractValue());
  }

 private:
  userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void AppendGetAllSolutionsComponent(
    userver::components::ComponentList& component_list) {
  component_list.Append<GetAllSolutionsHandler>();
}

}  // namespace most