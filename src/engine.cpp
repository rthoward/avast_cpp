#include "libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"

Engine::Engine() {
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(80, 50, "cpp roguelike", false);
   player = new Actor(40, 25, '@', TCODColor::white);
   actors.push(player);
   actors.push(new Actor(60, 13, '@', TCODColor::yellow));
   map = new Map(80, 45);
}

Engine::~Engine() {
   actors.clearAndDelete();
   delete map;
}

void Engine::update() {
   TCOD_key_t key;
   TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &key, NULL);

   switch(key.vk) {
      case TCODK_UP : 
         if (!map->isWall(player->getX(), player->getY() - 1)) {
            player->move(0, -1);   
         }
      break;
      case TCODK_DOWN : 
         if (!map->isWall(player->getX(), player->getY() + 1)) {
            player->move(0, 1);
         }
      break;
      case TCODK_LEFT : 
         if (!map->isWall(player->getX() - 1, player->getY())) {
            player->move(-1, 0);
         }
      break;
      case TCODK_RIGHT : 
         if (!map->isWall(player->getX() + 1,player->getY())) {
            player->move(1, 0);
         }
      break;

      default: break;
   }
}

void Engine::render() {
   TCODConsole::root->clear();

   map->render();

   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      (*iter)->render();
   }
}
