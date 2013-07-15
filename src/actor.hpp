class Actor {

public:
   Actor(int x, int y, int ch, const TCODColor &col);
   void render() const;
   void move(int x, int y);

private:
   int x, y;
   int ch;
   TCODColor color;
};
