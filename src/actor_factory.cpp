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

Actor *ActorFactory::generate(ActorType type, string name) {

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
      case PLAYER: 
         break;
      case M_ORC: 
         myName = "orc";
         ch = 'o';
         color = TCODColor::desaturatedGreen;
         ai = new MonsterAI();
         destructible = new MonsterDestructible(10, 0, "dead orc");
         attacker = new Attacker(3);
         break;
      case M_TROLL:
         myName = "troll";
         ch = 'T';
         color = TCODColor::lightYellow;
         ai = new MonsterAI();
         destructible = new MonsterDestructible(15, 0, "dead troll");
         attacker = new Attacker(5);
         break;
      case M_NEWT:
         myName = "newt";
         ch = ':';
         color = TCODColor::lightYellow;
         ai = new MonsterAI();
         destructible = new MonsterDestructible(5, 0, "dead newt");
         attacker = new Attacker(1);
         break;
      case M_GOPHER:
         myName = "gopher";
         ch = 'd';
         color = TCODColor::darkOrange;
         ai = new GopherAI();
         destructible = new MonsterDestructible(10, 2, "loafer");
         attacker = new Attacker(3);
         break;
      case F_STAIRS_UP:
         myName = "up stairs";
         ch = '<';
         color = TCODColor::white;
         break;
      case F_STAIRS_DOWN:
         myName = "down stairs";
         ch = '>';
         color = TCODColor::white;
         break;
      case F_PIT:
         myName = "pit";
         ch = '^';
         color = TCODColor::white;
         break;
      default:
         break;      
   }

   actor = new Actor(0, 0, ch, myName, color);
   actor->setType(type);

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

Actor *ActorFactory::genEquipment(int x, int y, ActorType type) {

   string myName = "";
   Actor *actor;
   string name = "";
   int ch = ')';
   TCODColor color = TCODColor::white;
   Equippable *equippable;
   Equippable::EquipType equipType;

   switch (type) {
      case W_STEEL_LONGSWORD:
         myName = "steel longsword";
         equippable = new Equippable(3, 0);
         equipType = Equippable::WEAPON;
         color = TCODColor::lightBlue;
         break;
      case A_STEEL_BREASTPLATE: 
         myName = "steel breastplate";
         ch = '[';
         equippable = new Equippable(0, 3);
         equipType = Equippable::ARMOR_BODY;
         color = TCODColor::lightBlue;
         break;
      default:
         myName = "steel longsword";
         equippable = new Equippable(3, 0);
         equipType = Equippable::WEAPON;
         color = TCODColor::lightBlue;
         break;
   }

   actor = new Actor(x, y, ch, myName, color);
   actor->setPickable(new EquipmentPickable);
   equippable->setType(equipType);
   actor->setEquippable(equippable);

   return actor;
}

Actor *ActorFactory::genItem(ActorType type) {
   Actor *actor = NULL;
   int ch = ' ';
   string name = "";
   TCODColor color = TCODColor::white;
   Pickable *pickable = NULL;

   switch(type) {
      case P_HEALING:
         ch = '!';
         name = "potion of healing";
         pickable = new Healer(5);
         break;
      case P_DEATH:
         ch = '!';
         name = "potion of death";
         pickable = new DeathPickable();
         break;
      default:
         break;
   }

   actor = new Actor(0, 0, ch, name, color);
   actor->setBlocking(false);
   if (pickable)
      actor->setPickable(pickable);

   return actor;
}

