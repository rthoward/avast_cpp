#ifndef __AI_HPP__
#define __AI_HPP__

// forward declaration
class Actor;

class AI {

public:

   virtual void update(Actor *me)=0;

   virtual ~AI();

};

class PlayerAI : public AI {

public:
   void update(Actor *me);
   void handleActionKey(Actor *me, int ascii);
   Actor *chooseFromInventory(Actor *me);

protected:
   bool moveOrAttack(Actor *me, int targetx, int targety);
   void checkTile(Actor *actor);
};

class MonsterAI : public AI {

public:
   MonsterAI();
   void update(Actor *me);

protected:
   void moveOrAttack(Actor *me, int targetx, int targety);
   int moveCount;
};

#endif
