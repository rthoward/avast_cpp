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

   // make sure target is destructible and alive
   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, att->getPower())) ) {
         cout << attacker->getName() << " -> " << target->getName();
         cout << " [" << damageTaken << "]\n";
      } else {
         cout << attacker->getName() << " -> " << target->getName();
         cout << " [" << "no effect" << "]\n";
      }
   }
}

float Attacker::getPower() const    { return this->power; }
