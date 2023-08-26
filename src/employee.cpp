#include "employee.hpp"

#include "csv.hpp"

Employee::Employee(std::string name, std::uint8_t hrs,
                   std::span<SkillLevel> skills, std::span<Shift> reqs)
    : m_name(name),
      m_hours(hrs),
      m_skills(skills.begin(), skills.end()),
      m_requests_off(reqs.begin(), reqs.end()) {}

std::vector<Employee> get_employees() {
  auto table = read_csv("personnel.csv");

  std::vector<Employee> employees;

  return employees;
}
