#pragma once

#include <string>
#include <string_view>
#include <userver/components/component_list.hpp>
#include "../checker/AbstractChecker.hpp"

namespace most {

void append_task_submiter_component(userver::components::ComponentList &component_list);

}  // namespace most
