#include "csv.hpp"

#include <algorithm>
#include <fstream>
#include <sstream>

#include "doctest.hpp"
#include "ranges.hpp"

std::string read_all(std::istream& file) {
  std::string ret;
  char c;
  while ((c = file.get()) != std::istream::traits_type::eof()) {
    ret.push_back(c);
  }
  return ret;
}

// read_all tests {{{
DOCTEST_TEST_CASE("read_all should read all contents from an istream") {
  std::stringstream test_stream;
  std::string contents;

  DOCTEST_SUBCASE("Empty file") {
    contents = "";
    test_stream.str(contents);
    auto ret = read_all(test_stream);
    DOCTEST_CHECK(ret == contents);
    DOCTEST_CHECK(test_stream.eof());
  }
  DOCTEST_SUBCASE("One line file") {
    contents = "test test test";
    test_stream.str(contents);
    auto ret = read_all(test_stream);
    DOCTEST_CHECK(ret == contents);
    DOCTEST_CHECK(test_stream.eof());
  }
  DOCTEST_SUBCASE("Many line file") {
    contents = "test\ntest\ntest\ntest\r\n";
    test_stream.str(contents);
    auto ret = read_all(test_stream);
    DOCTEST_CHECK(ret == contents);
    DOCTEST_CHECK(test_stream.eof());
  }
}
// }}}

Table read_csv(std::istream& file) {
  Table table;

  const auto all = read_all(file);
  auto lines = all | stdv::split('\n');
  const auto& first_line = *lines.begin();
  auto body = lines | stdv::drop(1);

  for (auto const& tok : first_line | stdv::split(','))
    table.headers.emplace_back(tok.begin(), tok.end());

  auto& data = table.data;
  for (auto const& line : body) {
    data.emplace_back();
    for (auto const& tok : line | stdv::split(','))
      data.back().emplace_back(tok.begin(), tok.end());
  }

  // Remove empty rows
  std::erase_if(data, [](auto const& row) { return row.empty(); });

  return table;
}

// read_csv tests {{{
using namespace std::literals;
DOCTEST_TEST_CASE("csv parsing") {
  std::stringstream file;

  DOCTEST_SUBCASE("Empty file") {
    file.str("");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret == Table{});
  }

  DOCTEST_SUBCASE("File with one header") {
    file.str("col1");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret.headers == std::vector{"col1"s});
    DOCTEST_CHECK(ret.data.size() == 0);
  }

  DOCTEST_SUBCASE("File with only headers") {
    file.str("col1,col2,col3");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret.headers == std::vector{"col1"s, "col2"s, "col3"s});
    DOCTEST_CHECK(ret.data.size() == 0);
  }

  DOCTEST_SUBCASE("File with only headers and newline") {
    file.str("col1,col2,col3\n");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret.headers == std::vector{"col1"s, "col2"s, "col3"s});
    DOCTEST_CHECK(ret.data.size() == 0);
  }

  DOCTEST_SUBCASE("File with headers and contents") {
    file.str("col1,col2,col3\ndata1,data2,data3\ndata4,data5,data6");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret.headers == std::vector{"col1"s, "col2"s, "col3"s});
    DOCTEST_CHECK(ret.data.size() == 2);
    DOCTEST_CHECK(ret.data[0] == std::vector{"data1"s, "data2"s, "data3"s});
    DOCTEST_CHECK(ret.data[1] == std::vector{"data4"s, "data5"s, "data6"s});
  }
  DOCTEST_SUBCASE("File with headers and contents (and trailing newline)") {
    file.str("col1,col2,col3\ndata1,data2,data3\ndata4,data5,data6\n");
    auto ret = read_csv(file);
    DOCTEST_CHECK(ret.headers == std::vector{"col1"s, "col2"s, "col3"s});
    DOCTEST_CHECK(ret.data.size() == 2);
    DOCTEST_CHECK(ret.data[0] == std::vector{"data1"s, "data2"s, "data3"s});
    DOCTEST_CHECK(ret.data[1] == std::vector{"data4"s, "data5"s, "data6"s});
  }
}
// }}}
