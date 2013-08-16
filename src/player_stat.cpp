#include "player_stat.hpp"

PlayerStat::PlayerStat() : telepathy(0), blind(0) {};

void PlayerStat::update() {
   if (telepathy > 0)
      --telepathy;
   if (blind > 0)
      --blind;
}

void PlayerStat::setTelepathic(int duration) {
   telepathy += duration;
}

void PlayerStat::setBlind(int duration) {
   blind += duration;
}

bool PlayerStat::isTelepathic() const        { return this->telepathy; }
bool PlayerStat::isBlind() const             { return this->blind; }
