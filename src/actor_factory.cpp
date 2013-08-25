#include "libtcod.hpp"
#include "actor.hpp"
#include "attacker.hpp"
#include "ai.hpp"
#include "container.hpp"
#include "destructible.hpp"
#include "pickable.hpp"
#include "equippable.hpp"
#include "equipment.hpp"
#include "actor_factory.hpp"
#include <string>
using namespace std;

ActorFactory::ActorFactory() {

}

Actor *ActorFactory::genPlayer(int x, int y, string name) {
   Actor *actor;
   int ch = '@';
   TCODColor color = TCODColor::white;
   AI *ai = NULL;
   Destructible *destructible = NULL;
   PlayerAttacker *attacker = NULL;
   Pickable *pickable = NULL;
   Container *container = NULL;

   ai = new PlayerAI(); 
   destructible = new Destructible(30, 2, "your cadaver");
   attacker = new PlayerAttacker(5);
   container = new Container(26);
   Equipment *equipment = new Equipment();

   actor = new Actor(x, y, ch, name, color);

   if (ai)
      actor->setAI(ai);
   if (destructible)
      actor->setDestructible(destructible);
   if (attacker)
      actor->setAttacker(attacker);
   if (container)
      actor->setContainer(container);
   if (pickable)
      actor->setPickable(pickable);
   if (equipment)
      actor->setEquipment(equipment);

   return actor;
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
      case ActorFactory::F_STAIRS_UP:
         myName = "up stairs";
         ch = '<';
         color = TCODColor::white;
         break;
      case ActorFactory::F_STAIRS_DOWN:
         myName = "down stairs";
         ch = '>';
         color = TCODColor::white;
      default:
         break;
   }

   actor = new Actor(x, y, ch, myName, color);

   if (ai)
      actor->setAI(ai);
   if (destructible)
      actor->setDestructible(destructible);
   if (attacker)
      actor->setAttacker(attacker);
   if (container)
      actor->setContainer(container);
   if (pickable)
      actor->setPickable(pickable);

   return actor;
}

Actor *ActorFactory::genWeapon(int x, int y, ActorType type, string name) {

   string myName = "";
   Actor *actor;
   int ch = ')';
   TCODColor color = TCODColor::white;
   Equippable *equippable;

   switch (type) {
      case ActorFactory::W_STEEL_LONGSWORD:
         myName = "steel longsword";
         equippable = new Equippable(3, 0);
         color = TCODColor::lightBlue;
         break;
      default:
         myName = "steel longsword";
         equippable = new Equippable(3, 0);
         color = TCODColor::lightBlue;
         break;
   }

   actor = new Actor(x, y, ch, myName, color);
   actor->setPickable(new Pickable);
   equippable->setType(Equippable::WEAPON);
   actor->setEquippable(equippable);

   return actor;
}

Actor *ActorFactory::genItem(int x, int y, ActorFactory::ActorType type, string name = "") {
   Actor *actor = NULL;
   int ch = ' ';
   TCODColor color = TCODColor::white;
   Pickable *pickable = NULL;

   switch(type) {
      case ActorFactory::P_HEALING:
         ch = '!';
         name = "potion of healing";
         pickable = new Healer(5);
         break;
      default:
         break;
   }

   actor = new Actor(x, y, ch, name, color);
   if (pickable)
      actor->setPickable(pickable);
}
