#pragma once
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "enums.hpp"

class Employee {
 private:
  std::string m_name;
  int m_hours = 40;
  std::unordered_map<std::string, int> m_skills;
  std::vector<Shift> m_requests_off;

 public:
  Employee() = default;
  Employee(std::string name, int hrs, std::span<std::string const> skill_names,
           std::span<int const> skills, std::span<Shift> reqs);
  bool operator==(const Employee&) const = default;
  inline std::string const& name() const { return m_name; }
  inline int skill_level(std::string const& sk) const {
    if (m_skills.contains(sk))
      return m_skills.at(sk);
    else
      return 0;
  }
  inline int hours() const { return m_hours; }
};

std::optional<std::vector<Employee>> get_employees();
