#include "loader.hpp"
#include <fstream>
#include <iostream>
#include <vector>
std::vector<uint64_t> line_to_nums_vec(const std::string &line) {

  std::vector<uint64_t> out;
  std::string number;
  for (auto c : line) {
    if (c != ' ') {
      number += c;
    } else {
      out.push_back(stoull(number));
      number.clear();
    }
  }
  return out;
}

std::vector<uint64_t> read_magic_numbers(std::string piece) {
  std::string file_name = piece + "_magic_numbers.txt";
  std::vector<uint64_t> out;
  std::string temp;
  std::fstream in(file_name);
  if (in) {
    while (getline(in, temp)) {
      out.push_back(stoull(temp));
    }
  } else {
    std::cerr << "the " << file_name << " can not be opened" << std::endl;
  }
  in.close();
  return out;
}
std::vector<std::vector<uint64_t>> read_look_up_tables(std::string piece) {

  std::string file_name = piece + "_look_up_tables.txt";
  std::vector<std::vector<uint64_t>> look_up_tables;
  std::string line;
  std::fstream in(file_name);
  std::vector<uint64_t> look_up_table;
  std::string number;
  if (in) {
    while (getline(in, line)) {
      look_up_table = line_to_nums_vec(line);
      look_up_tables.push_back(look_up_table);
    }
  } else {
    std::cerr << "the " << file_name << " can not be opened" << std::endl;
  }
  in.close();
  return look_up_tables;
}