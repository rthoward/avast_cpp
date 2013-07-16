class Actor {

public:
   Actor(int x, int y, int ch, const TCODColor &col);
   void render() const;
   void move(int x, int y);
   void moveTo(int x, int y);

   int getX();
   int getY();

private:
   int x, y;
   int ch;
   TCODColor color;
};
