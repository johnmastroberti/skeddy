#include "employee.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>

#include "csv.hpp"
#include "doctest.hpp"
#include "ranges.hpp"
#include "util.hpp"

std::optional<std::vector<Employee>> parse_employees(Table const& table);
bool check_employee_table_preconditions(Table const& table);

Employee::Employee(std::string name, std::uint8_t hrs,
                   std::span<SkillLevel> skills, std::span<Shift> reqs)
    : m_name(name),
      m_hours(hrs),
      m_skills(skills.begin(), skills.end()),
      m_requests_off(reqs.begin(), reqs.end()) {}

std::optional<std::vector<Employee>> get_employees() {
  std::ifstream file("personnel.csv");
  if (!file.good()) return {};
  const auto table = read_csv(file);
  return parse_employees(table);
}

std::optional<std::vector<Employee>> parse_employees(Table const& table) {
  if (!check_employee_table_preconditions(table)) return std::nullopt;

  std::vector<Employee> v;
  std::vector<std::optional<int>> skill_ints;
  std::vector<SkillLevel> skills;
  std::vector<Shift> empty_shifts;
  auto is_null = [](auto o) { return o == std::nullopt; };
  for (auto const& row : table.data) {
    skill_ints.resize(row.size() - 2);
    skills.resize(row.size() - 2);
    auto const& name = row[0];
    auto hrs = to_int(row[1]);
    if (!hrs) return std::nullopt;
    std::transform(row.begin() + 2, row.end(), skill_ints.begin(), to_int);
    if (stdr::any_of(skill_ints, is_null)) return std::nullopt;
    std::transform(skill_ints.begin(), skill_ints.end(), skills.begin(),
                   [](auto const& o) { return static_cast<SkillLevel>(*o); });
    v.emplace_back(name, *hrs, skills, empty_shifts);
  }
  return v;
}

bool check_employee_table_preconditions(Table const& table) {
  if (table.headers.empty()) return false;
  if (table.data.empty()) return false;
  if (table.headers.size() < 3) return false;
  if (table.headers[0] != "Employee") return false;
  if (table.headers[1] != "Target Hours") return false;
  const auto N = table.headers.size();
  if (stdr::any_of(table.data,
                   [N](auto const& row) { return row.size() != N; }))
    return false;
  return true;
}

/* parse_employees tests {{{*/
DOCTEST_TEST_CASE("Employee parsing tests") {
  using namespace std::literals;
  using std::nullopt;
  Table table;
  DOCTEST_SUBCASE("Failed preconditions") {
    DOCTEST_CHECK(parse_employees(table) == nullopt);
    table.headers = std::vector{"Hours"s};
    DOCTEST_CHECK(parse_employees(table) == nullopt);
    table.headers = std::vector{"Employee"s, "Target Hours"s};
    DOCTEST_CHECK(parse_employees(table) == nullopt);
    table.headers.push_back("Skill"s);
    DOCTEST_CHECK(parse_employees(table) == nullopt);
    table.data.emplace_back(std::vector{"Joe"s, "40"s, "1"s, "2"s});
    DOCTEST_CHECK(parse_employees(table) == nullopt);
  }
  DOCTEST_SUBCASE("Failure to parse integers") {
    table.headers = std::vector{"Employee"s, "Target Hours"s, "Skill"s};
    table.data.emplace_back(std::vector{"Joe"s, "apple"s, "1"s});
    DOCTEST_CHECK(parse_employees(table) == nullopt);
    table.data.back()[1] = "40"s;
    table.data.back()[2] = "apple"s;
    DOCTEST_CHECK(parse_employees(table) == nullopt);
  }
  DOCTEST_SUBCASE("Successful parse") {
    table.headers = std::vector{"Employee"s, "Target Hours"s, "Skill"s};
    table.data.emplace_back(std::vector{"Joe"s, "30"s, "1"s});
    table.data.emplace_back(std::vector{"Mary"s, "40"s, "2"s});
    auto ret = parse_employees(table);
    DOCTEST_CHECK(ret != nullopt);
    auto v = *ret;
    DOCTEST_CHECK(v.size() == 2);
    auto skills = std::vector{SkillLevel{1}};
    auto shifts = std::vector<Shift>{};
    DOCTEST_CHECK(v[0] == Employee("Joe"s, 30, skills, shifts));
    skills[0] = SkillLevel{2};
    DOCTEST_CHECK(v[1] == Employee("Mary"s, 40, skills, shifts));
  }
} /*}}}*/
