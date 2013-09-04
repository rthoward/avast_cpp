#ifndef __AI_HPP__
#define __AI_HPP__

// forward declaration
class Actor;

class AI {

public:

   AI();
   virtual void update(Actor *me)=0;
   virtual ~AI();

protected:   

   enum AIState {
      NORMAL,
      AUTORUN_LEFT,
      AUTORUN_RIGHT,
      AUTORUN_UP,
      AUTORUN_DOWN
   } aiState;

   void setState(enum AIState state) { this->aiState = state; }
   void moveRandom(Actor *me);
   bool drop(Actor *me, Actor *item);
};

class PlayerAI : public AI {

public:
   PlayerAI() : AI() {};
   void update(Actor *me);
   void handleActionKey(Actor *me, int ascii);
   Actor *chooseFromInventory(Actor *me, string filter = "");   

protected:   

   bool moveOrAttack(Actor *me, int targetx, int targety);
   void checkTile(Actor *actor);
   AIState chooseAutorun();
   bool shouldStop(Actor *me);
   void tryStaircase(Actor *me, bool up);
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
