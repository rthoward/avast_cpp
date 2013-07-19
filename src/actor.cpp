#include "libtcod.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include <stdio.h>

Actor::Actor(int x, int y, int ch, string name, const TCODColor &col) :
   x(x), y(y), ch(ch), name(name), color(col) {}

void Actor::render() const {
   TCODConsole::root->setChar(x, y, ch);
   TCODConsole::root->setCharForeground(x, y, color);
}

void Actor::update() {

}

bool Actor::moveOrAttack(int dx, int dy) {

   int newX = x + dx;
   int newY = y + dy;
   Actor *collider;

   Map *map = engine.getMap();

   if (engine.getMap()->isWall(newX, newY))
      return false;   

   if ( (collider = map->getActorAt(newX, newY)) ){
      printf("colliding with actor %s\n", collider->getName().c_str());
      return false;
   }

   this->x = newX;
   this->y = newY;

   return true;
}

void Actor::moveTo(int x, int y) {
   this->x = x;
   this->y = y;
}

int Actor::getX()             { return x; }
int Actor::getY()             { return y; }
string Actor::getName()       { return name; }
