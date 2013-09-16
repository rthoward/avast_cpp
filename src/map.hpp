#ifndef __MAP_HPP__
#define __MAP_HPP__

class ActorFactory;

struct Tile {
   bool explored;
   Tile() : explored(false) {}
};

class Map {

public:

   Map(int width, int height);
   ~Map();
   bool isWall(int x, int y) const;
   void render() const;

   bool isInFov(int x, int y) const;
   bool isExplored(int x, int y) const;
   bool canWalk(int x, int y) const;
   bool isAdjacentTo(Actor *me, int x, int y) const;
   void computeFov();

   Actor* getActorAt(int x, int y) const;
   Actor* getItemAt(int x, int y)  const;
   TCODList<Actor *> getActors() const;

   void addMonster(int x, int y);
   void addItems();
   void moveActorRandom(Actor *actor);
   void addStaircases();
   void addActor(Actor *);
   

protected:

   int width, height;
   Tile *tiles;
   TCODMap *map;
   friend class BspListener;
   TCODList<Actor *> actors;

   void dig(int x1, int y1, int x2, int y2);
   void createRoom(bool first, int x1, int y1, int x2, int y2);
   bool inMap(int x, int y) const;
};

#endif
