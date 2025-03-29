#include "register_user.hpp"
#include <userver/components/component.hpp>
#include <userver/crypto/hash.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace most {

namespace {

class ApiHandler final : public userver::server::handlers::HttpHandlerBase {
public:
    static constexpr std::string_view kName = "handler-api-register-user";

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
        const auto &username = request.GetArg("username");
        const auto &password = request.GetArg("password");

        if (username.empty() || password.empty()) {
            throw userver::server::handlers::ClientError(
                userver::server::handlers::ExternalBody{
                    "Missing required fields"}
            );
        }

        const auto user_exists = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "SELECT EXISTS(SELECT 1 FROM most_db.users WHERE username = $1 OR "
            "email = $2)",
            username
        );

        if (user_exists.AsSingleRow<bool>()) {
            throw userver::server::handlers::ClientError(
                userver::server::handlers::ExternalBody{"User already exists"}
            );
        }

        const auto password_hash = userver::crypto::hash::Sha256(password);

        const auto new_user = pg_cluster_->Execute(
            userver::storages::postgres::ClusterHostType::kMaster,
            "INSERT INTO most_db.users "
            "(username, password_hash)"
            "VALUES ($1, $2)"
            "RETURNING id, username, email",
            username, password_hash
        );

        return "Done";
    }

private:
    userver::storages::postgres::ClusterPtr pg_cluster_;
};

}  // namespace

void append_user_api_handler_register_user(
    userver::components::ComponentList &component_list
) {
    component_list.Append<ApiHandler>();
};

}  // namespace most
