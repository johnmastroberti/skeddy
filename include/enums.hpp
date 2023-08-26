#pragma once
#include <cstdint>

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
