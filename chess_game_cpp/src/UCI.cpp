#include "UCI.hpp"

void UCI::uci(const std::vector<std::string> &subCommands) {
  // std::string message = "id name Engine v0.1.0-alpha";
  // logMessage(message);
  std::cout << "id name Engine v0.1.3-alpha\n";
  logMessage("uciok");
  std::cout << "uciok\n";
}
void UCI::isready(const std::vector<std::string> &subCommands) {
  logMessage("readyok");
  std::cout << "readyok\n";
}
void UCI::go(const std::vector<std::string> &tokens) {
  int maxDepth = 500;
  int wtime = INT32_MAX;
  int btime = INT32_MAX;
  int winc = 0;
  int binc = 0;

  auto depthIt = std::find(tokens.begin(), tokens.end(), "depth");
  if (depthIt != tokens.end()) {
    maxDepth = stoi(*(depthIt + 1));
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
  Move bestMove = srch.searchIt(maxDepth, true);
  Move invalidMove(a1, a1, 0);
  if (bestMove.getMoveInt() != invalidMove.getMoveInt()) {
    std::string message;
    message += "bestmove ";
    logMessage(message + bestMove.toStr());
    std::cout << "bestmove " << bestMove.toStr() << "\n";
  }
}
// TODO: add fen support;
void UCI::position(const std::vector<std::string> &tokens) {
  if (tokens[0] == "startpos") {
    _position.setBoardToInitialConfiguration();
  } else {
    // TODO: FEN
  }
  auto moves = std::find(tokens.begin(), tokens.end(), "moves");
  for (auto it = moves + 1; it != tokens.end() && (moves != tokens.end());
       it++) {
    moveToStr ms(_position);
    _position.makeMove(ms.getMove(*it));
  }
}
void UCI::ucinewgame(const std::vector<std::string> &tokens) {
  _position.clear();
}
void UCI::loop() {
  std::string combinedCommand;
  // if (debugLog) {
  //   debugLog << "Debug Log\n";
  // } else {
  //   std::cerr << "couldnt open the debug file " + path;
  // }

  std::string message;
  while (std::getline(std::cin, combinedCommand)) {
    message = "Input:" + combinedCommand;
    logMessage(message);
    std::vector<std::string> tokens = tokenize(combinedCommand);
    std::string command = tokens[0];
    if (tokens[0] == "quit") {
      break;
    }
    if (tokens[0] == "stop") {
      continue;
    }
    if (tokens.size() < 2) {
      std::vector<std::string> emptyToken;
      commands[command](emptyToken);
    } else {
      tokens.erase(tokens.begin());
      commands[command](tokens);
    }
  }
}
void UCI::manual(const std::string &combinedCommand) {

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
void UCI::debugInit() const {}
void UCI::logMessage(std::string message) {
  // debugLog.open(path, std::ios::app);

  // auto in_time_t =
  //     std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  // auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
  //                   std::chrono::system_clock::now().time_since_epoch()) %
  //               1000;

  // debugLog << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
  // debugLog << '.' << std::setfill('0') << std::setw(3) << millis.count()
  //          << " - " << message << "\n";
  // debugLog.close();
}

Position UCI::getPosition() const { return _position; }