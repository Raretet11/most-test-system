#pragma once

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>
#include "../checker/AbstractChecker.hpp"

namespace my_database {

void append_task_loader_component(userver::components::ComponentList &component_list);

}  // namespace my_database
