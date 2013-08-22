#include "libtcod.hpp"
#include "engine.hpp"
#include "destructible.hpp"
#include "gui.hpp"
#include "actor.hpp"
#include "equipment.hpp"
#include "attacker.hpp"
#include <string>
#include <sstream>
using namespace std;

Attacker::Attacker(float power) : power(power) {

}

void Attacker::attack(Actor *attacker, Actor *target) {

   float calcDamage = 0;
   float damageTaken = 0;
   float weaponPower = 0;
   Attacker *att = attacker->getAttacker();
   Destructible *dest = target->getDestructible();
   ostringstream attack_msg;
   attack_msg << attacker->getName() << " attacks!";

   if (attacker->getEquipment())
      weaponPower = attacker->getEquipment()->getWeaponStr();
   calcDamage = att->getPower() + weaponPower;

   // make sure target is destructible and alive
   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, calcDamage)) ) {
         attack_msg << " It does " << damageTaken << " points of damage.";
         engine.getGUI()->message(attack_msg.str(), TCODColor::darkerRed);
      } else {
         attack_msg << " It misses.";
         engine.getGUI()->message(attack_msg.str(), TCODColor::white);
      }
   }
}

void PlayerAttacker::attack(Actor *attacker, Actor *target) {
   float calcDamage = 0;
   float damageTaken = 0;
   float weaponPower = 0;
   Attacker *att = attacker->getAttacker();
   Destructible *dest = target->getDestructible();
   ostringstream attack_msg;
   attack_msg << "You attack the " << target->getName();

   if (attacker->getEquipment())
      weaponPower = attacker->getEquipment()->getWeaponStr();
   calcDamage = att->getPower() + weaponPower;

   if (target->getDestructible() && !target->getDestructible()->isDead()) {
      if ( (damageTaken = dest->takeDamage(target, calcDamage)) ) {
         attack_msg << " for " << damageTaken << " points of damage.";
      } else {
         attack_msg << ". You miss.";
      }
   }
   engine.getGUI()->message(attack_msg.str(), TCODColor::white);

   // did we kill it?
   if (target->isDead()) {
      attacker->addExp(target->getDestructible()->getExp());
   }
}

float Attacker::getPower() const    { return this->power; }
