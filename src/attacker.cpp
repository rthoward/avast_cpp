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
   ostringstream attack_msg;
   attack_msg << attacker->getName() << " attacks!";

   // make sure target is destructible and alive
   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, att->getPower())) ) {
         attack_msg << " It does " << damageTaken << " points of damage.";
         engine.getGUI()->message(attack_msg.str(), TCODColor::darkerRed);
      } else {
         attack_msg << " It misses.";
         engine.getGUI()->message(attack_msg.str(), TCODColor::white);
      }
   }
}

void PlayerAttacker::attack(Actor *attacker, Actor *target) {
   float damageTaken = 0;
   Attacker *att = attacker->getAttacker();
   Destructible *dest = target->getDestructible();
   ostringstream attack_msg;
   attack_msg << "You attack the " << target->getName();

   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, att->getPower())) ) {
         attack_msg << " for " << damageTaken << " points of damage.";
      } else {
         attack_msg << ". You miss.";
      }
   }
   engine.getGUI()->message(attack_msg.str(), TCODColor::white);
}

float Attacker::getPower() const    { return this->power; }
