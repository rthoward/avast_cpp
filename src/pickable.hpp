#ifndef __PICKABLE_HPP__
#define __PICKABLE_HPP__

class Pickable {

public:
   
   bool pick(Actor *me, Actor *wearer);
   virtual bool use(Actor *me, Actor *wearer);
};

class Healer : public Pickable {

public:
   Healer(float amount);
   bool use(Actor *me, Actor *wearer);

private:
   float amount;
};

class EquipmentPickable : public Pickable {
public:
   bool use(Actor *me, Actor *wearer);
};

class DeathPickable : public Pickable {
public:
   bool use(Actor *me, Actor *wearer);
};

#endif
