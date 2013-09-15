#include "libtcod.hpp"
#include "random_actor.hpp"
#include "actor_factory.hpp"

ActorRarity::ActorRarity(ActorType type, float rarity) {
   this->type = type;
   this->rarity = rarity;
   if (rarity == 0)
      this->weight = 0;
   else
      this->weight = (float) 1000 / rarity;
}

RandomItemGenerator::RandomItemGenerator() {
   rarityList = vector<ActorRarity>();

   // put all items here with their rarity
   rarityList.push_back(ActorRarity(P_HEALING,         2));
   rarityList.push_back(ActorRarity(P_DEATH,           8));

   vector<ActorRarity>::iterator iter;
   for (iter = rarityList.begin(); iter != rarityList.end(); iter++) {         
      totalWeight += iter->weight;
   }

}

Actor *RandomItemGenerator::getRandomItem(float bias) {
   ActorFactory factory = ActorFactory();
   TCODRandom *rng = TCODRandom::getInstance();
   float roll = rng->getFloat(bias, totalWeight);     

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

RandomMonsterGenerator::RandomMonsterGenerator() {
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

Actor *RandomMonsterGenerator::getRandomMonster(float bias) {
   ActorFactory factory = ActorFactory();
   TCODRandom *rng = TCODRandom::getInstance();
   float roll = rng->getFloat(bias, totalWeight);     

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