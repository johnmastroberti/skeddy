#include "absl/strings/str_format.h"
#include "ortools/base/logging.h"
#include "ortools/sat/cp_model.h"
#include "ortools/sat/cp_model.pb.h"
#include "ortools/sat/cp_model_solver.h"
#include "ortools/sat/model.h"
#include "ortools/sat/sat_parameters.pb.h"
#include "ortools/util/time_limit.h"
namespace ors = operations_research::sat;

#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

#include <atomic>
#include <format>
#include <fstream>
#include <map>
#include <ranges>

#include "employee.hpp"
#include "shifts.hpp"

namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

enum Role { RECEPTION, APPOINTMENTS, SURGERY, FLOAT };

int main() {
  const auto emp = get_employees();
  const auto shf = ShiftSet::get_shifts();
  if (!emp) {
    fmt::print("Failed to parse employees\n");
    return 1;
  }
  if (!shf) {
    fmt::print("Failed to parse shifts\n");
    return 1;
  }
  auto const& employees = *emp;
  auto const& shifts = *shf;
  using enum Shift;
  const std::vector<Shift> all_shifts = {
      MondayMorning,    MondayAfternoon,    TuesdayMorning,  TuesdayAfternoon,
      WednesdayMorning, WednesdayAfternoon, ThursdayMorning, ThursdayAfternoon,
      FridayMorning,    FridayAfternoon,
  };

  ors::CpModelBuilder cp_model;

  // Create variables
  std::map<std::tuple<Shift, std::string, std::string>, ors::BoolVar> schedule;
  for (auto const& x : stdv::cartesian_product(
           all_shifts,
           employees | stdv::transform([](auto const& e) { return e.name(); }),
           shifts.roles())) {
    auto const& [s, e, r] = x;
    schedule[x] = cp_model.NewBoolVar().WithName(
        std::format("shift={};emp={};role={}", to_string(s), e, r));
  }

  std::vector<ors::BoolVar> bools;
  bools.reserve(20);

  // For each shift and role, set the total number of employees
  // assigned to the role to be at least the required number
  for (auto s : all_shifts) {
    for (auto const& r : shifts.roles()) {
      const auto N_required = shifts.personnel_count(s, r);
      fmt::print("{}, {} = {}\n", to_string(s), r, N_required);
      ors::LinearExpr sum;
      for (auto const& e : employees) sum += schedule[std::tie(s, e.name(), r)];
      cp_model.AddGreaterOrEqual(sum, N_required);
    }
  }

  // For each employee, assign the requested number of hours
  for (auto const& e : employees) {
    const auto N_hours = e.hours();
    ors::LinearExpr sum;
    for (auto s : all_shifts)
      for (auto r : shifts.roles()) sum += schedule[std::tie(s, e.name(), r)];
    cp_model.AddEquality(sum, N_hours / 5);
  }

  // Don't assign the same employee to two roles at the same time
  for (auto const& e : employees) {
    for (auto s : all_shifts) {
      bools.clear();
      for (auto r : shifts.roles())
        bools.push_back(schedule[std::tie(s, e.name(), r)]);
      cp_model.AddAtMostOne(bools);
    }
  }

  // // At least one employee per role per shift
  // for (auto s : shifts) {
  //   for (auto r : roles) {
  //     bools.clear();
  //     for (auto e : employees) {
  //       bools.push_back(schedule[std::tie(s, e, r)]);
  //     }
  //     cp_model.AddAtLeastOne(bools);
  //   }
  // }
  //
  // // Each employee only works one role at a time
  // for (auto e : employees) {
  //   for (auto s : shifts) {
  //     bools.clear();
  //     for (auto r : roles) {
  //       bools.push_back(schedule[std::tie(s, e, r)]);
  //     }
  //     cp_model.AddAtMostOne(bools);
  //   }
  // }

  // Set up solver
  ors::Model model;
  ors::SatParameters parameters;
  parameters.set_linearization_level(0);
  parameters.set_enumerate_all_solutions(true);
  auto p = ors::NewSatParameters(parameters);
  model.Add(p);

  std::atomic<bool> stopped(false);
  model.GetOrCreate<operations_research::TimeLimit>()
      ->RegisterExternalBooleanAsLimit(&stopped);

  const int kSolutionLimit = 1;
  int num_solutions = 0;
  model.Add(
      ors::NewFeasibleSolutionObserver([&](const ors::CpSolverResponse& r) {
        std::string csv_out;
        fmt::print("Solution {}\n", num_solutions);

        csv_out += "Shift,";
        for (auto ro : shifts.roles()) {
          csv_out += ro;
          csv_out += ",";
        }

        for (auto s : all_shifts) {
          csv_out += "\n";
          csv_out += to_string(s);
          for (auto ro : shifts.roles()) {
            csv_out += ",";
            for (auto const& e : employees) {
              if (ors::SolutionIntegerValue(
                      r, schedule[std::tie(s, e.name(), ro)])) {
                csv_out += e.name();
                csv_out += " ";
              }
            }
          }
        }
        fmt::print("{}", csv_out);
        fmt::print("\n----------------------\n");
        std::ofstream out("result.csv");
        out << csv_out;

        num_solutions++;
        if (num_solutions >= kSolutionLimit) {
          stopped = true;
        }
      }));

  auto resp = ors::SolveCpModel(cp_model.Build(), &model);
  fmt::print("Statistics\n");
  fmt::print("{}\n", ors::CpSolverResponseStats(resp));
  return 0;
}
