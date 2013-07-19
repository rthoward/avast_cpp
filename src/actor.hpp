#include <string>
using namespace std;

class Actor {

public:
   Actor(int x, int y, int ch, string name, const TCODColor &col);
   void render() const;
   void update();
   bool moveOrAttack(int dx, int dy);
   void moveTo(int x, int y);

   int getX();
   int getY();
   string getName();

private:
   int x, y;
   int ch;
   string name;
   TCODColor color;
};
