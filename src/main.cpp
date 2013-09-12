#include "libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include "random_actor.hpp"

RandomItemGenerator randomItem;
RandomMonsterGenerator randomMonster;
Engine engine(100, 60);

int main() {

   while (!TCODConsole::isWindowClosed() && engine.getStatus() != Engine::QUIT) {
      engine.update();
      engine.render();
      TCODConsole::flush();
   }

   return 0;
}
