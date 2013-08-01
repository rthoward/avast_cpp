#include "libtcod.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "actor.hpp"
#include "engine.hpp"
#include "map.hpp"
#include "gui.hpp"
#include <string>

Engine::Engine(int screenWidth, int screenHeight) {
   setStatus(STARTUP);
   fovRadius = 10;
   computeFov = true;
   this->screenWidth = screenWidth;
   this->screenHeight = screenHeight;
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(screenWidth, screenHeight, "cpp roguelike", false);
   std::string playerName = "Player";
   player = new Actor(40, 25, '@', playerName, TCODColor::white);
   player->setDestructible(new PlayerDestructible(30, 2, "your cadaver"));
   player->setAttacker(new Attacker(5));
   player->setAI(new PlayerAI());
   actors.push(player);
   map = new Map(80, 43);
   gui = new GUI();
}

Engine::~Engine() {
   actors.clearAndDelete();
   delete map;
   delete gui;
}

void Engine::update() {
   if (gameStatus == STARTUP)
      map->computeFov();

   gameStatus = IDLE;
   TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
   player->update();

   // update all non-player actors
   if (gameStatus == NEW_TURN) {
      for (Actor **iterator = actors.begin();
           iterator != actors.end(); iterator++) {
         Actor *actor = *iterator;
         if (actor != player)
            actor->update();
      }
   }
}

void Engine::render() {
   TCODConsole::root->clear();

   map->render();
   TCODConsole::root->print(1, screenHeight - 2, "HP : %d/%d",
         (int)player->getDestructible()->getHP(), 
         (int)player->getDestructible()->getHPMax());

   // render actors
   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (actor != player && map->isInFov(actor->getX(), actor->getY()))
            actor->render();
   }

   player->render();
   gui->render();
}

void Engine::addActor(Actor *actor) {
   this->actors.push(actor);
}

void Engine::sendToFront(Actor *actor) {
   actors.remove(actor);
   actors.insertBefore(actor, 0);
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

TCOD_key_t Engine::getLastKey() {
   return this->lastKey;
}

int Engine::getScreenWidth() const     { return screenWidth; }
int Engine::getScreenHeight() const    { return screenHeight; }

void Engine::setStatus(enum GameStatus status) {
   gameStatus = status;
}
