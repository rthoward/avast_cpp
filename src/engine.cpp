#include "libtcod.hpp"
#include "actor.hpp"
#include "engine.hpp"
#include "map.hpp"
#include <string>

Engine::Engine() {
   fovRadius = 10;
   computeFov = true;
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(80, 50, "cpp roguelike", false);
   std::string playerName = "Player";
   player = new Actor(40, 25, '@', playerName, TCODColor::white);
   actors.push(player);
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
            player->moveOrAttack(0, -1);   
            setStatus(NEW_TURN);
         }
      break;
      case TCODK_DOWN : 
         if (!map->isWall(player->getX(), player->getY() + 1)) {
            player->moveOrAttack(0, 1);
            setStatus(NEW_TURN);
         }
      break;
      case TCODK_LEFT : 
         if (!map->isWall(player->getX() - 1, player->getY())) {
            player->moveOrAttack(-1, 0);
            setStatus(NEW_TURN);
         }
      break;
      case TCODK_RIGHT : 
         if (!map->isWall(player->getX() + 1,player->getY())) {
            player->moveOrAttack(1, 0);
            setStatus(NEW_TURN);
         }
      break;

      default: break;
   }

   // depending on state, recompute fov and update actors
   if (gameStatus == NEW_TURN || gameStatus == STARTUP) {
      map->computeFov();
      for (Actor **iterator=actors.begin(); iterator != actors.end(); iterator++) {
         Actor *actor = *iterator;
         if ( actor != player )
            actor->update();
      }
   }
}

void Engine::render() {
   TCODConsole::root->clear();

   map->render();

   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (map->isInFov(actor->getX(), actor->getY()))
            actor->render();
   }
}

void Engine::addActor(Actor *actor) {
   this->actors.push(actor);
}

TCODList<Actor *> Engine::getActorList() {
   return actors;
}

Actor* Engine::getPlayer() {
   return player;
}

Map* Engine::getMap() {
   return map;
}

Engine::GameStatus Engine::getStatus() {
   return gameStatus;
}

void Engine::setStatus(enum GameStatus status) {
   gameStatus = status;
}
