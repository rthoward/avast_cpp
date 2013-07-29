#include "libtcod.hpp"
#include "destructible.hpp"
#include "actor.hpp"
#include "attacker.hpp"
#include <iostream>
using namespace std;

Attacker::Attacker(float power) : power(power) {

}

void Attacker::attack(Actor *attacker, Actor *target) {

   float damageTaken = 0;
   Attacker *att = attacker->getAttacker();
   Destructible *dest = target->getDestructible();
   float a, d, dmg;

   // make sure target is destructible and alive
   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, att->getPower())) ) {
         // todo: clean this up
         a = attacker->getAttacker()->getPower();
         d = target->getDestructible()->getDefense();
         dmg = a - d;
         cout << attacker->getName() << " -> " << target->getName();
         cout << "(" << target->getID() << ")";
         cout << " [" << a << " - " << d << " = " << dmg << "]" << endl;
      } else {

      }
   }
}

float Attacker::getPower() const    { return this->power; }
