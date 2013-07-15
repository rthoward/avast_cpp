struct Tile {
   bool canWalk;
   Tile() : canWalk(true) {}
};

class Map {

public:

   Map(int width, int height);
   ~Map();
   bool isWall(int x, int y) const;
   void render() const;

protected:

   int width, height;
   Tile *tiles;

   void setWall(int x, int y);
};
