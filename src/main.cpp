#include "libtcod.hpp"
#include "actor.hpp"

int main() {
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(80, 50, "c++ roguelike", false);

   Actor player(20, 20, '@', TCODColor::white);

   TCOD_key_t key;

   while (!TCODConsole::isWindowClosed()) {

      TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
      switch(key.vk) {
         case TCODK_UP : player.move(0, -1); break;
         case TCODK_DOWN : player.move(0, 1); break;
         case TCODK_LEFT : player.move(-1, 0); break;
         case TCODK_RIGHT : player.move(1, 0); break;
         default: break;
      }

      TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, NULL, NULL);
      TCODConsole::root->clear();
      player.render();
      TCODConsole::flush();
   }

   return 0;
}
