#pragma once
#include "constants.hpp"
#include "float.h"
#include "moveToStr.hpp"
#include "move_generation.hpp"
#include "position.hpp"
#include "search.hpp"
#include "utilities.hpp"
#include <chrono>
#include <fstream>
#include <functional>
#include <iomanip>
#include <map>
class UCI {

public:
  UCI() {
    std::string currentDIR = getExecutableDirectory();
    std::string executableName = getExecutableName();
    path = currentDIR + '/' + executableName + "debugLog";
    // debugLog.open(path);
    // debugLog.close();
  }

  void loop();

  void manual(const std::string &command);
  void debugInit() const;

  void logMessage(std::string message);
  Position getPosition() const;

private:
  // commands:
  std::ofstream debugLog;
  void uci(const std::vector<std::string> &subCommand);
  void isready(const std::vector<std::string> &subCommand);
  void go(const std::vector<std::string> &subCommand);
  void position(const std::vector<std::string> &subCommand);
  void ucinewgame(const std::vector<std::string> &subCommand);

  std::map<std::string, std::function<void(const std::vector<std::string> &)>>
      commands = {
          {"isready", std::bind(&UCI::isready, this, std::placeholders::_1)},
          {"uci", std::bind(&UCI::uci, this, std::placeholders::_1)},
          {"go", std::bind(&UCI::go, this, std::placeholders::_1)},
          {"ucinewgame",
           std::bind(&UCI::ucinewgame, this, std::placeholders::_1)},
          {"position", std::bind(&UCI::position, this, std::placeholders::_1)}};
  Position _position;
  std::string path;
  Search search_;
};