#include "constants.hpp"
#include "position.hpp"
#include "utilities.hpp"
class UCI {

public:
  UCI() = default;
  UCI(const Position &position_) : position(position_){};
  Move getMove(const std::string &moveStr) const;

private:
  Position position;
};