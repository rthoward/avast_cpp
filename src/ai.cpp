#include "libtcod.hpp"
#include "actor.hpp"
#include "destructible.hpp"
#include "attacker.hpp"
#include "engine.hpp"
#include "map.hpp"
#include "ai.hpp"
#include <iostream>
using namespace std;

void PlayerAI::update(Actor *me) {
   
   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   int dx = 0, dy = 0;

   switch(engine.lastKey.vk) {
      case TCODK_UP     : dy -= 1; break;
      case TCODK_DOWN   : dy += 1; break;
      case TCODK_LEFT   : dx -= 1; break;
      case TCODK_RIGHT  : dx += 1; break;
      default: break;
   }

   if ( (dx + dy) != 0) {
      engine.setStatus(Engine::NEW_TURN);
      if (moveOrAttack(me, me->getX() + dx, me->getY() + dy))
         engine.getMap()->computeFov();
   }
}

bool PlayerAI::moveOrAttack(Actor *me, int targetx, int targety) {

   Map *map = engine.getMap();
   Actor *actor = NULL;

   // stop player from running in to wall
   if (engine.getMap()->isWall(targetx, targety))
      return false;

   if ( !(actor = map->getActorAt(targetx, targety)) )
      return true;

   // living actor present. attack!
   if (actor->getDestructible() && !actor->getDestructible()->isDead()) {
      me->getAttacker()->attack(me, actor);
      return false;
   } 
   // corpse present. notify player 
   else if (actor->getDestructible() && actor->getDestructible()->isDead()) {
      cout << "There's a " << actor->getName() << " here\n";
   }

   me->moveTo(targetx, targety);
   return true;
}

// 

void MonsterAI::update(Actor *me) {
   if (me->getDestructible() && me->getDestructible()->isDead())
      return;

   if (engine.getMap()->isInFov(me->getX(), me->getY())) {

   }


}
