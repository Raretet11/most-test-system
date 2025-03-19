#pragma once

#include <userver/components/component_list.hpp>

namespace most {

void append_solution_api_handler_get_all_solutions_component(
    userver::components::ComponentList &component_list
);

}  // namespace most