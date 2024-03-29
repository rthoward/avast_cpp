#ifndef __DESTRUCTIBLE_HPP__
#define __DESTRUCTIBLE_HPP__

#include <string>
#include "actor.hpp"
using namespace std;

class Destructible {
   
public:

   Destructible(float hpMax, float def, string corpsename, int exp = 5);
   virtual ~Destructible();

   float takeDamage(Actor *me, float damage);
   float heal(float amount);
   virtual void die(Actor *me);

   inline bool isDead() { return hp <= 0; }
   float getHP();
   float getHPMax();
   float getDefense();
   int getExp();

private:
   float hpMax;
   float hp;
   float defense;
   string corpseName;
   int exp;
};

class MonsterDestructible : public Destructible {
public :
	MonsterDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
};

class PlayerDestructible : public Destructible {
public :
	PlayerDestructible(float maxHp, float defense, const char *corpseName);
	void die(Actor *owner);
};

#endif
