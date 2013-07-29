#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

// forward include
class Map;

class Engine {

private:
   TCODList<Actor *> actors;
   Actor *player;
   Map *map;
   bool computeFov;

   int screenWidth, screenHeight;
   TCOD_key_t lastKey;

public:

   enum GameStatus {
      STARTUP,
      IDLE,
      NEW_TURN,
      VICTORY,
      DEFEAT
   } gameStatus;

   Engine(int screenWidth, int screenHeight);
   ~Engine();
   void update();
   void render();

   void addActor(Actor *actor);
   void sendToFront(Actor *actor);

   TCODList<Actor *> getActorList();
   Actor* getPlayer();
   Map* getMap();
   TCOD_key_t getLastKey();
   enum GameStatus getStatus();

   void setStatus(enum GameStatus status);

   int fovRadius;
};

extern Engine engine;

#endif
