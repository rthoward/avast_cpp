// forward include
class Map;

class Engine {

private:
   TCODList<Actor *> actors;
   Actor *player;
   Map *map;
   bool computeFov;

   enum GameStatus {
      STARTUP,
      IDLE,
      NEW_TURN,
      VICTORY,
      DEFEAT
   } gameStatus;

public:
   Engine();
   ~Engine();
   void update();
   void render();

   void addActor(Actor *actor);

   TCODList<Actor *> getActorList();
   Actor* getPlayer();
   Map* getMap();
   enum GameStatus getStatus();

   void setStatus(enum GameStatus status);

   int fovRadius;
};

extern Engine engine;
