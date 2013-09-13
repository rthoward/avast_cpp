#ifndef __RANDOM_ACTOR_HPP__
#define __RANDOM_ACTOR_HPP__

#include "actor.hpp"
#include "actor_factory.hpp"
#include "actor_list.hpp"
#include <vector>
#include "libtcod.hpp"
#include <stdio.h>
using namespace std;

class ActorRarity {
public:
   ActorType type;
   float rarity;
   float weight;

   ActorRarity() {};

   ActorRarity(ActorType type, float rarity) {
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
      rarityList = vector<ActorRarity>();

      // put all items here with their rarity
      rarityList.push_back(ActorRarity(P_HEALING,         2));
      rarityList.push_back(ActorRarity(P_DEATH,           8));

      vector<ActorRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {         
         totalWeight += iter->weight;
      }

   }

   Actor *getRandomItem(float bias = 0) {
      ActorFactory factory = ActorFactory();
      TCODRandom *rng = TCODRandom::getInstance();
      float roll = rng->getFloat(bias, totalWeight);     

      float currentWeight = totalWeight;
      vector<ActorRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {
         roll -= iter->weight;
         if (roll <= 0) {
            Actor *actor = factory.genItem(iter->type);           
            return actor;
         }
      }

      return NULL;
   }

private:
   vector<ActorRarity> rarityList;
   float totalWeight;
};

class RandomMonsterGenerator {
public:
   RandomMonsterGenerator() {
      rarityList = vector<ActorRarity>();

      // put all items here with their rarity
      rarityList.push_back(ActorRarity(M_ORC,         3));
      rarityList.push_back(ActorRarity(M_NEWT,        2));
      rarityList.push_back(ActorRarity(M_TROLL,       8));
      rarityList.push_back(ActorRarity(M_GOPHER,      9));

      vector<ActorRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {         
         totalWeight += iter->weight;
      }
   }

   Actor *getRandomMonster(float bias = 0) {
      ActorFactory factory = ActorFactory();
      TCODRandom *rng = TCODRandom::getInstance();
      float roll = rng->getFloat(bias, totalWeight);     

      float currentWeight = totalWeight;
      vector<ActorRarity>::iterator iter;
      for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {
         roll -= iter->weight;
         if (roll <= 0) {
            Actor *actor = factory.generate(iter->type);           
            return actor;
         }
      }

      return NULL;
   }

private:
   vector<ActorRarity> rarityList;
   float totalWeight;
};

extern RandomItemGenerator randomItem;
extern RandomMonsterGenerator randomMonster;

#endif