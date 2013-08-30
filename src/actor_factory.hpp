#ifndef __ACTOR_FACTORY_HPP__
#define __ACTOR_FACTORY_HPP__

// forward declaration
class Actor;

#include <string>
using namespace std;

class ActorFactory {

public:

   ActorFactory();

   enum ActorType {
      PLAYER,

      M_ORC,
      M_TROLL,

      P_HEALING,

      F_STAIRS_UP,
      F_STAIRS_DOWN,

      W_STEEL_LONGSWORD,
      A_STEEL_BREASTPLATE
   };

   Actor *genPlayer(int x, int y, string name);
   Actor *generate(int x, int y, ActorType type, string name = "");
   Actor *genEquipment(int x, int y, ActorType type);
   Actor *genItem(int x, int y, ActorType type);
};

#endif