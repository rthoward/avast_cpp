class Engine {

public:
   Engine();
   ~Engine();
   void update();
   void render();

private:
   TCODList<Actor *> actors;
   Actor *player;
   Map *map;
};

extern Engine engine;
