#pragma once
#include <istream>
#include <string>
#include <string_view>
#include <vector>

struct Table {
  std::vector<std::string> headers;
  std::vector<std::vector<std::string>> data;

  Table() = default;
  bool operator==(Table const&) const = default;
};

Table read_csv(std::istream& file);
