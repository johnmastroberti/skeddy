#pragma once

#include <optional>
#include <string>

inline std::optional<int> to_int(std::string const& s) {
  try {
    return std::stoi(s);
  } catch (...) {
    return {};
  }
}
