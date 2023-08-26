#pragma once
#include <string>
#include <string_view>
#include <vector>

struct Table {
  std::vector<std::string> headers;
  std::vector<std::vector<std::string>> data;

  Table() = default;
};

Table read_csv(std::string_view filename);
