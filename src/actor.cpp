#include "libtcod.hpp"
#include "destructible.hpp"
#include "gui.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "pickable.hpp"
#include "container.hpp"
#include "equipment.hpp"
#include "equippable.hpp"
#include "actor.hpp"
#include "map.hpp"
#include "engine.hpp"
#include <stdio.h>

int Actor::ACTOR_ID = 0;

Actor::Actor(int x, int y, int ch, string name, const TCODColor &col) :
   x(x), y(y), ch(ch), name(name), color(col), id(ACTOR_ID++),
   blocks(true), fovOnly(false), floor(0), exp(0), expLevel(1),
   attacker(NULL), destructible(NULL), ai(NULL), pickable(NULL), container(NULL),
   stats(NULL), equippable(NULL), equipment(NULL) {};

Actor::~Actor() {
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

   string msg = "There is nothing here to pick up.";

   if (item == NULL) {
      engine.getGUI()->message(msg);
      return false;
   }
   else if ( !(item->getType() == ITEM) ) {
      engine.getGUI()->message(msg);
      return false;
   }
   else if (item->getPickable()->pick(item, me)) {
      msg = "You pick up the "; msg += item->getName() + ".";
      engine.getGUI()->message(msg, TCODColor::lightGreen);
      return true;
   }
   else {
      msg = "Your inventory is full";
      engine.getGUI()->message(msg, TCODColor::lightRed);
   }

   return false;
}

bool Actor::tryEquip(Actor *me, Actor *item) {
   Equippable *equippable;

   if ( (equippable = item->getEquippable()) ) {
      if (equipment->equip(me, item)) {
         string msg = "You equip the ";
         msg += item->getName() + ".";
         engine.getGUI()->message(msg, TCODColor::lightGreen);
         return true;
      } else {
         string msg = "You already have something equipped in that slot.";
         engine.getGUI()->message(msg, TCODColor::lightRed);
         return false;
      }
   }

   engine.getGUI()->message("That item cannot be equipped!", TCODColor::lightRed);
   return false;
}

bool Actor::isBlocking() const         { return blocks && !isDead(); }
bool Actor::isDead() const {
   if (destructible)
      return destructible->isDead();
   else
      return false;
}
enum Actor::Type Actor::getType() const {
   if (pickable)
      return ITEM;
   else if (this == engine.getPlayer())
      return PLAYER;
   else if (ai) {
      if (isDead())  return CORPSE;
      else           return MONSTER; 
   }
   return UNKNOWN;
}

void Actor::addExp(int exp) {
   this->exp += exp;

   while (this->exp >= expLevel * 12) {
      expLevel++;
   }
}

int Actor::getX() const                            { return x; }
int Actor::getY() const                            { return y; }
int Actor::getID() const                           { return id; }
int Actor::getChar() const                         { return this->ch; }
string Actor::getName() const                      { return name; }
Destructible* Actor::getDestructible() const       { return this->destructible; }
Attacker* Actor::getAttacker() const               { return this->attacker; }
Container* Actor::getContainer() const             { return this->container; }
Pickable* Actor::getPickable() const               { return this->pickable; }
Stats* Actor::getStats() const                     { return this->stats; }
Equippable* Actor::getEquippable() const           { return this->equippable; }
Equipment* Actor::getEquipment() const             { return this->equipment; }
bool Actor::getFovOnly() const                     { return this->fovOnly; }
int Actor::getFloor() const                        { return this->floor; }

void Actor::setGlyph(int glyph)                    { this->ch = glyph; }
void Actor::setName(string name)                   { this->name = name; }
void Actor::setBlocking(bool blocks)               { this->blocks = blocks; }
void Actor::setDestructible(Destructible *dest )   { this->destructible = dest; }
void Actor::setAttacker(Attacker *att)             { this->attacker = att; }
void Actor::setAI(AI *ai)                          { this->ai = ai; }
void Actor::setContainer(Container *cont)          { this->container = cont; }
void Actor::setPickable(Pickable *pick)            { this->pickable = pick; }
void Actor::setStats(Stats *stats)                 { this->stats = stats; }
void Actor::setEquippable(Equippable *equip)       { this->equippable = equip; }
void Actor::setEquipment(Equipment *equipment)     { this->equipment = equipment; }
void Actor::setFovOnly(bool fovOnly)               { this->fovOnly = fovOnly; }
void Actor::setFloor(int floor)                    { this->floor = floor; }
