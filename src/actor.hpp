#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <string>
#include "actor_list.hpp"
using namespace std;

// forward declarations
class Attacker;
class Destructible;
class AI;
class Pickable;
class Container;
class Equippable;
class Equipment;

class Actor {

public:
   Actor(int x, int y, int ch, string name, const TCODColor &col);
   ~Actor();
  
   void render() const;
   void update();

   // actions
   void moveTo(int x, int y);
   bool tryPickUp(Actor *me, Actor *item);
   bool tryEquip(Actor *me, Actor *item);

   // status, attributes
   bool isBlocking() const;
   bool isDead() const;
   bool isItem() const;
   bool isEquipment() const;
   bool isAttackable() const;

   // accessors
   int getX() const;
   int getY() const;
   int getID() const;
   string getName() const;
   Destructible* getDestructible() const;
   Attacker* getAttacker() const;
   Container *getContainer() const;
   Pickable *getPickable() const;
   Equippable *getEquippable() const;
   Equipment *getEquipment() const;   
   bool getFovOnly() const;
   int getChar() const;
   int getFloor() const;
   ActorType getType() const;

   // mutators
   void setGlyph(int glyph);
   void setName(string name);
   void setBlocking(bool blocks);
   void setDestructible(Destructible *dest);
   void setAttacker(Attacker *att);
   void setAI(AI *ai);
   void setContainer(Container *cont);
   void setPickable (Pickable *pick);
   void setEquippable(Equippable *equip);
   void setEquipment(Equipment *equipment);
   void setFovOnly(bool fovOnly);
   void setFloor(int floor);
   void addExp(int exp);
   void setType(ActorType type);

private:

   // properties
   int x, y;
   int ch;
   string name;
   TCODColor color;
   static int ACTOR_ID;
   const int id;
   bool blocks;
   bool fovOnly;
   int floor;
   int exp;
   int expLevel;
   ActorType type;

   // compositional behavior objects
   Attacker *attacker;
   Destructible *destructible;
   AI *ai;
   Pickable *pickable;
   Container *container;
   Equippable *equippable;
   Equipment *equipment;
};

#endif
