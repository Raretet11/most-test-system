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

class UnloadAllSolution final : public HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-unload-all-solutions";

  UnloadAllSolution(const userver::components::ComponentConfig& config,
                        const userver::components::ComponentContext& context)
      : HttpHandlerBase(config, context),
        pg_cluster_(
            context.FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    
    // Создаем таблицу со всей инфой
    auto result = pg_cluster_->Execute(
        ClusterHostType::kMaster,
"SELECT most_db.solutions.id, "
    "most_db.solutions.task_id, "
    "most_db.tasks.name as task_name, "
    "most_db.solutions.language, "
    "most_db.solutions.code, "
    "most_db.solutions.verdict "
    "FROM most_db.solutions "
    "JOIN most_db.tasks "
    "ON most_db.solutions.task_id = most_db.tasks.id");

    // Строим JSON ответ
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

void UnloadAllSolutionsComponent(
    userver::components::ComponentList& component_list) {
  component_list.Append<UnloadAllSolution>();
}

}  // namespace most