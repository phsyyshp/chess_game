#include "UCI.hpp"
void UCI::uci(const std::vector<std::string> &subCommands) const {
  std::cout << "id name  Machine Algorithm for Real-time Tactical "
               "Analysis\n";
  std::cout << "uciok\n";
}
void UCI::isready(const std::vector<std::string> &subCommands) const {
  std::cout << "readyok\n";
}
void UCI::go(const std::vector<std::string> &subCommands) const {
  std::cout << "going\n";
}
void UCI::position(const std::vector<std::string> &subCommands) {}
void UCI::loop() {

  std::string combinedCommand;
  while (std::getline(std::cin, combinedCommand)) {
    std::string command = getCommand(combinedCommand);
    std::vector<std::string> subCommands = getSubCommands(combinedCommand);

    commands[command](subCommands);
  }
}
