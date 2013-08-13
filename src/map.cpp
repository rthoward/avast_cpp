#include "libtcod.hpp"
#include "actor_factory.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include "pickable.hpp"
#include <stdio.h>

static const int ROOM_MAX_SIZE = 12;
static const int ROOM_MIN_SIZE = 6;
static const int ROOM_MAX_MONSTERS = 3;
static const int ROOM_MAX_ITEMS = 2;

class BspListener : public ITCODBspCallback {
public:
   BspListener(Map &map) : map(map), roomNum(0) {}
private:
   Map &map;
   int roomNum;
   int lastx, lasty;
   bool visitNode(TCODBsp *node, void *userData) {
      if (node->isLeaf()) {
         int x, y, w, h;

         // dig room
         TCODRandom *rng = TCODRandom::getInstance();
         w = rng->getInt(ROOM_MIN_SIZE, node->w - 2);
         h = rng->getInt(ROOM_MIN_SIZE, node->h - 2);
         x = rng->getInt(node->x + 1, node->x + node->w - w - 1);
         y = rng->getInt(node->y + 1, node->y + node->h - h - 1);
         map.createRoom(roomNum == 0, x, y, x + w - 1, y + h - 1);

         if (roomNum != 0) {
            // dig corridor from last room
            map.dig(lastx, lasty, x + w / 2, lasty);
            map.dig(x + w / 2, lasty, x + w / 2, y + h / 2);
         }

         lastx = x + w / 2;
         lasty = y + h / 2;
         roomNum++;
      }

      return true;
   }
};

Map::Map(int width, int height) : width(width), height(height) {
   tiles = new Tile[width * height];
   map = new TCODMap(width, height);
   TCODBsp bsp(0, 0, width, height);
   actorFactory = new ActorFactory;
   bsp.splitRecursive(NULL, 8, ROOM_MAX_SIZE, ROOM_MAX_SIZE, 1.5f, 1.5f);
   BspListener listener(*this);
   bsp.traverseInvertedLevelOrder(&listener, NULL);
   addStaircases();
}

Map::~Map() {
   delete [] tiles;
   delete map;
   delete actorFactory;
}

bool Map::isWall(int x, int y) const {
   return !map->isWalkable(x, y);
}

bool Map::isExplored(int x, int y) const {
   return tiles[x + y * width].explored;
}

bool Map::isInFov(int x, int y) const {
   if (map->isInFov(x, y)) {
      tiles[x + y * width].explored = true;
      return true;
   }

   return false;
}

bool Map::canWalk(int x, int y) const {
   if (isWall(x, y))
      return false;

   Actor *actor;

   if ( (actor = getActorAt(x, y)) ) {
      if (actor->isBlocking())
         return false;
   }

   return true;
}

void Map::computeFov() {
   Actor *player = engine.getPlayer();
   map->computeFov(player->getX(), player->getY(),
         engine.fovRadius);
}

void Map::render() const {
   static const TCODColor darkWall(0, 0, 100);
   static const TCODColor darkGround(50, 50, 150);
   static const TCODColor lightWall(130, 110, 50);
   static const TCODColor lightGround(200, 180, 50);

   for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
         if (isInFov(x, y))
            TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? lightWall : lightGround);
         else if (isExplored(x, y))
            TCODConsole::root->setCharBackground(x, y, isWall(x,y) ? darkWall : darkGround);
      }
   }
}

void Map::dig(int x1, int y1, int x2, int y2) {

   // swap if necessary
   if (x2 < x1) {
      int tmp = x2;
      x2 = x1;
      x1 = tmp;
   }
   if (y2 < y1) {
      int tmp = y2;
      y2 = y1;
      y1 = tmp;
   }

   // hollow out room
   for (int tilex = x1; tilex <=  x2; tilex++) {
      for (int tiley = y1; tiley <= y2; tiley++) {
         map->setProperties(tilex, tiley, true, true);
      }
   }
}

void Map::createRoom(bool first, int x1, int y1, int x2, int y2) {
   dig(x1, y1, x2, y2);

   // add player to first room
   // otherwise, random chance of adding NPC
   if (first) {
      engine.getPlayer()->moveTo((x1 + x2) / 2, (y1 + y2) / 2);
   }
   else {
      TCODRandom *rng = TCODRandom::getInstance();
      int numMonsters = rng->getInt(0, ROOM_MAX_MONSTERS);
      int numItems = rng->getInt(0, ROOM_MAX_ITEMS);

      while(numMonsters > 0) {
         int x = rng->getInt(x1, x2);
         int y = rng->getInt(y1, y2);
         if (canWalk(x, y)) {
            addMonster(x, y);
         }
         else {
         }
         numMonsters--;
      }

      while (numItems > 0) {
         int x = rng->getInt(x1, x2);
         int y = rng->getInt(y1, y2);
         if (canWalk(x, y)) {
            addItem(x, y);
         }
         numItems--;
      }
   }

}

void Map::addMonster(int x, int y) {
   TCODRandom *rng = TCODRandom::getInstance();
   Actor *monster;

   if (rng->getInt(0, 100) < 80) {
      monster = actorFactory->generate(x, y, ActorFactory::M_ORC);
   } else {
      monster = actorFactory->generate(x, y, ActorFactory::M_TROLL);
   }

   monster->setFloor(engine.getCurrentDLevel());
   engine.addActor(monster);
}

void Map::addItem(int x, int y) {
   Actor *healthPotion = new Actor(x, y, '!', "potion of healing", TCODColor::violet);
   healthPotion->setFloor(engine.getCurrentDLevel());
   healthPotion->setBlocking(false);
   healthPotion->setPickable(new Healer(4));
   engine.addActor(healthPotion);
}

void Map::addStaircases() {
   int currentFloor = engine.getCurrentDLevel();

   Actor *upStaircase = actorFactory->generate(0, 0, ActorFactory::F_STAIRS_UP);
   Actor *downStaircase = actorFactory->generate(0, 0, ActorFactory::F_STAIRS_DOWN);
   upStaircase->setFloor(currentFloor);
   downStaircase->setFloor(currentFloor);
 
   upStaircase->moveTo(engine.getPlayer()->getX(), engine.getPlayer()->getY());
   moveActorRandom(downStaircase);
   engine.addActor(upStaircase);
   engine.addActor(downStaircase);
}

void Map::moveActorRandom(Actor *actor) {
   int randX, randY;
   bool added = false;
   TCODRandom *rng = TCODRandom::getInstance();

   while (!added) {
      randX = rng->getInt(0, width);
      randY = rng->getInt(0, height);
      if (canWalk(randX, randY)) {
         actor->moveTo(randX, randY);
         added = true;
      }
   }
}

Actor* Map::getActorAt(int x, int y) const {

   TCODList<Actor *> actors = engine.getActorList();

   for (Actor **iter = actors.begin(); iter != actors.end(); iter++) {
      Actor *actor = *iter;
      if (actor == NULL || actor->getFloor() != engine.getCurrentDLevel())
         continue;
      else if (actor != engine.getPlayer() &&
            actor->getX() == x && actor->getY() == y)
         return actor;
   }
   return NULL;
}

Actor* Map::getItemAt(int x, int y) const {
   Actor *actor = getActorAt(x, y);
   if (actor == NULL)
      return NULL;
   if (!actor->getPickable())
      return NULL;
   return actor;
}
