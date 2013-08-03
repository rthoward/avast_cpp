#include "actor.hpp"
#include "pickable.hpp"

bool Pickable::pick(Actor *me, Actor *wearer) {
   if (me->getContainer() && wearer->getContainer->add(me)) {
      engine.getActorList->remove(me);
      return true;
   }
   return false;
}
