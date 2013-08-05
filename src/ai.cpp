#include "libtcod.hpp"
#include "actor.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "engine.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "ai.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>
using namespace std;

static const int MONS_TRACKING_TURNS = 3;

AI::~AI() {};

void PlayerAI::update(Actor *me) {

   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   int dx = 0, dy = 0;

   switch(engine.getLastKey().vk) {
      case TCODK_UP     : dy -= 1; break;
      case TCODK_DOWN   : dy += 1; break;
      case TCODK_LEFT   : dx -= 1; break;
      case TCODK_RIGHT  : dx += 1; break;
      case TCODK_CHAR   : handleActionKey(me, engine.getLastKey().c); break;
      default           : break;
   }

   if ( (dx + dy) != 0) {
      engine.setStatus(Engine::NEW_TURN);
      if (moveOrAttack(me, me->getX() + dx, me->getY() + dy)) {
         engine.getMap()->computeFov();
      }
   }
}

void PlayerAI::handleActionKey(Actor *me, int ascii) {
   switch (ascii) {
      case ',' : 
         me->tryPickUp(me, engine.getMap()->getItemAt(me->getX(), me->getY()));
         break;
      default  : break;
   }
}

bool PlayerAI::moveOrAttack(Actor *me, int targetx, int targety) {

   Map *map = engine.getMap();
   Actor *actor = NULL;

   // stop player from running in to wall
   if (engine.getMap()->isWall(targetx, targety))
      return false;

   // actor present
   if ( (actor = map->getActorAt(targetx, targety)) ) {

      // living actor present. attack!
      if (actor->getDestructible() && !actor->getDestructible()->isDead()) {
         me->getAttacker()->attack(me, actor);
         return false;
      }
      // corpse present. notify player
      else if (actor->getDestructible() && actor->getDestructible()->isDead()) {
         cout << "There's a " << actor->getName() << " here\n";
      }
   }

   me->moveTo(targetx, targety);
   return true;
}

MonsterAI::MonsterAI() : moveCount(3) {};

void MonsterAI::update(Actor *me) {
   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   // if player is in fov, move toward him / her
   if (engine.getMap()->isInFov(me->getX(), me->getY())) {
      moveCount = MONS_TRACKING_TURNS;
   } else {
      moveCount--; 
   }

   if (moveCount > 0) {
      Actor *player = engine.getPlayer();
      moveOrAttack(me, player->getX(), player->getY());
   }
}

void MonsterAI::moveOrAttack(Actor *me, int targetx, int targety) {
   int dx = targetx - me->getX();
   int dy = targety - me->getY();
   int stepdx = (dx > 0 ? 1 : -1);
   int stepdy = (dy > 0 ? 1 : -1);
   float distance = sqrtf(dx * dx + dy * dy);

   // if out of melee range, move toward player
   if (distance >= 2) {
      // normalize vector
      dx = (int)(round(dx / distance));
      dy = (int)(round(dy / distance));

      if (engine.getMap()->canWalk(me->getX() + dx, me->getY() + dy)) {
         me->moveTo(me->getX() + dx, me->getY() + dy);
      } else if (engine.getMap()->canWalk(me->getX() + stepdx, me->getY())) {
         me->moveTo(me->getX() + stepdx, me->getY());
      } else if (engine.getMap()->canWalk(me->getX(), me->getY() + stepdy)) {
         me->moveTo(me->getX(), me->getY() + stepdy);
      }
   } else if (me->getAttacker()) {
      me->getAttacker()->attack(me, engine.getPlayer());
   }

}
