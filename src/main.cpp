#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include "admin_component/load_task.hpp"
#include "hello.hpp"
#include "user_component/submit_solution.hpp"

int main(int argc, char *argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::server::handlers::TestsControl>();

    my_database::append_task_loader_component(component_list);
    my_database::AppendHello(component_list);
    my_database::append_task_submiter_component(component_list);

    return userver::utils::DaemonMain(argc, argv, component_list);
}
