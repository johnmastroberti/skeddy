#include "shifts.hpp"

#include "csv.hpp"
#include "ranges.hpp"

size_t shift_to_index(Shift shift);

std::optional<ShiftSet> ShiftSet::get_shifts() { return std::nullopt; }

int ShiftSet::personnel_count(Shift shift, std::string_view role) const {
  size_t shift_ix = shift_to_index(shift);
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
    default:  // TODO: better error handling?
      return 0;
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
