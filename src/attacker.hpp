#ifndef __ATTACKER_HPP__
#define __ATTACKER_HPP__

class Attacker {

public:

   Attacker(float power);
   virtual void attack(Actor *attacker, Actor *target);

   float getPower() const;

private:

   float power;
};

class PlayerAttacker : public Attacker {
public:
   PlayerAttacker(float power) : Attacker::Attacker(power) {};
   void attack(Actor *attacker, Actor *target);
};


#endif 
