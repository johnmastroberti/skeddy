#pragma once
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "enums.hpp"

class ShiftSet {
 private:
  std::vector<std::string> m_roles;
  // std::vector<Shift> m_shifts;
  std::vector<std::vector<int>> m_personnel_per_role;
  std::unordered_map<std::string, std::string> m_skill_map;
  void set_skill_map();

 public:
  ShiftSet() = default;
  std::vector<std::string> const& roles() const;
  bool operator==(ShiftSet const& o) const = default;
  static std::optional<ShiftSet> get_shifts();
  int personnel_count(Shift shift, std::string_view role) const;
  std::string_view skill_for_role(std::string_view role) const;
};
