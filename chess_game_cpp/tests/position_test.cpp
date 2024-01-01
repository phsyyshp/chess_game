#include "position.hpp"
#include <iostream>
int main() {
  Position new_pos;
  new_pos.set_board_to_initial_configuration();
  new_pos.print_board();
}