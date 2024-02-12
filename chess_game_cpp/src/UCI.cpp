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
  int wtime = INT32_MAX;
  int btime = INT32_MAX;
  int winc = 0;
  int binc = 0;

  auto depthIt = std::find(tokens.begin(), tokens.end(), "depth");
  if (depthIt != tokens.end()) {
    depth = stoi(*(depthIt + 1));
  }
  auto wtimeIt = std::find(tokens.begin(), tokens.end(), "wtime");
  if (wtimeIt != tokens.end()) {
    wtime = std::stod(*(wtimeIt + 1));
  }
  auto btimeIt = std::find(tokens.begin(), tokens.end(), "btime");
  if (btimeIt != tokens.end()) {
    btime = std::stod(*(btimeIt + 1));
  }
  auto wincIt = std::find(tokens.begin(), tokens.end(), "winc");
  if (wincIt != tokens.end()) {
    winc = std::stod(*(wincIt + 1));
  }
  auto bincIt = std::find(tokens.begin(), tokens.end(), "binc");
  if (bincIt != tokens.end()) {
    binc = std::stod(*(bincIt + 1));
  }
  Search srch(_position, wtime, winc, btime, binc);
  Move bestMove = srch.searchIt(depth);
  std::cout << "bestmove " << bestMove.toStr() << "\n";
}
// TODO: add fen support;
void UCI::position(const std::vector<std::string> &tokens) {
  if (tokens[0] == "startpos") {
    _position.setBoardToInitialConfiguration();
  } else {
  }
  auto moves = std::find(tokens.begin(), tokens.end(), "move");
  for (auto it = moves + 1; it != tokens.end() && (moves != tokens.end());
       it++) {
    moveToStr ms(_position);
    _position.makeMove(ms.getMove(*it));
  }
}
void UCI::ucinewgame(const std::vector<std::string> &tokens) {
  Position tempPosition;
  _position = tempPosition;
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
