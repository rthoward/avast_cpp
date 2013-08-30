#ifndef __RANDOM_ACTOR_HPP__
#define __RANDOM_ACTOR_HPP__

#include "actor.hpp"
#include "actor_factory.hpp"
#include "actor_list.hpp"
#include <vector>
#include "libtcod.hpp"
#include <stdio.h>
using namespace std;

class ItemRarity {
public:
   ActorType type;
   float rarity;
   float weight;

   ItemRarity() {};

   ItemRarity(ActorType type, float rarity) {
      this->type = type;
      this->rarity = rarity;
      if (rarity == 0)
         this->weight = 0;
      else
         this->weight = (float) 1000 / rarity;
   }
};

class RandomItemGenerator {

public:
   RandomItemGenerator() {
      rarityList = vector<ItemRarity>();

      // put all items here with their rarity
      rarityList.push_back(ItemRarity(P_HEALING,         2));
      rarityList.push_back(ItemRarity(P_DEATH,           8));

      vector<ItemRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {         
         totalWeight += iter->weight;
      }

   }

   Actor *getRandomItem(float bias = 0) {
      ActorFactory factory = ActorFactory();
      TCODRandom *rng = TCODRandom::getInstance();
      float roll = rng->getFloat(bias, totalWeight);

      printf("rolled a %f\n", roll);

      float currentWeight = totalWeight;
      vector<ItemRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {
         roll -= iter->weight;
         if (roll <= 0) {
            Actor *actor = factory.genItem(iter->type);
            printf("generated a %s\n", actor->getName().c_str());
            return actor;
         }
      }

      return NULL;
   }

private:
   vector<ItemRarity> rarityList;
   float totalWeight;
};

extern RandomItemGenerator randomItem;

#endif