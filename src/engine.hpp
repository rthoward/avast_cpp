#ifndef __ENGINE_HPP__
#define __ENGINE_HPP__

// forward include
class Map;
class GUI;
class Actor;
class PlayerStat;

class Engine {

private:
   TCODList<Actor *> actors;
   TCODList<Actor *> futureActors;
   TCODList<Map *> maps;
   Actor *player;
   GUI *gui;
   bool computeFov;
   int currentDLevel;
   int maxDLevel;
   int turn;
   PlayerStat *playerStat;

   int screenWidth, screenHeight;
   TCOD_key_t lastKey;

   bool shouldRender(Actor *actor);
   bool shouldUpdate(Actor *actor);

public:

   enum GameStatus {
      STARTUP,
      IDLE,
      NEW_TURN,
      VICTORY,
      DEFEAT,
      QUIT
   } gameStatus;

   Engine(int screenWidth, int screenHeight);
   ~Engine();
   void update();
   void render();

   void addActor(Actor *actor);
   void addActorFuture(Actor *actor);
   void removeActor(Actor *actor);
   void sendToFront(Actor *actor);
   void sendToBack(Actor *actor);

   bool upLevel();
   bool downLevel();

   TCODList<Actor *> getActorList();
   Actor* getPlayer();
   Map* getMap();
   TCOD_key_t getLastKey();
   enum GameStatus getStatus();
   int getScreenWidth() const;
   int getScreenHeight() const;
   GUI *getGUI();
   int getCurrentDLevel() const;
   Actor *getDownStaircase() const;
   Actor *getUpStaircase() const;
   PlayerStat *getPlayerStat() const;
   int getTurn() const;

   void setStatus(enum GameStatus status);

   int fovRadius;
};

extern Engine engine;

#endif
