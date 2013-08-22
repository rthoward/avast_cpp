#ifndef __EQUIPPABLE_HPP__
#define __EQUIPPABLE_HPP__

class Equippable {

public:

   Equippable(int power, int def);

   enum EquipType {
      WEAPON,
      SHIELD,
      ARMOR_BODY,
      ARMOR_FEET
   } type;

   float getPower() const;
   float getDef() const;
   enum EquipType getType() const;

   void setPower(float power);
   void setDef(float def);
   void setType(enum EquipType type);


private:
   float power;
   float def;
};

#endif
