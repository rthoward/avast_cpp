#include "libtcod.hpp"
#include "destructible.hpp"
#include "engine.hpp"
#include "stats.hpp"
#include <iostream>
using namespace std;

Destructible::Destructible(float hpMax, string corpseName, int exp) :
    hpMax(hpMax),hp(hpMax),corpseName(corpseName),exp(exp) {
}

Destructible::~Destructible() {};

float Destructible::takeDamage(Actor *me, float damage) {
   
   // defense negates some damage
   if (me->getStats())
      damage -= me->getStats()->getCon();
   if (damage > 0) {
      hp -= damage;
      if (isDead())
         die(me);
   }

   return damage;
}

float Destructible::heal(float amount) {
   hp += amount;

   if (hp > hpMax) {
      amount -= hp - hpMax;
      hp = hpMax;
   }

   return amount;
}

void Destructible::die(Actor *me) {
   // turn actor into corpse
   me->setGlyph('%');
   me->setName(corpseName);
   engine.sendToBack(me);
}

MonsterDestructible::MonsterDestructible(float maxHp, const char *corpseName) :
	Destructible(maxHp,corpseName) {
}

void MonsterDestructible::die(Actor *me) {
	Destructible::die(me);
}

PlayerDestructible::PlayerDestructible(float maxHp, const char *corpseName) :
	Destructible(maxHp,corpseName) {
}

void PlayerDestructible::die(Actor *me) {
   cout << "You died. Game over.\n";
	Destructible::die(me);
	engine.setStatus(Engine::DEFEAT);
}

float Destructible::getHP()         { return hp; }
float Destructible::getHPMax()      { return hpMax; }
int Destructible::getExp()          { return exp; }
