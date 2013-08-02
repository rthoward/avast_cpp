#ifndef __MAP_HPP__
#define __MAP_HPP__

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
   Actor* getActorAt(int x, int y) const;
   void computeFov();

   void addMonster(int x, int y);

protected:

   int width, height;
   Tile *tiles;
   TCODMap *map;
   friend class BspListener;

   void dig(int x1, int y1, int x2, int y2);
   void createRoom(bool first, int x1, int y1, int x2, int y2);
};

#endif