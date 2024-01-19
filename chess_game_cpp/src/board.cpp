#include "position.hpp"
#include <iostream>

int main() {

  Position new_position;
  new_position.set_board_to_initial_configuration();
  new_position.print_board();
  std::cin.get();
  return 0;
}
