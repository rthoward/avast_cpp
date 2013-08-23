#ifndef __DESTRUCTIBLE_HPP__
#define __DESTRUCTIBLE_HPP__

#include <string>
#include "actor.hpp"
using namespace std;

class Destructible {
   
public:

   Destructible(float hpMax, string corpsename, int exp = 5);
   virtual ~Destructible();

   float takeDamage(Actor *me, float damage);
   float heal(float amount);
   virtual void die(Actor *me);

   inline bool isDead() { return hp <= 0; }
   float getHP();
   float getHPMax();   
   int getExp();

private:
   float hpMax;
   float hp;   
   string corpseName;
   int exp;
};

class MonsterDestructible : public Destructible {
public :
	MonsterDestructible(float maxHp, const char *corpseName);
	void die(Actor *owner);
};

class PlayerDestructible : public Destructible {
public :
	PlayerDestructible(float maxHp, const char *corpseName);
	void die(Actor *owner);
};

#endif
