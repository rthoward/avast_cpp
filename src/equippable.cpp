#include "equippable.hpp"

Equippable::Equippable(int power, int def) : power(power), def(def) {};

void Equippable::setPower(float power)          { this->power = power; }
void Equippable::setDef(float def)              { this->def = def; }
void Equippable::setType(enum EquipType type)   { this->type = type; }

float Equippable::getPower() const              { return this->power; }
float Equippable::getDef() const                { return this->def; }
Equippable::EquipType Equippable::getType() const { return this->type; }
