#pragma once
#include "enums.hpp"
#include <span>
#include <string>
#include <vector>

class Employee {
private:
  std::string m_name;
  std::uint8_t m_hours = 40;
  std::vector<SkillLevel> m_skills;
  std::vector<Shift> m_requests_off;

public:
  Employee() = default;
  Employee(std::string name, std::uint8_t hrs, std::span<SkillLevel> skills,
           std::span<Shift> reqs);
  bool operator==(const Employee &) const = default;
};

std::vector<Employee> get_employees();
