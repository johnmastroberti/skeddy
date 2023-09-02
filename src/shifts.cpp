#include "shifts.hpp"

#include <fstream>

#include "csv.hpp"
#include "ranges.hpp"
#include "util.hpp"

size_t shift_to_index(Shift shift);
bool check_shift_table_preconditions(Table const&);

std::optional<ShiftSet> ShiftSet::get_shifts() {
  std::ifstream shift_csv("shifts.csv");
  if (!shift_csv.good()) return std::nullopt;
  const auto table = read_csv(shift_csv);
  if (!check_shift_table_preconditions(table)) return std::nullopt;

  ShiftSet shifts;
  shifts.m_roles =
      std::vector<std::string>(table.headers.begin() + 1, table.headers.end());

  // Assume rows are ordered Monday-Friday
  std::vector<std::optional<int>> personnel;
  std::vector<int> personnel_ints;
  auto unwrap = [](auto const& opt) { return *opt; };
  for (auto const& row : table.data) {
    personnel.clear();
    personnel_ints.clear();
    personnel.reserve(row.size());
    personnel_ints.reserve(row.size());
    std::transform(row.begin() + 1, row.end(), std::back_inserter(personnel),
                   to_int);

    if (stdr::any_of(personnel, [](auto o) { return o == std::nullopt; }))
      return std::nullopt;
    std::transform(personnel.begin(), personnel.end(),
                   std::back_inserter(personnel_ints), unwrap);
    shifts.m_personnel_per_role.emplace_back(std::move(personnel_ints));
  }

  shifts.set_skill_map();
  return shifts;
}

bool check_shift_table_preconditions(Table const& t) {
  if (t.headers.empty()) return false;
  if (t.data.size() != 5) return false;
  const auto N = t.headers.size();
  if (N < 2ul) return false;
  auto same_size = [N](auto const& row) { return row.size() == N; };
  if (!stdr::all_of(t.data, same_size)) return false;
  return true;
}

int ShiftSet::personnel_count(Shift shift, std::string_view role) const {
  size_t shift_ix = shift_to_index(shift);
  if (shift_ix == size_t(-1)) return 0;
  auto role_it = stdr::find(m_roles, role);
  if (role_it == m_roles.end()) return 0;
  size_t role_ix = role_it - m_roles.begin();

  return m_personnel_per_role[shift_ix][role_ix];
}

size_t shift_to_index(Shift shift) {
  using enum Shift;
  switch (shift) {
    case MondayMorning:
    case MondayAfternoon:
      return 0;
    case TuesdayMorning:
    case TuesdayAfternoon:
      return 1;
    case WednesdayMorning:
    case WednesdayAfternoon:
      return 2;
    case ThursdayMorning:
    case ThursdayAfternoon:
      return 3;
    case FridayMorning:
    case FridayAfternoon:
      return 4;
    default:
      return size_t(-1);
  }
}

std::string_view ShiftSet::skill_for_role(std::string_view role) const {
  const auto k = std::string(role);
  if (m_skill_map.contains(k))
    return m_skill_map.at(k);
  else
    return {};
}

void ShiftSet::set_skill_map() {
  // TODO: hardcoded for now, need to be more flexible
  m_skill_map["Appointments"] = "Appointments";
  m_skill_map["Float"] = "";
  m_skill_map["OR"] = "Surgery";
  m_skill_map["Procedures"] = "Surgery";
  m_skill_map["Reception"] = "Reception";
  m_skill_map["Shift Manager"] = "Shift Manager";
  m_skill_map["Office"] = "Shift Manager";
}

std::vector<std::string> const& ShiftSet::roles() const { return m_roles; }
