#include "libtcod.hpp"

int main() {
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(80, 50, "c++ roguelike", false);

   int playerx = 40, playery = 25;
   TCOD_key_t key;

   while (!TCODConsole::isWindowClosed()) {

      TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);
      switch(key.vk) {
         case TCODK_UP : playery--; break;
         case TCODK_DOWN : playery++; break;
         case TCODK_LEFT : playerx--; break;
         case TCODK_RIGHT : playerx++; break;
         default: break;
      }

      TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, NULL, NULL);
      TCODConsole::root->clear();
      TCODConsole::root->putChar(playerx, playery, '@');
      TCODConsole::flush();
   }

   return 0;
}
