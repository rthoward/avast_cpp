#include "libtcod.hpp"
#include "engine.hpp"
#include "equipment.hpp"
#include "container.hpp"
#include "equippable.hpp"
#include "actor.hpp"

Equipment::Equipment() : head(NULL), body(NULL), weapon(NULL),
   subWeapon(NULL), lFinger(NULL), rFinger(NULL), feet(NULL) {};

bool Equipment::equip(Actor *me, Actor *equipment) {
   if (!equipment->getEquippable())
      return false;


   switch (equipment->getEquippable()->getType()) {
      case (Equippable::WEAPON):
         if (weapon == NULL) {
            weapon = equipment;
            me->getContainer()->remove(equipment);
            return true;
         } else 
            return false;
         break;
      default:
         return false;
         break;
   }

   return false;
}

float Equipment::getWeaponStr() const {
   return this->weapon->getEquippable()->getPower();
}