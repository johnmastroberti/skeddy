#pragma once
#include <cstdint>
#include <string>

enum class SkillLevel : std::uint8_t {
  None,
  Beginner,
  Proficient,
  Expert,
};

enum class Shift : std::uint8_t {
  MondayMorning,
  MondayAfternoon,
  TuesdayMorning,
  TuesdayAfternoon,
  WednesdayMorning,
  WednesdayAfternoon,
  ThursdayMorning,
  ThursdayAfternoon,
  FridayMorning,
  FridayAfternoon,
};

inline std::string to_string(Shift s) {
  using enum Shift;
  switch (s) {
    case MondayMorning:
      return "Monday Morning";
    case MondayAfternoon:
      return "Monday Afternoon";
    case TuesdayMorning:
      return "Tuesday Morning";
    case TuesdayAfternoon:
      return "Tuesday Afternoon";
    case WednesdayMorning:
      return "Wednesday Morning";
    case WednesdayAfternoon:
      return "Wednesday Afternoon";
    case ThursdayMorning:
      return "Thursday Morning";
    case ThursdayAfternoon:
      return "Thursday Afternoon";
    case FridayMorning:
      return "Friday Morning";
    case FridayAfternoon:
      return "Friday Afternoon";
    default:
      return "";
  }
}
