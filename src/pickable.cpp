#include "libtcod.hpp"
#include "destructible.hpp"
#include "actor.hpp"
#include "container.hpp"
#include "engine.hpp"
#include "pickable.hpp"

bool Pickable::pick(Actor *me, Actor *wearer) {
   if (me->getContainer() && wearer->getContainer()->add(me)) {
      engine.getActorList().remove(me);
      return true;
   }
   return false;
}

bool Pickable::use(Actor *me, Actor *wearer) {
   if (wearer->getContainer()) {
      wearer->getContainer()->remove(me);
      delete me;
      return true;
   }
   
   return false;
}

// item types -----------------------------------------------------------------

Healer::Healer(float amount) : amount(amount) {}

bool Healer::use(Actor *me, Actor *wearer) {
   if (wearer->getDestructible()) {
      float amtHealed = wearer->getDestructible()->heal(amount);
      if (amtHealed > 0)      return Pickable::use(me, wearer);
   }
   return false;
}
