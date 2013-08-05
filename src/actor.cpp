#include "libtcod.hpp"
#include "destructible.hpp"
#include "gui.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "pickable.hpp"
#include "container.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include <stdio.h>

int Actor::ACTOR_ID = 0;

Actor::Actor(int x, int y, int ch, string name, const TCODColor &col) :
   x(x), y(y), ch(ch), name(name), color(col), id(ACTOR_ID++),
   blocks(true), attacker(NULL), destructible(NULL), ai(NULL),
   pickable(NULL), container(NULL) {}

Actor::~Actor() {
   if (attacker)     delete attacker;
   if (destructible) delete destructible;
   if (ai)           delete ai;
   if (pickable)     delete pickable;
   if (container)    delete container;
}

void Actor::render() const {
   TCODConsole::root->setChar(x, y, ch);
   TCODConsole::root->setCharForeground(x, y, color);
}

void Actor::update() {
   if (ai)
      ai->update(this);
}

void Actor::moveTo(int x, int y) {
   this->x = x;
   this->y = y;
}

bool Actor::tryPickUp(Actor *me, Actor *item) {
   if (item == NULL)
      return false;
   if (!item->getPickable())
      return false;

   if (item->getPickable()->pick(me, item)) {
      return true;
   } else {
      return false;
   }
   
}

bool Actor::isBlocking() const         { return blocks; }
bool Actor::isDead() const             { return destructible->isDead(); }
enum Actor::Type Actor::type() const {
   if (pickable)
      return ITEM;
   else if (this == engine.getPlayer())
      return PLAYER;
   else if (ai) {
      if (isDead())  return CORPSE;
      else           return MONSTER; 
   }
}

int Actor::getX() const                            { return x; }
int Actor::getY() const                            { return y; }
int Actor::getID() const                           { return id; }
string Actor::getName() const                      { return name; }
Destructible* Actor::getDestructible() const       { return this->destructible; }
Attacker* Actor::getAttacker() const               { return this->attacker; }
Container* Actor::getContainer() const             { return this->container; }
Pickable* Actor::getPickable() const               { return this->pickable; }

void Actor::setGlyph(int glyph)                    { this->ch = glyph; }
void Actor::setName(string name)                   { this->name = name; }
void Actor::setBlocking(bool blocks)               { this->blocks = blocks; }
void Actor::setDestructible(Destructible *dest )   { this->destructible = dest; }
void Actor::setAttacker(Attacker *att)             { this->attacker = att; }
void Actor::setAI(AI *ai)                          { this->ai = ai; }
void Actor::setContainer(Container *cont)          { this->container = cont; }
void Actor::setPickable(Pickable *pick)            { this->pickable = pick; }
