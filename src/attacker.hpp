#ifndef __ATTACKER_HPP__
#define __ATTACKER_HPP__

class Attacker {

public:

   Attacker(float power);
   void attack(Actor *attacker, Actor *target);

   float getPower() const;

private:

   float power;
};


#endif 