#ifndef __RANDOM_ACTOR_HPP__
#define __RANDOM_ACTOR_HPP__

#include "actor_list.hpp"
#include "actor.hpp"
#include <vector>
using namespace std;

class ActorRarity {
public:
   ActorType type;
   float rarity;
   float weight;

   ActorRarity(ActorType type, float rarity);
};

class RandomItemGenerator {

public:
   
   RandomItemGenerator();
   Actor *getRandomItem(float bias = 0);   

private:
   vector<ActorRarity> rarityList;
   float totalWeight;
};

class RandomMonsterGenerator {
public:
   RandomMonsterGenerator();
   Actor *getRandomMonster(float bias = 0);

private:
   vector<ActorRarity> rarityList;
   float totalWeight;
};

extern RandomItemGenerator randomItem;
extern RandomMonsterGenerator randomMonster;

#endif
