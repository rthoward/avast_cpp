#include "libtcod.hpp"
#include "destructible.hpp"
#include "engine.hpp"
#include <iostream>
using namespace std;

Destructible::Destructible(float hpMax, float def, string corpseName) :
    hpMax(hpMax),hp(hpMax),defense(def),corpseName(corpseName) {
}

float Destructible::takeDamage(Actor *me, float damage) {
   
   // defense negates some damage
   damage -= defense;
   if (damage > 0) {
      hp -= damage;
      if (isDead())
         die(me);
   }

   return damage;
}

void Destructible::die(Actor *me) {
   // turn actor into corpse
   me->setGlyph('%');
   me->setName(corpseName);
   engine.sendToFront(me);
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
