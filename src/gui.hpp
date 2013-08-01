#ifndef __GUI_HPP__
#define __GUI_HPP__

#include <string>
using namespace std;

class GUI {

public:
   GUI();
   ~GUI();
   void render();
   void message(const TCODColor &col, const char *text, ...);

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
};

#endif
