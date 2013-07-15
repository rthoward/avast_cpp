#include "libtcod.hpp"

int main() {
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(80, 50, "c++ roguelike", false);

   while (!TCODConsole::isWindowClosed()) {
      TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, NULL, NULL);
      TCODConsole::root->clear();
      TCODConsole::root->putChar(40, 25, '@');
      TCODConsole::flush();
   }

   return 0;
}
