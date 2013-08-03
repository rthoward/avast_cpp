#ifndef __ACTOR_HPP__
#define __ACTOR_HPP__

#include <string>
using namespace std;

// forward declarations
class Attacker;
class Destructible;
class AI;
class Pickable;
class Container;

class Actor {

public:
   Actor(int x, int y, int ch, string name, const TCODColor &col);
   ~Actor();
   void render() const;
   void update();
   bool moveOrAttack(int dx, int dy);
   void moveTo(int x, int y);

   // accessors
   int getX() const;
   int getY() const;
   int getID() const;
   string getName() const;
   Destructible* getDestructible() const;
   Attacker* getAttacker() const;

   // mutators
   void setGlyph(int glyph);
   void setName(string name);
   void setBlocks(bool blocks);
   void setDestructible(Destructible *dest);
   void setAttacker(Attacker *att);
   void setAI(AI *ai);

   // status
   bool isBlocking() const;
   bool isDead() const;

private:

   // properties
   int x, y;
   int ch;
   string name;
   TCODColor color;
   static int ACTOR_ID;
   const int id;
   bool blocks;

   // compositional behavior objects
   Attacker *attacker;
   Destructible *destructible;
   AI *ai;
   Pickable *pickable;
   Container *container;
};

#endif
