class Engine {

public:
   Engine();
   ~Engine();
   void update();
   void render();

   TCODList<Actor *> getActorList();
   Actor* getPlayer();
   Map* getMap();
   int fovRadius;

private:
   TCODList<Actor *> actors;
   Actor *player;
   Map *map;
   bool computeFov;
};

extern Engine engine;
