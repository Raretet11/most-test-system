#pragma once

#include <string>
#include <cstdint>

namespace most {

struct User {
  std::int64_t id;
  std::string username;
  std::string email;
  std::string password_hash;
};

} // namespace most