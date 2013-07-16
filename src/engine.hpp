class Engine {

public:
   Engine();
   ~Engine();
   void update();
   void render();

   TCODList<Actor *> getActorList();
   Actor* getPlayer();
   Map* getMap();

private:
   TCODList<Actor *> actors;
   Actor *player;
   Map *map;
};

extern Engine engine;
