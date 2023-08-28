#include "employee.hpp"

#include <cassert>
#include <fstream>

#include "csv.hpp"

std::vector<Employee> parse_employees(Table const& table);

Employee::Employee(std::string name, std::uint8_t hrs,
                   std::span<SkillLevel> skills, std::span<Shift> reqs)
    : m_name(name),
      m_hours(hrs),
      m_skills(skills.begin(), skills.end()),
      m_requests_off(reqs.begin(), reqs.end()) {}

std::vector<Employee> get_employees() {
  std::ifstream file("personnel.csv");
  assert(file.good());
  const auto table = read_csv(file);

  return parse_employees(table);
}

std::vector<Employee> parse_employees(Table const& table) {
  // Determine column indexes
  const auto name = get_table_col("Employee");
  const auto hrs = get_table_col("Target Hours");
