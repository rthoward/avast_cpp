#include "libtcod.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "container.hpp"
#include "ai.hpp"
#include "actor.hpp"
#include "engine.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "actor_factory.hpp"
#include <string>

Engine::Engine(int screenWidth, int screenHeight) {
   setStatus(STARTUP);
   fovRadius = 10;
   computeFov = true;
   ActorFactory factory;
   this->screenWidth = screenWidth;
   this->screenHeight = screenHeight;
   TCODConsole::setCustomFont("dejavu12x12_gs_tc.png", TCOD_FONT_LAYOUT_TCOD | TCOD_FONT_TYPE_GREYSCALE);
   TCODConsole::initRoot(screenWidth, screenHeight, "cpp roguelike", false);
   std::string playerName = "Player";
   maxDLevel = currentDLevel = 1;
   player = factory.generate(40, 25, ActorFactory::PLAYER, playerName);
   addActor(player);
   maps.push(new Map(80, 43));
   gui = new GUI();

   gui->message("Welcome to the dungeons of danger!");
}

Engine::~Engine() {
   actors.clearAndDelete();
   maps.clearAndDelete();
   delete gui;
}

void Engine::update() {
   if (gameStatus == STARTUP)
      getMap()->computeFov();

   gameStatus = IDLE;
   TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &lastKey, NULL);
   player->update();

   // update all non-player actors
   if (gameStatus == NEW_TURN) {
      for (Actor **iterator = actors.begin();
           iterator != actors.end(); iterator++) {
         Actor *actor = *iterator;
         if (shouldUpdate(actor))
            actor->update();
      }
   } else if (gameStatus == QUIT) {

   }
}

void Engine::render() {
   TCODConsole::root->clear();

   getMap()->render();
   TCODConsole::root->print(1, screenHeight - 2, "HP : %d/%d | DLVL %d",
         (int)player->getDestructible()->getHP(), 
         (int)player->getDestructible()->getHPMax(),
         currentDLevel);

   // render actors
   for (Actor **iter = actors.end() - 1; iter != actors.begin(); iter--) {
      Actor *actor = *iter;
      if (shouldRender(actor))
         actor->render();
   }

   player->render();
   gui->render();
}

void Engine::addActor(Actor *actor) {
   this->actors.push(actor);
}

void Engine::removeActor(Actor *actor) {
   this->actors.remove(actor);
}

void Engine::sendToFront(Actor *actor) {
   actors.remove(actor);
   actors.insertBefore(actor, 0);
}

void Engine::sendToBack(Actor *actor) {
   actors.remove(actor);
   actors.insertBefore(actor, actors.size());
}

TCODList<Actor *> Engine::getActorList() {
   return actors;
}

Actor* Engine::getPlayer() {
   return player;
}

Map* Engine::getMap() {
   return maps.get(currentDLevel - 1);
}

Engine::GameStatus Engine::getStatus() {
   return gameStatus;
}

TCOD_key_t Engine::getLastKey() {
   return this->lastKey;
}

int Engine::getScreenWidth() const     { return screenWidth; }
int Engine::getScreenHeight() const    { return screenHeight; }
GUI *Engine::getGUI()                  { return this->gui; }
int Engine::getCurrentDLevel() const   { return this->currentDLevel; }

void Engine::setStatus(enum GameStatus status) {
   gameStatus = status;
}

bool Engine::upLevel() {
   if (currentDLevel <= 1)
      return false;
   else {
      currentDLevel--;
      Actor *stairs = getDownStaircase();
      player->moveTo(stairs->getX(), stairs->getY());
      return true;
   }
}

bool Engine::downLevel() {
   if (currentDLevel < maxDLevel) {
      currentDLevel++;
      Actor *stairs = getUpStaircase();
      player->moveTo(stairs->getX(), stairs->getY());
      return true;
   } else {
      // generate new map
      currentDLevel = ++maxDLevel;
      maps.push(new Map(80, 43));
      return true;
   }
}

bool Engine::shouldRender(Actor *actor) {

   if (actor->getFloor() != currentDLevel)
      return false;
   else if (actor == player)
      return false;

   int x, y;
   x = actor->getX();
   y = actor->getY();

   if (!actor->getFovOnly() && getMap()->isExplored(x, y))
      return true;
   else if (getMap()->isInFov(x, y))
      return true;

   return false;
}

bool Engine::shouldUpdate(Actor *actor) {
   return (actor->getFloor() == getCurrentDLevel() && actor != player);
}

Actor *Engine::getDownStaircase() const {
   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (actor->getChar() == '>')
         return actor;
   }

   return NULL;
}

Actor *Engine::getUpStaircase() const {
   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (actor->getChar() == '<')
         return actor;
   }

   return NULL;
}
