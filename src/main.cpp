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
#include <map>
#include <ranges>
namespace stdr = std::ranges;
namespace stdv = std::ranges::views;

enum Role { RECEPTION, APPOINTMENTS, SURGERY, FLOAT };

int main() {
  auto shifts = stdv::iota(0, 10);
  auto employees = stdv::iota(0, 20);
  auto roles = stdv::iota(0, 4);

  ors::CpModelBuilder cp_model;

  // Create variables
  std::map<std::tuple<int, int, int>, ors::BoolVar> schedule;
  for (auto x : stdv::cartesian_product(shifts, employees, roles)) {
    auto [s, e, r] = x;
    schedule[x] = cp_model.NewBoolVar().WithName(
        std::format("shift={};emp={};role={}", s, e, r));
  }

  std::vector<ors::BoolVar> bools;
  bools.reserve(20);

  // At least one employee per role per shift
  for (auto s : shifts) {
    for (auto r : roles) {
      bools.clear();
      for (auto e : employees) {
        bools.push_back(schedule[std::tie(s, e, r)]);
      }
      cp_model.AddAtLeastOne(bools);
    }
  }

  // Each employee only works one role at a time
  for (auto e : employees) {
    for (auto s : shifts) {
      bools.clear();
      for (auto r : roles) {
        bools.push_back(schedule[std::tie(s, e, r)]);
      }
      cp_model.AddAtMostOne(bools);
    }
  }

  // Set up solver
  ors::Model model;
  ors::SatParameters parameters;
  parameters.set_linearization_level(0);
  parameters.set_enumerate_all_solutions(true);
  model.Add(ors::NewSatParameters(parameters));

  std::atomic<bool> stopped(false);
  model.GetOrCreate<operations_research::TimeLimit>()
      ->RegisterExternalBooleanAsLimit(&stopped);

  const int kSolutionLimit = 5;
  int num_solutions = 0;
  model.Add(
      ors::NewFeasibleSolutionObserver([&](const ors::CpSolverResponse &r) {
        fmt::print("Solution {}\n", num_solutions);
        fmt::print("Shift\t");
        for (auto ro : roles) {
          fmt::print("\tRole {}", ro);
        }

        for (auto s : shifts) {
          fmt::print("\nShift {}\t", s);
          for (auto ro : roles) {
            fmt::print("\t");
            for (auto e : employees) {
              if (ors::SolutionIntegerValue(r, schedule[std::tie(s, e, ro)]))
                fmt::print("{},", e);
            }
          }
        }
        fmt::print("\n----------------------\n");

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
