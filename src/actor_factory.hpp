#ifndef __ACTOR_FACTORY_HPP__
#define __ACTOR_FACTORY_HPP__

// forward declaration
class Actor;

#include "actor_list.hpp"
#include <string>
using namespace std;

class ActorFactory {

public:

   ActorFactory();   

   Actor *genPlayer(int x, int y, string name);
   Actor *generate(ActorType type, string name = "");
   Actor *genEquipment(int x, int y, ActorType type);
   Actor *genItem(ActorType type);
};

#endif