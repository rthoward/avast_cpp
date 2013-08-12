#ifndef __ACTOR_FACTORY_HPP__
#define __ACTOR_FACTORY_HPP__

// forward declaration
class Actor;

#include <string>

class ActorFactory {

public:

   ActorFactory();

   enum ActorType {
      PLAYER,

      M_ORC,
      M_TROLL,

      P_HEALING,

      F_STAIRS_UP,
      F_STAIRS_DOWN
   };

   Actor *generate(int x, int y, ActorType type, std::string name = "");

};

#endif
