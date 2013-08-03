#ifndef __PICKABLE_HPP__
#define __PICKABLE_HPP__

class Pickable {

public:
   virtual ~Pickable();

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

#endif
