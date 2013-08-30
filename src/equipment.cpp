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
      case Equippable::WEAPON:
         if (weapon == NULL) {
            weapon = equipment;            
            return true;
         } 
         break;
      case Equippable::ARMOR_BODY:
         if (body == NULL) {
            body = equipment;
            return true;
         }
         break;
      default:
         return false;
         break;
   }

   return false;
}

bool Equipment::isEquipped(Actor *me, Actor *equipment) const {
   switch(equipment->getEquippable()->getType()) {
   case Equippable::WEAPON:
      if (this->weapon == equipment)      return true;
      break;
   case Equippable::ARMOR_BODY:
      if (this->body == equipment)        return true;
      break;
   default:
      return false;
   }

   return false;
}

bool Equipment::remove(Actor *me, Actor *equipment) {
   switch(equipment->getEquippable()->getType()) {
   case Equippable::WEAPON:
      if (this->weapon == equipment) {
         this->weapon = NULL;
         return true;
      }
      break;
   case Equippable::ARMOR_BODY:
      if (this->body == equipment) {
         this->body = NULL;
         return true;
      }
      break;
   default:
      return false;
   }

   return false;
}

float Equipment::getWeaponStr() const {
   if (weapon)
      return weapon->getEquippable()->getPower();
   else
      return 0;
}
