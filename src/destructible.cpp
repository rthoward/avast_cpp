#include "libtcod.hpp"
#include "destructible.hpp"
#include "engine.hpp"
#include <iostream>
using namespace std;

Destructible::Destructible(float hpMax, float def, string corpseName, int exp) :
    hpMax(hpMax),hp(hpMax),defense(def),corpseName(corpseName),exp(exp) {
}

Destructible::~Destructible() {};

float Destructible::takeDamage(Actor *me, float damage) {
   
   // defense negates some damage
   damage -= defense;
   if (damage < 0)
      damage = 0;
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
   hp = 0;
   me->setGlyph('%');
   me->setName(corpseName);
   engine.sendToBack(me);
}

MonsterDestructible::MonsterDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp,defense,corpseName) {
}

void MonsterDestructible::die(Actor *me) {
	Destructible::die(me);
}

PlayerDestructible::PlayerDestructible(float maxHp, float defense, const char *corpseName) :
	Destructible(maxHp,defense,corpseName) {
}

void PlayerDestructible::die(Actor *me) {
   cout << "You died. Game over.\n";
	Destructible::die(me);
	engine.setStatus(Engine::DEFEAT);
}

float Destructible::getHP()         { return hp; }
float Destructible::getHPMax()      { return hpMax; }
float Destructible::getDefense()    { return defense; }
int Destructible::getExp()          { return exp; }
