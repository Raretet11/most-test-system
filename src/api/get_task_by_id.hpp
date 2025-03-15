#pragma once

#include <userver/components/component_list.hpp>

namespace most {

void append_task_api_handler_get_task_by_id_component(
    userver::components::ComponentList &component_list
);

}  // namespace most
