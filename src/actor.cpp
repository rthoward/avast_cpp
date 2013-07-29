#include "libtcod.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include <stdio.h>

int Actor::ACTOR_ID = 0;

Actor::Actor(int x, int y, int ch, string name, const TCODColor &col) :
   x(x), y(y), ch(ch), name(name), color(col), id(ACTOR_ID++),
   blocks(true), attacker(NULL), destructible(NULL), ai(NULL) {}

void Actor::render() const {
   TCODConsole::root->setChar(x, y, ch);
   TCODConsole::root->setCharForeground(x, y, color);
}

void Actor::update() {
   if (ai)
      ai->update(this);
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

int Actor::getX() const          { return x; }
int Actor::getY() const          { return y; }
int Actor::getID() const         { return id; }
string Actor::getName() const    { return name; }
Destructible* Actor::getDestructible() const {
   return this->destructible;
}
Attacker* Actor::getAttacker() const {
   return this->attacker;
}

void Actor::setGlyph(int glyph)        { this->ch = glyph; }
void Actor::setName(string name)       { this->name = name; }
void Actor::setBlocks(bool blocks)     { this->blocks = blocks; }
void Actor::setDestructible(Destructible *dest ) {
   this->destructible = dest;
}
void Actor::setAttacker(Attacker *att) {
   this->attacker = att;
}
void Actor::setAI(AI *ai) {
   this->ai = ai;
}

bool Actor::isBlocking() const         { return blocks; }
bool Actor::isDead() const             { return destructible->isDead(); }
