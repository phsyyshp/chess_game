#include "UCI.hpp"
void UCI::uci() const {
  std::cout << "id name M.A.R.T.A.: Machine Algorithm for Real-time Tactical "
               "Analysis\n";
  std::cout << "uciok\n";
}
void UCI::isready() const { std::cout << "readyok\n"; }

void UCI::go() const { std::cout << "going\n"; }
void UCI::loop() {

  std::string command;
  while (std::getline(std::cin, command)) {
    commands[command]();
  }
}