#include "UCI.hpp"

void UCI::uci(const std::vector<std::string> &subCommands) const {
  std::cout << "id name  Machine Algorithm for Real-time Tactical "
               "Analysis\n";
  std::cout << "uciok\n";
}
void UCI::isready(const std::vector<std::string> &subCommands) const {
  std::cout << "readyok\n";
}
void UCI::go(const std::vector<std::string> &tokens) const {
  int depth = 500;
  auto depthIt = std::find(tokens.begin(), tokens.end(), "depth");

  if (depthIt != tokens.end()) {
    depth = stoi(*(depthIt + 1));
  }
  Search srch(_position);
  Move bestMove = srch.searchAB(depth);
  std::cout << "bestmove " << bestMove.toStr() << "\n";
}

void UCI::position(const std::vector<std::string> &tokens) {
  if (tokens[0] == "startpos") {
    _position.setBoardToInitialConfiguration();
  } else {
  }
  auto moves = std::find(tokens.begin(), tokens.end(), "move");
  for (auto it = moves + 1; it != tokens.end(); it++) {
    moveToStr ms(_position);
    _position.makeMove(ms.getMove(*it));
  }
}
void UCI::loop() {
  std::string combinedCommand;
  while (std::getline(std::cin, combinedCommand)) {
    std::vector<std::string> tokens = tokenize(combinedCommand);
    std::string command = tokens[0];
    if (tokens.size() < 2) {
      std::vector<std::string> emptyToken;
      commands[command](emptyToken);
    } else {

      tokens.erase(tokens.begin());
      commands[command](tokens);
    }
  }
}
