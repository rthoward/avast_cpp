#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <string>
using namespace std;

class GUI {

public:
   GUI();
   ~GUI();
   void render();
   void message(string text, const TCODColor &col = TCODColor::lightGrey);

protected:
   TCODConsole *con;

   struct Message {
      string text;
      TCODColor col;
      Message(string text, const TCODColor &col);
      ~Message();
   };
   TCODList<Message *> log;

   void renderBar(int x, int y, int width, string name, float value,
         float maxValue, const TCODColor &barCol, const TCODColor &backCol);
   void stats(int x, int y, int width) const;
   void debug(int x, int y, int width) const;
};

#endif
