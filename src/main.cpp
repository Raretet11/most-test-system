#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include "api/get_all_solutions.hpp"
#include "api/get_all_tasks.hpp"
#include "api/get_task_by_id.hpp"
#include "api/get_tasks.hpp"
#include "api/load_task.hpp"
#include "api/submit_solution.hpp"

int main(int argc, char *argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::server::handlers::TestsControl>()
            .Append<userver::components::Postgres>("postgres-db-1")
            .Append<userver::clients::dns::Component>();

    most::append_task_api_handler_load_tasks_component(component_list);
    most::append_solution_api_handler_submit_solution_component(component_list);
    most::append_task_api_handler_get_all_tasks_component(component_list);
    most::append_task_api_handler_get_task_by_id_component(component_list);
    most::append_solution_api_handler_get_all_solutions_component(component_list
    );
    most::append_task_api_handler_get_tasks_component(component_list);

    return userver::utils::DaemonMain(argc, argv, component_list);
}
