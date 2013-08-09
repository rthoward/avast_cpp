#include "libtcod.hpp"
#include "actor.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "container.hpp"
#include "destructible.hpp"
#include "pickable.hpp"
#include "actor_factory.hpp"
#include <string>
using namespace std;

ActorFactory::ActorFactory() {

}

Actor *ActorFactory::generate(int x, int y, ActorFactory::ActorType type, 
      string name) {

   // default humaniod actor
   string myName = "";
   Actor *actor;
   int ch = '@';
   TCODColor color = TCODColor::white;
   AI *ai = NULL;
   Destructible *destructible = NULL;
   Attacker *attacker = NULL;
   Pickable *pickable = NULL;
   Container *container = NULL;

   switch (type) {
      case ActorFactory::PLAYER: 
         myName = name;
         ai = new PlayerAI(); 
         destructible = new Destructible(30, 2, "your cadaver");
         attacker = new Attacker(5);
         container = new Container(26);
         break;
      case ActorFactory::M_ORC: 
         myName = "orc";
         ch = 'o';
         color = TCODColor::desaturatedGreen;
         ai = new MonsterAI();
         destructible = new MonsterDestructible(10, 0, "dead orc");
         attacker = new Attacker(3);
         break;
      case ActorFactory::M_TROLL:
         myName = "troll";
         ch = 'T';
         color = TCODColor::lightYellow;
         ai = new MonsterAI();
         destructible = new MonsterDestructible(15, 0, "dead troll");
         attacker = new Attacker(5);
         break;
      default:
         break;
   }

   if (!(ai))
      return NULL;

   actor = new Actor(x, y, ch, myName, color);
   actor->setAI(ai);
   actor->setDestructible(destructible);
   actor->setAttacker(attacker);
   actor->setContainer(container);
   actor->setPickable(pickable);

   return actor;
}
