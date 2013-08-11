#include "libtcod.hpp"
#include "engine.hpp"
#include "destructible.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "attacker.hpp"
#include <string>
#include <sstream>
using namespace std;

Attacker::Attacker(float power) : power(power) {

}

void Attacker::attack(Actor *attacker, Actor *target) {

   float damageTaken = 0;
   Attacker *att = attacker->getAttacker();
   Destructible *dest = target->getDestructible();
   ostringstream attack_msg, dmg_msg;
   attack_msg<< attacker->getName() << " attacks " << target->getName();
   engine.getGUI()->message(attack_msg.str(), TCODColor::white);

   // make sure target is destructible and alive
   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, att->getPower())) ) {
         dmg_msg << target->getName() << " takes " << damageTaken;
         dmg_msg << " points of damage!";
         if (target == engine.getPlayer())
            engine.getGUI()->message(dmg_msg.str(), TCODColor::darkerRed);
         else
            engine.getGUI()->message(dmg_msg.str(), TCODColor::white);
      } else {
         dmg_msg << attacker->getName() << " misses!";
         engine.getGUI()->message(dmg_msg.str(), TCODColor::white);
      }
   }
}

float Attacker::getPower() const    { return this->power; }
