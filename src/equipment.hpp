#ifndef __EQUIPMENT_HPP__
#define __EQUIPMENT_HPP__

class Actor;

class Equipment {

public:

   Equipment();

   bool equip(Actor *me, Actor *equipment);
   bool remove(Actor *me, Actor *equipment);   

   float getWeaponStr() const;
   int getArmorDef() const;
   bool isEquipped(Actor *me, Actor *item) const;

private:
   Actor *head;
   Actor *body;
   Actor *weapon;
   Actor *subWeapon;
   Actor *lFinger;
   Actor *rFinger;
   Actor *feet;
};

#endif
