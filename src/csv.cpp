#include "csv.hpp"

#include <fstream>

#include "ranges.hpp"

std::string read_all(std::istream& file) {
  std::string ret;
  char c;
  while ((c = file.get()) != std::istream::traits_type::eof()) {
    ret.push_back(c);
  }
  return ret;
}

Table read_csv(std::string_view filename) {
  std::ifstream file(filename.data());
  if (!file.good()) return {};

  Table table;

  const auto all = read_all(file);
  auto lines = all | stdv::split('\n');
  const auto& first_line = *lines.begin();
  auto body = lines | stdv::drop(1);

  for (auto const& tok : first_line | stdv::split(','))
    table.headers.emplace_back(tok.begin(), tok.end());

  for (auto const& line : body) {
    table.data.emplace_back();
    for (auto const& tok : line | stdv::split(','))
      table.data.back().emplace_back(tok.begin(), tok.end());
  }

  return table;
}
